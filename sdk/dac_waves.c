/*
 * play_dma.c
 *
 *  Created on: 11may20
 *      Author: jmcd
 *
 *  Note: dacSin(), & dacSinCos() require the math library.
 *        To set: C/C++ build settings requires -lm (Library 'm' added) to
 *        gcc Linker.
 */
/***************************** Include Files *********************************/
#include "dac_waves.h"
#include "xil_io.h"
#include "xil_types.h"
#include <stdio.h>
#include "cli.h"
#include "xparameters.h"
#include "main.h"
#include <math.h>
#include "xrfdc.h"
#include "xil_cache.h"



/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern XRFdc RFdcInst;
volatile void *bufAddr;
volatile u32 maxBufSize;
volatile int bufSet;

/*****************************************************************************/
/**
*
* cli_play_dma_init: Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_dac_waves_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		//000000000011111111112222    000000000011111111112222222222333333333
		//012345678901234567890123    012345678901234567890123456789012345678
		{"###################### DAC WAVE #######################", " "			    , 0, *cmdComment},
		{"waveSetBuf"  , "<addr_h> <addr_l> <size KB> - set waveform buffer addr"	, 3, *waveSetBuf},
		{"waveSawtooth" , "<numKB> <increment per sample> - write sawtooth to DDR"	, 2, *waveSawtooth},
		{"waveSin"      , "<numKB> <freq (Hz)> <dBFS>"                              , 3, *waveSin},
//		{"waveSinCos"   , "<numKB> <freq (Hz)> <dBFS>"                              , 3, *dacSinCos}, // This is for an IQ design but the current design is real.
		{" "		   , " "														, 0, *cmdComment},
	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));

}

/*****************************************************************************/
/**
*
* waveSetBuf: sets buffer addr used by waveform fill functions.
*
* @param	cmdVals[0] = BufferAddr high
*           cmdVals[1] = BufferAddr low
*           cmdVals[2] = max buf size used for DRC checks
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void waveSetBuf (u32 *cmdVals)
{
	bufAddr = (void *)( ((u64)(cmdVals[0]) << 32) | (u64)cmdVals[1] );
	maxBufSize = cmdVals[2] * 1024;
	bufSet = 1;

	xil_printf("Play buffer addr set to: %p, and Max size to: 0x%08x\n\n\r", bufAddr, maxBufSize);
	return;
}


/*****************************************************************************/
/**
*
* sawtooth: Writes a sawtooth wave pattern into TX buffer memory space with
* user-specified size and increment per sample.
*
* @param	cmdVals[0] = numKBytes  cmdVals[1] = increment value
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void waveSawtooth (u32 cmdVals[])
{
	u32 NumBytes;
	u32 increment;
	u16 *TxPacket;
	u16 Value;
	int Index;

	NumBytes = cmdVals[0] * 1024;
	increment = cmdVals[1];
	TxPacket = (void *) bufAddr;

	if (!bufSet) {
		xil_printf("Must run waveSetBuf() first to setup buffer location and size\n\r");
		return;
	}

	if (NumBytes < 0x1000) {
		NumBytes = 0x1000;
		xil_printf("Warning: number of bytes must be at least 4K. Rounding up.\r\n");
	}
	if (NumBytes > maxBufSize) {
		NumBytes = maxBufSize;
		xil_printf("Warning: number of bytes exceeded TX buffer memory space. Reducing to max.\r\n");
	}
	if (NumBytes % 0x1000 != 0) {
		xil_printf("Warning: number of bytes must be 4K aligned. Rounding down.\r\n");
		NumBytes &= ~0xFFF;
	}

	// Verify increment value is a power of 2
	if ( !(increment && (!(increment & (increment-1)))) ) {
		increment = 1;
		xil_printf("Warning: increment per sample must be a power of 2 in order to be evenly divisible into 4KB. Defaulting to 1.\r\n");
	}


	xil_printf("Writing %d bytes of sawtooth pattern with increment value of %d to memory location 0x%p\r\n", NumBytes, increment, TxPacket);


	// each sample is 16bits
	Value = 0x0;
	for(Index = 0; Index < NumBytes/sizeof(TxPacket[0]); Index++) {
		TxPacket[Index] = Value;

		Value += increment;
	}

	/* Flush the SrcBuffer before the DMA transfer, in case the Data Cache
	 * is enabled
	 */
	Xil_DCacheFlushRange((UINTPTR)TxPacket, NumBytes);
	return;
}



