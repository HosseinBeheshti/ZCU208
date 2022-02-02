


/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include "xil_io.h"
#include "xil_types.h"
#include "cli.h"
#include "xparameters.h"
#include "xrfdc.h"
#include "xrfdc_mts.h"
#include "main.h"
#include "rfdc_mts.h"


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
int mygetline(void);

/************************** Variable Definitions *****************************/

XRFdc_MultiConverter_Sync_Config ADC_Sync_Config;
XRFdc_MultiConverter_Sync_Config DAC_Sync_Config;


/************************** Function Definitions ******************************/


/*****************************************************************************/
/**
*
* cli_cmd_func_dac_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_cmd_mts_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		//000000000011111111112222    000000000011111111112222222222333333333
		//012345678901234567890123    012345678901234567890123456789012345678
		{"################### DAC and ADC MTS ####################" , " " , 0, *cmdComment   },
		{"dacMTS"             , "<tileMask> - DAC MTS"                    , 1, *dacMTS},
		{"dacMTSwl"           , "<tileMask> - DAC MTS with fixed latency" , 1, *dacMTSwl},
		{"adcMTS"             , "<tileMask> - ADC MTS"                    , 1, *adcMTS},
		{"adcMTSwl"           , "<tileMask> - ADC MTS with fixed latency" , 1, *adcMTSwl},
		{"dacAdcMTSStatus"    , "- Dump DAC & ADC Sync status"            , 0, *dacAdcMTSStatus},
		{" "                       , " "                                  , 0, *cmdComment   },

	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}




/*****************************************************************************/
/**
*
* Dac Sync Set
*
* @param	cmdVals[0] bit pattern for DAC tiles to sync. I.E Dac tiles 0 and 1 = 0x3
*           cmdVals[1] 0=event src tile, 1=pl event
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacMTS(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 tiles;
	int status;
    int i;
    u32 factor;

	tiles = cmdVals[0];

	// Set MTS
	xil_printf("\r\n###############################################\r\n");
	xil_printf("Enabling DAC Multi-Tile Synchronization...\n\r\n");

    XRFdc_MultiConverter_Init (&DAC_Sync_Config, 0, 0);
    DAC_Sync_Config.Tiles = tiles;	/* Sync DAC tiles as defined by bits set */
    xil_printf("DAC_Sync_Config.Tiles: 0x%08x\r\n", DAC_Sync_Config.Tiles);
	xil_printf("\n=== Multi-Tile Synchronization Metal Log Report ===\r\n");
   status = XRFdc_MultiConverter_Sync(RFdcInstPtr, XRFDC_DAC_TILE, &DAC_Sync_Config);
    if(status != XRFDC_MTS_OK) {
    	xil_printf("XRFdc_MultiConverter_Sync() FAILED\r\n");
    	return;
    }

    if(status == XRFDC_MTS_OK) {
    	xil_printf("\n========== DAC Multi-Tile Sync Report ==========\r\n");
    	for(i=0; i<4; i++) {
    		if((1<<i)&DAC_Sync_Config.Tiles) {
                XRFdc_GetInterpolationFactor(RFdcInstPtr, i, 0, &factor);
                xil_printf("DAC%d: Latency(T1) = %3d, Adjusted Delay "
				 "Offset(T%d) = %3d, Marker Delay = %d \r\n", i, DAC_Sync_Config.Latency[i],
						 factor, DAC_Sync_Config.Offset[i],DAC_Sync_Config.Marker_Delay);
                xil_printf("=== MTS DAC Tile%d PLL Report ===\r\n",i);
                xil_printf("    DAC%d: PLL DTC Code =%d \n\r", i, DAC_Sync_Config.DTC_Set_PLL.DTC_Code[i]);
                xil_printf("    DAC%d: PLL Num Windows =%d \n\r", i, DAC_Sync_Config.DTC_Set_PLL.Num_Windows[i]);
                xil_printf("    DAC%d: PLL Max Gap =%d \n\r", i, DAC_Sync_Config.DTC_Set_PLL.Max_Gap[i]);
                xil_printf("    DAC%d: PLL Min Gap =%d \n\r", i, DAC_Sync_Config.DTC_Set_PLL.Min_Gap[i]);
                xil_printf("    DAC%d: PLL Max Overlap =%d \n\r", i, DAC_Sync_Config.DTC_Set_PLL.Max_Overlap[i]);
                xil_printf("=== MTS DAC Tile%d T1 Report ===\r\n",i);
                xil_printf("    DAC%d: T1 DTC Code =%d \n\r", i, DAC_Sync_Config.DTC_Set_T1.DTC_Code[i]);
                xil_printf("    DAC%d: T1 Num Windows =%d \n\r", i, DAC_Sync_Config.DTC_Set_T1.Num_Windows[i]);
                xil_printf("    DAC%d: T1 Max Gap =%d \n\r", i, DAC_Sync_Config.DTC_Set_T1.Max_Gap[i]);
                xil_printf("    DAC%d: T1 Min Gap =%d \n\r", i, DAC_Sync_Config.DTC_Set_T1.Min_Gap[i]);
                xil_printf("    DAC%d: T1 Max Overlap =%d \n\n\r", i, DAC_Sync_Config.DTC_Set_T1.Max_Overlap[i]);

    		}
    	}

		xil_printf("DAC Multi-Tile Synchronization is complete.");
		xil_printf("\r\n###############################################\r\n");
	}

    return;
}


