/*
 * adcFreezeCal.c
 *
 *  Created on: Aug 10, 2019
 *      Author: jlantz
 */

/***************************** Include Files *********************************/
#include "xil_io.h"
#include "xil_types.h"
#include <stdio.h>
#include "cli.h"
#include "xparameters.h"
#include "main.h"
#include "xrfdc.h"
#include "adcSaveCalCoefficients.h"


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
void storedCoeffDisplay(perAdcCoeffType *destCoeffPtr, int coeff_set);
void storedCoeffWrite(XRFdc_Calibration_Coefficients *srcCoeff, perAdcCoeffType *destCoeffPtr, int coeff_set);

/************************** Variable Definitions *****************************/

/* stored coefficient are as follows:
	There are 4 Sets of coefficients
	Each set contains an entry for each of 4 tiles
	Each tile entry contains 4 block entries
	Each block entry contains 3 types of coefficients
	Each type of coefficient contains a 'written' flag and an array for 8 coeff values
	All values are 32bit values (4 bytes).

	The linkerscript (lscript.ld) contains a memory definition which is used
	by section .coeff_mem to set aside memory for storing the coefficients.
	This section of memory is located above the application memory.
	Refer to the lscript.ld file for the following entries used by the coeff memory

	_COEFF_SIZE = 4 *						# sets
	                 (4 *					# tiles
	                     (4 *				# blocks per tile
	                         (3 *			# number of different coefficients
	                             (4 +  		# coeff 'written' flag
	                                 (8*4)	# 8 32bit coeff values per type of coeff
	                             )
	                         )
	                     )
	                 );

	MEMORY
	{
	   ..
	   ..
	   psu_ddr_0_MEM_0     : ORIGIN = 0x0, LENGTH = 0x00100000 - _COEFF_SIZE
	   psu_ddr_0_MEM_COEFF : ORIGIN = 0x00100000 - _COEFF_SIZE, LENGTH = _COEFF_SIZE
	   ..
	   ..
	}

	SECTIONS
	{
		..
		..

		.coeff_mem (NOLOAD) : {
		   _CoeffMemBase = .;
		   KEEP(*(.coeffSection));
		   _CoeffMemEnd = .;
		} > psu_ddr_0_MEM_COEFF

		..
		..
	}
*/
perAdcCoeffType storedCoeffSets[NUM_SETS][NUM_TILES][NUM_BLOCKS] __attribute__((section (".coeffSection")));

volatile int selectedCoeffSet = 0;


/************************** Function De__attribute__finitions ******************************/