/*****************************************************************************/
/**
*
* sinCos Gen will fill memory with a I/Q sine wave
*
* @param	cmdVals[0] = numKBytes cmdVals[1] = freq  cmdVals[2] = dBFS
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacSinCos(u32 *cmdVals) {
	s16 iVal, qVal;
	u32 *TxPacket;
	u32 NumBytes;
	int amplitude;
	double SamplingFreq;
	double dBFS;
	double freq;
	double stepSize;
	double newFreq;
	u32 numCycles;
	int Status;

	NumBytes = cmdVals[0] * 1024;
	freq = (int)cmdVals[1];
	dBFS = (int)cmdVals[2];

	// TxPacket is u32 since it points to I/Q combination
	TxPacket = (void *) bufAddr;

	if (!bufSet) {
		xil_printf("Must run waveSetBuf() first to setup buffer location and size\n\r");
		return;
	}

	if (NumBytes < 0x1000) {
		NumBytes = 0x1000;
		xil_printf("Warning: number of bytes must be at least 4K. Rounding up.\r\n");
	}
	if (NumBytes > maxBufSize) {
		NumBytes = maxBufSize;
		xil_printf("Warning: number of bytes exceeded TX buffer memory space. Reducing to max.\r\n");
	}
	if (NumBytes % 0x1000 != 0) {
		xil_printf("Warning: number of bytes must be 4K aligned. Rounding up.\r\n");
		NumBytes += 0x1000;
		NumBytes &= ~0xFFF;
	}

	//Calculate amplitude in hex from input of dBFS
	{
		double ratio;

		ratio = pow(10.0, (dBFS/20));
		amplitude = 0x7ffe * ratio;
		printf("ratio: %f, amplitude: 0x%08x\r\n", ratio, amplitude);

		if(amplitude>0x7FFF) {
			xil_printf("ERROR: Amplitude greater than 16bits\r\n");
			return;
		}
	}

	//Get sampling rate and interpolation of DAC 7 and use for sine freq calculation
	{
		XRFdc* RFdcInstPtr = &RFdcInst;
		XRFdc_BlockStatus blockStatus;
	    u32 InterpolationFactor;

		if(XRFdc_IsDACBlockEnabled(RFdcInstPtr, 1, 3)) {
			XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_DAC_TILE, 1, 3, &blockStatus);
			SamplingFreq = blockStatus.SamplingFreq * 1e9;
		} else {
			xil_printf("Error reading DAC 7 sampling rate\r\n");
			return;
		}

		Status = XRFdc_GetInterpolationFactor(RFdcInstPtr, 1, 3, &InterpolationFactor);
		if (Status != XST_SUCCESS) {
			xil_printf("XRFdc_GetInterpolationFactor() failed\r\n");
			return;
		}

		// update Sampling Freq with interpolation factor to represent sampling freq
		// of axis interface
		SamplingFreq = SamplingFreq / (float)InterpolationFactor;

	}


	//determine how many sine waves fit in buffer then round up to slightly
	//increase freq to fit buffer
	numCycles = (float)((NumBytes/sizeof(TxPacket[0])) / (SamplingFreq / freq));

	// determine new freq that evenly fits into buffer
	newFreq = 1/ ((NumBytes/sizeof(TxPacket[0])) / numCycles * 1/SamplingFreq);


	stepSize = (float)360 / ((float)(NumBytes/sizeof(TxPacket[0])) / numCycles);

	printf("Adjusted Sampling Frequency: %f Hz\r\n", SamplingFreq);
	printf("freq:                        %f\r\n", freq);
	printf("newFreq:                     %f\r\n", newFreq);
	printf("num cycles in buffer:  %d\r\n", numCycles);
	printf("calculated step size:  %f\r\n", stepSize);


	xil_printf("Writing %d bytes of sin/cos pattern to memory location 0x%p\r\n", NumBytes, TxPacket);

    for(int i=0; i < NumBytes/sizeof(TxPacket[0]); i++) {
    	// y(t) = A * sin(2 * PI * f * t + shift)
    	// A = amplitude, peak deviation of function from zero
    	// f = ordinary freq, number of oscillations (cycles)
    	// t = time
    	// shift = phase shift

    	float rads = M_PI/180;

    	// Calculate I/Q values and write to memory
    	iVal = amplitude * sin(stepSize * i * rads);
    	qVal = amplitude * cos(stepSize * i * rads);
    	TxPacket[i] = ((iVal << 16) & 0xFFFF0000) | (qVal & 0xFFFF);

 //   	if( (i > 140) && (i < 160) ) {
 //   		xil_printf("i: %d addr: 0x%p I/Q: 0x%04x 0x%04x\r\n",i, &TxPacket[i],iVal, qVal);
 //   	}
    }

	/* Flush the SrcBuffer before the DMA transfer, in case the Data Cache
	 * is enabled
	 */
	Xil_DCacheFlushRange((UINTPTR)TxPacket, NumBytes);
	return;
}