/*****************************************************************************/
/**
*
* mygetline
*
* @param	get line from console
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
int mygetline()
{
	char line[10];
	int i=0;
	char mychar;
	int finalVal;

	while(1) {
		 mychar=inbyte();
		 if (mychar == 0x0a) continue;
		 if (mychar == 0x0d) break;
		 if(i>=8) break;
		 xil_printf("%c", mychar);
		 line[i++]=mychar;
	}

	line[i] = 0;
	finalVal = atoi(&line[0]);

	return finalVal;
}

/*****************************************************************************/
/**
*
* Dac Sync Set with fixed latency
*
* @param	Keyboard entry required
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacMTSwl(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 tiles;
	int i;
	int status;
	u32 factor;
	int lat_test;

	tiles = cmdVals[0];

	xil_printf("\r\n###############################################\r\n");
	xil_printf("Enabling DAC Multi-Tile Synchronization with fixed latency...\n\r\n");
	xil_printf("Set latency to : ");
	lat_test = mygetline();
	xil_printf("\r\n    Latency set to : %d\r\n",lat_test);

	// Set MTS
    XRFdc_MultiConverter_Init (&DAC_Sync_Config, 0, 0);
    DAC_Sync_Config.Tiles = tiles;	/* Sync DAC tiles as defined by bits set */
    DAC_Sync_Config.Target_Latency = lat_test;
    xil_printf("DAC_Sync_Config.Tiles: 0x%08x\r\n", DAC_Sync_Config.Tiles);
	xil_printf("\n=== Multi-Tile Synchronization Metal Log Report ===\r\n");
    status = XRFdc_MultiConverter_Sync(RFdcInstPtr, XRFDC_DAC_TILE, &DAC_Sync_Config);
    if(status != XRFDC_MTS_OK) {
    	xil_printf("XRFdc_MultiConverter_Sync() FAILED\r\n");
    	return;
    }

	if(status == XRFDC_MTS_OK) {
	   	xil_printf("\n========== DAC Multi-Tile Sync Report ==========\r\n");
		for(i=0; i<4; i++) {
			if((1<<i)&DAC_Sync_Config.Tiles) {
				XRFdc_GetInterpolationFactor(RFdcInstPtr, i, 0, &factor);
	               xil_printf("DAC%d: Latency(T1) = %3d, Adjusted Delay "
					 "Offset(T%d) = %3d, Marker Delay = %d \r\n", i, DAC_Sync_Config.Latency[i],
							 factor, DAC_Sync_Config.Offset[i],DAC_Sync_Config.Marker_Delay);
	                xil_printf("=== MTS DAC Tile%d PLL Report ===\r\n",i);
	                xil_printf("    DAC%d: PLL DTC Code =%d \n\r", i, DAC_Sync_Config.DTC_Set_PLL.DTC_Code[i]);
	                xil_printf("    DAC%d: PLL Num Windows =%d \n\r", i, DAC_Sync_Config.DTC_Set_PLL.Num_Windows[i]);
	                xil_printf("    DAC%d: PLL Max Gap =%d \n\r", i, DAC_Sync_Config.DTC_Set_PLL.Max_Gap[i]);
	                xil_printf("    DAC%d: PLL Min Gap =%d \n\r", i, DAC_Sync_Config.DTC_Set_PLL.Min_Gap[i]);
	                xil_printf("    DAC%d: PLL Max Overlap =%d \n\r", i, DAC_Sync_Config.DTC_Set_PLL.Max_Overlap[i]);
	                xil_printf("=== MTS DAC Tile%d T1 Report ===\r\n",i);
	                xil_printf("    DAC%d: T1 DTC Code =%d \n\r", i, DAC_Sync_Config.DTC_Set_T1.DTC_Code[i]);
	                xil_printf("    DAC%d: T1 Num Windows =%d \n\r", i, DAC_Sync_Config.DTC_Set_T1.Num_Windows[i]);
	                xil_printf("    DAC%d: T1 Max Gap =%d \n\r", i, DAC_Sync_Config.DTC_Set_T1.Max_Gap[i]);
	                xil_printf("    DAC%d: T1 Min Gap =%d \n\r", i, DAC_Sync_Config.DTC_Set_T1.Min_Gap[i]);
	                xil_printf("    DAC%d: T1 Max Overlap =%d \n\n\r", i, DAC_Sync_Config.DTC_Set_T1.Max_Overlap[i]);
			}
		}
		xil_printf("DAC Multi-Tile Synchronization with fixed latency is complete.");
		xil_printf("\r\n###############################################\r\n");


	}
    return;
}


