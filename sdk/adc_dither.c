// in the main.h file add #include "adc_dither.h"
//  In this section of the main.c file add...
	// Initialize CLI commands structure
	//	cli_adc_dither_init();


/***************************** Include Files *********************************/
#include "xil_io.h"
#include "xil_types.h"
#include <stdio.h>
#include "cli.h"
#include "xparameters.h"
#include "main.h"
#include "xrfdc.h"
#include "adc_dither.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/


/*****************************************************************************/
/**
*
* cli_adc_dither_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_adc_dither_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		{"##################### ADC Dither #######################" , " "    , 0, *cmdComment   },
		{"adcGetDither"    , "- Display Dither Setting for all tiles"        , 0, *adcGetDither},
		{"adcSetDither"    , "- <tile> <adc> <dither> 0-off, 1-on"           , 3, *adcSetDither},
		{" "                       , " "                                     , 0, *cmdComment   },
	};
	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}


/*****************************************************************************/
/**
*
* adcGetDither TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcGetDither(u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 getDitherSettings;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];

	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\n\r");
	xil_printf("=== ADC Get Dither ===\n\r");

	for (Tile_Id=0; Tile_Id<=3; Tile_Id++) {
		for(Block_Id=0; Block_Id<=3; Block_Id++) {
			if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
				XRFdc_GetDither(RFdcInstPtr, Tile_Id, Block_Id, &getDitherSettings);
				if (getDitherSettings == 1){
					xil_printf("   ADC Tile%d ch%d Dither: %d (Dither Enabled)\r\n", Tile_Id, Block_Id,getDitherSettings);
				}
				if (getDitherSettings == 0){
					xil_printf("   ADC Tile%d ch%d Dither: %d (Dither Disabled)\r\n", Tile_Id, Block_Id,getDitherSettings);
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
* adcSetDither TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcSetDither(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	int Status;
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc_BlockStatus blockStatus;
	u32 GetDither;
	u32 Dither;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];
	Dither = cmdVals[2];

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\r\n");
	if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
		xil_printf("Set ADC Dither...\n\r");
		XRFdc_GetBlockStatus(RFdcInstPtr,XRFDC_ADC_TILE, Tile_Id, Block_Id, &blockStatus);
		//////////////////////////////////////////////////////////////////////////////
		// ADC Set Dither
		Status = XRFdc_SetDither(RFdcInstPtr, Tile_Id, Block_Id, Dither);
		if (Status != XST_SUCCESS) {
			xil_printf("XRFdc_SetDither() for ADC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
			return;
		}

		Status = XRFdc_GetDither(RFdcInstPtr, Tile_Id, Block_Id, &GetDither);
		if (Status != XST_SUCCESS) {
			xil_printf("XRFdc_GetDither() for DAC Tile%d ch%d failed.\n\r",Tile_Id,Block_Id);
			return;
		}else{
			if (GetDither == 1){
				xil_printf("   ADC Tile%d ch%d Dither: %d (Dither Enabled)\r\n", Tile_Id, Block_Id,GetDither);
			}
			if (GetDither == 0){
				xil_printf("   ADC Tile%d ch%d Dither: %d (Dither Disabled)\r\n", Tile_Id, Block_Id,GetDither);
			}
		}

	} else{
		xil_printf("ADC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
	}
	xil_printf("###############################################\r\n\n");

	return;
}