/*****************************************************************************/
/**
*
* cli_adcSaveCalCoeff_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_adcSaveCalCoeff_init(void)
{
	static CMDSTRUCT cliCmds[] = {
			{"adcCoeffSet"        , "- <set Num> Select coeff set"            , 1, *adcCoeffSet},
			{"adcSaveCalCoeff"    , "- <tile> <adc> Save Cal Coeff for ch"    , 2, *adcSaveCalCoeff},
			{"adcCoeffSetDump"    , "- <tile> <adc> <set> Dump coeff set"     , 3, *adcCoeffSetDump},
			{"adcCoeffClr"        , "- <set Num> Clear entered coeff set"     , 1, *adcCoeffClr},
	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}

/*****************************************************************************/
/**
*
* adcCoeffSetClr TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcCoeffClr (u32 *cmdVals)
{
	int EnteredSet;

	EnteredSet = cmdVals[0];

	// Index through the set's structure and clear all values.
	// Note: Using for loops is done for clarity of memory layout.
	// Ideal method to clear memory is using:
	// memset() for sizeof(storedCoeffSets[EnteredSet])
	if((EnteredSet >= 0) && (EnteredSet < NUM_SETS)) {
		for(int tile=0; tile < NUM_TILES; tile++) {
			for(int block=0; block < NUM_BLOCKS; block++) {
				for(int typeIndex=0; typeIndex<4; typeIndex++) {
					storedCoeffSets[EnteredSet][tile][block].type[typeIndex].written = 0;
					for(int coeffIndex=0; coeffIndex<8; coeffIndex++) {
						storedCoeffSets[EnteredSet][tile][block].type[typeIndex].Coeff[coeffIndex] = 0;
					}
				}
			}
		}
		xil_printf("Cleared Coeff Set %d at BaseAddr: %08x Size: %04x\r\n", EnteredSet, storedCoeffSets[selectedCoeffSet], sizeof(storedCoeffSets[selectedCoeffSet]));
	} else {
		xil_printf("Error: set number %d is out of range. Must be within range 0-%d\r\n", EnteredSet, NUM_SETS-1);
		return;
	}

	return;
}


/*****************************************************************************/
/**
*
* adcCoeffSet TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcCoeffSet (u32 *cmdVals)
{
	int EnteredSet;

	EnteredSet = cmdVals[0];

	if((EnteredSet >= 0) && (EnteredSet < NUM_SETS)) {
		selectedCoeffSet = EnteredSet;
		xil_printf("Set Coeff Set to %d at BaseAddr: %08x Size: %04x\r\n", selectedCoeffSet, storedCoeffSets[selectedCoeffSet], sizeof(storedCoeffSets[selectedCoeffSet]));
	} else {
		xil_printf("Error: set number %d is out of range. Must be within range 0-%d\r\n", EnteredSet, NUM_SETS-1);
		return;
	}

	return;
}


/*****************************************************************************/
/**
*
* adcLoadCalCoeff TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcSaveCalCoeff (u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	XRFdc_Calibration_Coefficients calCoeff;

	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\n\r");
	xil_printf("=== ADC Save Coefficients Report ===\n\r");
	xil_printf("   Saving Coeff Set: %d  at BaseAddr: 0x%08x Size: %04x\n\r", selectedCoeffSet, storedCoeffSets[selectedCoeffSet], sizeof(storedCoeffSets[selectedCoeffSet]));

	if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id) == 1) {

		///////////////////////////////////////////////////////////////
		// Process OCB1 coefficients

		// load OCB1 coefficients from ADC
		XRFdc_GetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id,XRFDC_CAL_BLOCK_OCB1, &calCoeff);

		// Save OCB1 coefficients and display saved values
		storedCoeffWrite(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_OCB1);
		xil_printf("   ADC Tile%d ch%d OCB1 Cal Coefficients: \r\n", Tile_Id, Block_Id);
		storedCoeffDisplay(&storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_OCB1);


		///////////////////////////////////////////////////////////////
		// Process OCB2 coefficients

		// load OCB2 coefficients from ADC
		XRFdc_GetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id,XRFDC_CAL_BLOCK_OCB2, &calCoeff);

		// Save OCB2 coefficients and display saved values
		storedCoeffWrite(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_OCB2);
		xil_printf("   ADC Tile%d ch%d OCB2 Cal Coefficients: \r\n", Tile_Id, Block_Id);
		storedCoeffDisplay(&storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_OCB2);


		///////////////////////////////////////////////////////////////
		// Process GCB coefficients

		// load GCB coefficients from ADC
		XRFdc_GetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id,XRFDC_CAL_BLOCK_GCB, &calCoeff);

		// Save GCB coefficients and display saved values
		storedCoeffWrite(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_GCB);
		xil_printf("   ADC Tile%d ch%d GCB Cal Coefficients: \r\n", Tile_Id, Block_Id);
		storedCoeffDisplay(&storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_GCB);


		///////////////////////////////////////////////////////////////
		// Process TSCB coefficients

		// load TSCB coefficients from ADC
		XRFdc_GetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id,XRFDC_CAL_BLOCK_TSCB, &calCoeff);

		// Save TSCB coefficients and display saved values
		storedCoeffWrite(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_TSCB);
		xil_printf("   ADC Tile%d ch%d TSCB Cal Coefficients: \r\n", Tile_Id, Block_Id);
		storedCoeffDisplay(&storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_TSCB);

	} else {
		(xil_printf("Tile%d Ch%d is not available.\n\r",Tile_Id,Block_Id ));
	}

	xil_printf("###############################################\r\n\n");

	return;
}

/*****************************************************************************/
/**
*
* adcCoeffSetDump Display contents from stored coeff set
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcCoeffSetDump (u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	u32 coeffSet;

	Tile_Id  = cmdVals[0];
	Block_Id = cmdVals[1];
	coeffSet = cmdVals[2];

	xil_printf("\n\r###############################################\n\r");
	xil_printf("=== ADC Save Coefficients Report ===\n\r");



	if((Tile_Id < NUM_TILES) && (Block_Id < NUM_BLOCKS) && (coeffSet < NUM_SETS)) {

		xil_printf("   Coeff Set: %d  at BaseAddr: 0x%08x Size: %04x\n\r", coeffSet, storedCoeffSets[coeffSet], sizeof(storedCoeffSets[coeffSet]));

		// Display OCB1 coefficients
		xil_printf("   ADC Tile%d ch%d OCB1 Cal Coefficients: \r\n", Tile_Id, Block_Id);
		storedCoeffDisplay(&storedCoeffSets[coeffSet][Tile_Id][Block_Id], COEFF_OCB1);

		// Display OCB2 coefficients
		xil_printf("   ADC Tile%d ch%d OCB2 Cal Coefficients: \r\n", Tile_Id, Block_Id);
		storedCoeffDisplay(&storedCoeffSets[coeffSet][Tile_Id][Block_Id], COEFF_OCB2);

		// Display GCB coefficients
		xil_printf("   ADC Tile%d ch%d GCB Cal Coefficients: \r\n", Tile_Id, Block_Id);
		storedCoeffDisplay(&storedCoeffSets[coeffSet][Tile_Id][Block_Id], COEFF_GCB);

		// Display TSCB coefficients
		xil_printf("   ADC Tile%d ch%d TSCB Cal Coefficients: \r\n", Tile_Id, Block_Id);
		storedCoeffDisplay(&storedCoeffSets[coeffSet][Tile_Id][Block_Id], COEFF_TSCB);
	}
	else
	{
		xil_printf("   ADC Tile%d ch%d coefficient set %d is not available.\r\n", Tile_Id, Block_Id, coeffSet);
	}
	xil_printf("###############################################\r\n\n");

	return;
}

/*****************************************************************************/
/**
*
* storedCoeffDisplay low level display
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void storedCoeffDisplay(perAdcCoeffType *destCoeffPtr, int coeff_set)
{
	for(int i=0; i<8; i++) {
//		xil_printf("      Coeff%d: %11d\r\n", i, destCoeffPtr->type[coeff_set].Coeff[i]);
		xil_printf("      Coeff%d: 0x%08x : %11d\r\n", i, destCoeffPtr->type[coeff_set].Coeff[i], destCoeffPtr->type[coeff_set].Coeff[i]);
	}
	return;
}

/*****************************************************************************/
/**
*
* storedCoeffWrite Write coeff to globals
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void storedCoeffWrite(XRFdc_Calibration_Coefficients *srcCoeff, perAdcCoeffType *destCoeffPtr, int coeff_set)
{

	destCoeffPtr->type[coeff_set].written = 1;
	destCoeffPtr->type[coeff_set].Coeff[0] =  srcCoeff->Coeff0;
	destCoeffPtr->type[coeff_set].Coeff[1] =  srcCoeff->Coeff1;
	destCoeffPtr->type[coeff_set].Coeff[2] =  srcCoeff->Coeff2;
	destCoeffPtr->type[coeff_set].Coeff[3] =  srcCoeff->Coeff3;
	destCoeffPtr->type[coeff_set].Coeff[4] =  srcCoeff->Coeff4;
	destCoeffPtr->type[coeff_set].Coeff[5] =  srcCoeff->Coeff5;
	destCoeffPtr->type[coeff_set].Coeff[6] =  srcCoeff->Coeff6;
	destCoeffPtr->type[coeff_set].Coeff[7] =  srcCoeff->Coeff7;
	return;
}
