// add these lines to the appropriate location in the cmd_globals.c file
// #include "rfdc_nyquistzone.h"
// {"rfdcNyquistZone"    , "- Display Nyquist Zone for all tiles"    , 0, *rfdcNyquistZone},


/***************************** Include Files *********************************/
#include "xil_io.h"
#include "xil_types.h"
#include <stdio.h>
#include "cli.h"
#include "xparameters.h"
#include "main.h"
#include "xrfdc.h"
#include "rfdc_nyquistzone.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/


/*****************************************************************************/
/**
*
* cli_rfdc_nyquistzone_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_rfdcNyquistzone_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		//000000000011111111112222    000000000011111111112222222222333333333
		//012345678901234567890123    012345678901234567890123456789012345678
		{"#################### Nyquist Zone ######################" , " "     , 0, *cmdComment   },
		{"rfdcNyquistZone"    , "- Display Nyquist Zone for all tiles"        , 0, *rfdcNyquistZone},
		{"dacSetNyquistZone"  , "- <tile> <dac> <zone> Set Nyquist Zone"      , 3, *dacSetNyquistZone},
		{"adcSetNyquistZone"  , "- <tile> <adc> <zone> Set Nyquist Zone"      , 3, *adcSetNyquistZone},
		{" "                       , " "                                      , 0, *cmdComment   },

	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}


/*****************************************************************************/
/**
*
* rfdcNyquistZone TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void rfdcNyquistZone(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	int Status;
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc_BlockStatus blockStatus;
	u32 GetNyquistZone;



	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\r\n");
	xil_printf("=== DAC Nyquist Zone Report ===\n\r");



	for (Tile_Id=0; Tile_Id<=3; Tile_Id++) {
		if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {
			for(Block_Id=0; Block_Id<=3; Block_Id++) {
				if(XRFdc_IsDACBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
					XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &blockStatus);

					//////////////////////////////////////////////////////////////////////////////
					// DAC Nyquist Zone
					Status = XRFdc_GetNyquistZone(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &GetNyquistZone);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetNyquistZone() for DAC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
						return;
					}

					XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &blockStatus);
					printf("   DAC Tile%d ch%d Nyquist Zone: %d",Tile_Id,Block_Id,GetNyquistZone);
					printf(" - Sampling Frequency: %f GHz.\n\r", blockStatus.SamplingFreq);
				}
			}
		}
	}


	xil_printf("=== ADC Nyquist Zone Report ===\n\r");


	for (Tile_Id=0; Tile_Id<=3; Tile_Id++) {
		if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1) {

			for(Block_Id=0; Block_Id<=3; Block_Id++) {
				if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
					XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &blockStatus);

					//////////////////////////////////////////////////////////////////////////////
					// DAC Nyquist Zone
					Status = XRFdc_GetNyquistZone(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &GetNyquistZone);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetNyquistZone() for ADC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
						return;
					}

					XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &blockStatus);
					printf("   ADC Tile%d ch%d Nyquist Zone: %d",Tile_Id,Block_Id,GetNyquistZone);
					printf(" - Sampling Frequency: %f GHz.\n\r", blockStatus.SamplingFreq);
				}
			}
		}
	}


	xil_printf("###############################################\r\n\n");

	return;
}


/*****************************************************************************/
/**
*
* dacSetNyquistZone TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacSetNyquistZone(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	int Status;
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc_BlockStatus blockStatus;
	u32 GetNyquistZone;
	u32 Nyquist_Zone;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];
	Nyquist_Zone = cmdVals[2];

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\r\n");
	if(XRFdc_IsDACBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
		xil_printf("Set DAC Nyquist zone...\n\r");
		XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &blockStatus);

		//////////////////////////////////////////////////////////////////////////////
		// DAC Nyquist Zone
		Status = XRFdc_SetNyquistZone(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, Nyquist_Zone);
		if (Status != XST_SUCCESS) {
			xil_printf("XRFdc_SetNyquistZone() for DAC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
			return;
		}

		Status = XRFdc_GetNyquistZone(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &GetNyquistZone);
		if (Status != XST_SUCCESS) {
			xil_printf("XRFdc_GetNyquistZone() for DAC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
			return;
		}

		XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &blockStatus);
		printf("   DAC Tile%d ch%d Nyquist Zone: %d",Tile_Id,Block_Id,GetNyquistZone);
		printf(" - Sampling Frequency: %f GHz.\n\r", blockStatus.SamplingFreq);
	} else{
		xil_printf("DAC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
	}

	xil_printf("###############################################\r\n\n");

	return;
}


/*****************************************************************************/
/**
*
* adcSetNyquistZone TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcSetNyquistZone(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	int Status;
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc_BlockStatus blockStatus;
	u32 GetNyquistZone;
	u32 Nyquist_Zone;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];
	Nyquist_Zone = cmdVals[2];

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\r\n");
	if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
		xil_printf("Set ADC Nyquist zone...\n\r");
		XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &blockStatus);
		//////////////////////////////////////////////////////////////////////////////
		// ADC Nyquist Zone
		Status = XRFdc_SetNyquistZone(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, Nyquist_Zone);
		if (Status != XST_SUCCESS) {
			xil_printf("XRFdc_SetNyquistZone() for ADC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
			return;
		}

		Status = XRFdc_GetNyquistZone(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &GetNyquistZone);
		if (Status != XST_SUCCESS) {
			xil_printf("XRFdc_GetNyquistZone() for DAC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
			return;
		}

		XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &blockStatus);
		printf("   ADC Tile%d ch%d Nyquist Zone: %d",Tile_Id,Block_Id,GetNyquistZone);
		printf(" - Sampling Frequency: %f GHz.\n\r", blockStatus.SamplingFreq);
	} else{
		xil_printf("ADC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
	}
	xil_printf("###############################################\r\n\n");

	return;
}