//******************************************************************************/
void adcMTSwl(u32 *cmdVals)
{
	u32 tiles;
	int i;
	int status;
	u32 factor;
	int lat_test;
	XRFdc* RFdcInstPtr = &RFdcInst;

	tiles = cmdVals[0];

	xil_printf("\r\n###############################################\r\n");
	xil_printf("Enabling ADC Multi-Tile Synchronization with fixed latency...\n\r\n");

	xil_printf("Set latency to : ");
	lat_test = mygetline();
	xil_printf("\r\n    Latency set to : %d\r\n",lat_test);

	// Set MTS
	XRFdc_MultiConverter_Init (&ADC_Sync_Config, 0, 0);
	ADC_Sync_Config.Tiles = tiles;	/* Sync DAC tiles as defined by bits set */
	ADC_Sync_Config.Target_Latency = lat_test;
	xil_printf("ADC_Sync_Config.Tiles: 0x%08x\r\n", ADC_Sync_Config.Tiles);
	xil_printf("\n=== Multi-Tile Synchronization Metal Log Report ===\r\n");
	status = XRFdc_MultiConverter_Sync(RFdcInstPtr, XRFDC_ADC_TILE, &ADC_Sync_Config);
	if(status != XRFDC_MTS_OK) {
		xil_printf("XRFdc_MultiConverter_Sync() FAILED\r\n");
		return;
	}

	if(status == XRFDC_MTS_OK) {
	   	xil_printf("\n========== ADC Multi-Tile Sync Report ==========\r\n");
		for(i=0; i<4; i++) {
			if((1<<i)&ADC_Sync_Config.Tiles) {
				XRFdc_GetDecimationFactor(RFdcInstPtr, i, 0, &factor);
	               xil_printf("ADC%d: Latency(T1) = %3d, Adjusted Delay "
					 "Offset(T%d) = %3d, Marker Delay = %d \r\n", i, ADC_Sync_Config.Latency[i],
							 factor, ADC_Sync_Config.Offset[i],ADC_Sync_Config.Marker_Delay);
	                xil_printf("=== MTS ADC Tile%d PLL Report ===\r\n",i);
	                xil_printf("    ADC%d: PLL DTC Code =%d \n\r", i, ADC_Sync_Config.DTC_Set_PLL.DTC_Code[i]);
	                xil_printf("    ADC%d: PLL Num Windows =%d \n\r", i, ADC_Sync_Config.DTC_Set_PLL.Num_Windows[i]);
	                xil_printf("    ADC%d: PLL Max Gap =%d \n\r", i, ADC_Sync_Config.DTC_Set_PLL.Max_Gap[i]);
	                xil_printf("    ADC%d: PLL Min Gap =%d \n\r", i, ADC_Sync_Config.DTC_Set_PLL.Min_Gap[i]);
	                xil_printf("    ADC%d: PLL Max Overlap =%d \n\r", i, ADC_Sync_Config.DTC_Set_PLL.Max_Overlap[i]);
	                xil_printf("=== MTS ADC Tile%d T1 Report ===\r\n",i);
	                xil_printf("    ADC%d: T1 DTC Code =%d \n\r", i, ADC_Sync_Config.DTC_Set_T1.DTC_Code[i]);
	                xil_printf("    ADC%d: T1 Num Windows =%d \n\r", i, ADC_Sync_Config.DTC_Set_T1.Num_Windows[i]);
	                xil_printf("    ADC%d: T1 Max Gap =%d \n\r", i, ADC_Sync_Config.DTC_Set_T1.Max_Gap[i]);
	                xil_printf("    ADC%d: T1 Min Gap =%d \n\r", i, ADC_Sync_Config.DTC_Set_T1.Min_Gap[i]);
	                xil_printf("    ADC%d: T1 Max Overlap =%d \n\n\r", i, ADC_Sync_Config.DTC_Set_T1.Max_Overlap[i]);
			}
		}
		xil_printf("ADC Multi-Tile Synchronization with fixed latency is complete.");
		xil_printf("\r\n###############################################\r\n");


	}


    return;
}