/*****************************************************************************/
/**
*
* sin Gen will fill memory with a Real sine wave
*
* @param	cmdVals[0] = numKBytes cmdVals[1] = freq  cmdVals[2] = dBFS
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void waveSin(u32 *cmdVals) {
	s16 Val;
	u16 *TxPacket;
	u32 NumBytes;
	int amplitude;
	double SamplingFreq;
	double dBFS;
	double freq;
	double stepSize;
	double newFreq;
	u32 numCycles;
	int Status;

	NumBytes = cmdVals[0] * 1024;
	freq = (int)cmdVals[1];
	dBFS = (int)cmdVals[2];

	// TxPacket is u16 since it points to Real combination
	TxPacket = (void *) bufAddr;

	if (!bufSet) {
		xil_printf("Must run waveSetBuf() first to setup buffer location and size\n\r");
		return;
	}

	if (NumBytes < 0x1000) {
		NumBytes = 0x1000;
		xil_printf("Warning: number of bytes must be at least 4K. Rounding up.\r\n");
	}
	if (NumBytes > maxBufSize) {
		NumBytes = maxBufSize;
		xil_printf("Warning: number of bytes exceeded TX buffer memory space. Reducing to max.\r\n");
	}
	if (NumBytes % 0x1000 != 0) {
		xil_printf("Warning: number of bytes must be 4K aligned. Rounding up.\r\n");
		NumBytes += 0x1000;
		NumBytes &= ~0xFFF;
	}

	//Calculate amplitude in hex from input of dBFS
	{
		double ratio;

		ratio = pow(10.0, (dBFS/20));
		amplitude = 0x7ffe * ratio;
		printf("ratio: %f, amplitude: 0x%08x\r\n", ratio, amplitude);

		if(amplitude>0x7FFF) {
			xil_printf("ERROR: Amplitude greater than 16bits\r\n");
			return;
		}
	}

	//Get sampling rate and interpolation of DAC 7 and use for sine freq calculation
	{
		XRFdc* RFdcInstPtr = &RFdcInst;
		XRFdc_BlockStatus blockStatus;
	    u32 InterpolationFactor;

		if(XRFdc_IsDACBlockEnabled(RFdcInstPtr, 0, 0)) {
			XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_DAC_TILE, 0, 0, &blockStatus);
			SamplingFreq = blockStatus.SamplingFreq * 1e9;
		} else {
			xil_printf("Error reading DAC 7 sampling rate\r\n");
			return;
		}

		Status = XRFdc_GetInterpolationFactor(RFdcInstPtr, 0, 0, &InterpolationFactor);
		if (Status != XST_SUCCESS) {
			xil_printf("XRFdc_GetInterpolationFactor() failed\r\n");
			return;
		}

		// update Sampling Freq with interpolation factor to represent sampling freq
		// of axis interface
		SamplingFreq = SamplingFreq / (float)InterpolationFactor;
	}


	//determine how many sine waves fit in buffer then round up to slightly
	//increase freq to fit buffer
	numCycles = (float)((NumBytes/sizeof(TxPacket[0])) / (SamplingFreq / freq));

	// determine new freq that evenly fits into buffer
	newFreq = 1/(((1/SamplingFreq)*NumBytes/sizeof(TxPacket[0]))/numCycles);


	stepSize = (float)360 / ((float)(NumBytes/sizeof(TxPacket[0])) / numCycles);

	printf("Adjusted Sampling Frequency: %f Hz\r\n", SamplingFreq);
	printf("Requested Frequency:         %f Hz\r\n", freq);
	printf("Adjusted Frequency:          %f Hz\r\n", newFreq);
	printf("num cycles in buffer:  %d\r\n", numCycles);
	printf("calculated step size:  %f\r\n", stepSize);


	xil_printf("Writing %d bytes of sin pattern to memory location 0x%p\r\n", NumBytes, TxPacket);

    for(int i=0; i < NumBytes/sizeof(TxPacket[0]); i++) {
    	// y(t) = A * sin(2 * PI * f * t + shift)
    	// A = amplitude, peak deviation of function from zero
    	// f = ordinary freq, number of oscillations (cycles)
    	// t = time
    	// shift = phase shift

    	float rads = M_PI/180;

    	// Calculate Sine values and write to memory
    	Val = amplitude * sin(stepSize * i * rads);
    	TxPacket[i] = Val;
#if 0
    	if( (i > 140) && (i < 160) ) {
    		xil_printf("i: %d addr: 0x%p Real: 0x%04x\r\n", i, &TxPacket[i], Val);
    	}
#endif
    }

	/* Flush the SrcBuffer before the DMA transfer, in case the Data Cache
	 * is enabled
	 */
	Xil_DCacheFlushRange((UINTPTR)TxPacket, NumBytes);
	return;
}