/*****************************************************************************/
/**
*
* Adc Sync Set
*
* @param	cmdVals[0] bit pattern for ADC tiles to sync. I.E ADC tiles 0 and 1 = 0x3
*           cmdVals[1] 0=event src tile, 1=pl event
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcMTS(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 tiles;
	int status;
	int i;
	u32 factor;

	tiles = cmdVals[0];

	xil_printf("\r\n###############################################\r\n");
	xil_printf("Enabling ADC Multi-Tile Synchronization...\n\r\n");

	// Set MTS
	XRFdc_MultiConverter_Init (&ADC_Sync_Config, 0, 0);
	ADC_Sync_Config.Tiles = tiles;	/* Sync ADC tiles as defined by bits set */
	xil_printf("ADC_Sync_Config.Tiles: 0x%08x\r\n", ADC_Sync_Config.Tiles);
	xil_printf("\n=== Multi-Tile Synchronization Metal Log Report ===\r\n");
	status = XRFdc_MultiConverter_Sync(RFdcInstPtr, XRFDC_ADC_TILE, &ADC_Sync_Config);
	if(status != XRFDC_MTS_OK) {
		xil_printf("XRFdc_MultiConverter_Sync() FAILED with error 0x%08x\r\n", status);
	}


	if(status == XRFDC_MTS_OK) {
	   	xil_printf("\n========== ADC Multi-Tile Sync Report ==========\r\n");
		for(i=0; i<4; i++) {
			if((1<<i)&ADC_Sync_Config.Tiles) {
				XRFdc_GetDecimationFactor(RFdcInstPtr, i, 0, &factor);
	               xil_printf("ADC%d: Latency(T1) = %3d, Adjusted Delay "
					 "Offset(T%d) = %3d, Marker Delay = %d \r\n", i, ADC_Sync_Config.Latency[i],
							 factor, ADC_Sync_Config.Offset[i],ADC_Sync_Config.Marker_Delay);
	                xil_printf("=== MTS ADC Tile%d PLL Report ===\r\n",i);
	                xil_printf("    ADC%d: PLL DTC Code =%d \n\r", i, ADC_Sync_Config.DTC_Set_PLL.DTC_Code[i]);
	                xil_printf("    ADC%d: PLL Num Windows =%d \n\r", i, ADC_Sync_Config.DTC_Set_PLL.Num_Windows[i]);
	                xil_printf("    ADC%d: PLL Max Gap =%d \n\r", i, ADC_Sync_Config.DTC_Set_PLL.Max_Gap[i]);
	                xil_printf("    ADC%d: PLL Min Gap =%d \n\r", i, ADC_Sync_Config.DTC_Set_PLL.Min_Gap[i]);
	                xil_printf("    ADC%d: PLL Max Overlap =%d \n\r", i, ADC_Sync_Config.DTC_Set_PLL.Max_Overlap[i]);
	                xil_printf("=== MTS ADC Tile%d T1 Report ===\r\n",i);
	                xil_printf("    ADC%d: T1 DTC Code =%d \n\r", i, ADC_Sync_Config.DTC_Set_T1.DTC_Code[i]);
	                xil_printf("    ADC%d: T1 Num Windows =%d \n\r", i, ADC_Sync_Config.DTC_Set_T1.Num_Windows[i]);
	                xil_printf("    ADC%d: T1 Max Gap =%d \n\r", i, ADC_Sync_Config.DTC_Set_T1.Max_Gap[i]);
	                xil_printf("    ADC%d: T1 Min Gap =%d \n\r", i, ADC_Sync_Config.DTC_Set_T1.Min_Gap[i]);
	                xil_printf("    ADC%d: T1 Max Overlap =%d \n\n\r", i, ADC_Sync_Config.DTC_Set_T1.Max_Overlap[i]);
			}
		}
		xil_printf("ADC Multi-Tile Synchronization is complete.");
		xil_printf("\r\n###############################################\r\n");

	}

    return;
}

/*****************************************************************************/
/**
*
* Dac and ADC Sync Dump
*
* @param	None
*
* @return	None
*
* @note		Report Overall Latency in T1 (Sample Clocks) and
*           Offsets (in terms of PL words) added to each FIFO
*
******************************************************************************/
void dacAdcMTSStatus(u32 *cmdVals)
{
	int i;
	u32 factor;
	XRFdc* RFdcInstPtr = &RFdcInst;
	xil_printf("\r\n###############################################\r\n");
	xil_printf("=== Multi-Tile Sync Report ===\r\n");
	for(i=0; i<4; i++) {
		if((1<<i)&DAC_Sync_Config.Tiles) {
				XRFdc_GetInterpolationFactor(RFdcInstPtr, i, 0, &factor);
				xil_printf("DAC%d: Latency(T1) =%3d, Adjusted Delay"
				 "Offset(T%d) =%3d\r\n", i, DAC_Sync_Config.Latency[i],
						 factor, DAC_Sync_Config.Offset[i]);
		}
	}

	for(i=0; i<4; i++) {
		if((1<<i)&ADC_Sync_Config.Tiles) {
			XRFdc_GetDecimationFactor(RFdcInstPtr, i, 0, &factor);
			xil_printf("ADC%d: Latency(T1) =%3d, Adjusted Delay"
			 "Offset(T%d) =%3d\r\n", i, ADC_Sync_Config.Latency[i],
					 factor, ADC_Sync_Config.Offset[i]);
		}
	}
	xil_printf("###############################################\r\n\n");
	return;
}





