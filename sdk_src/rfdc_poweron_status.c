/*
 * rfdc_poweron_status.c
 *
 *  Created on: Aug 9, 2019
 *      Author: jlantz
 */

/***************************** Include Files *********************************/
#include "xil_io.h"
#include "xil_types.h"
#include <stdio.h>
#include "xparameters.h"
#include "main.h"
#include "xrfdc.h"
#include "rfdc_poweron_status.h"
#include "cli.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/



/*****************************************************************************/
/**
*
* cli_cmd_func_mem_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_rfdc_poweron_status_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		//000000000011111111112222    000000000011111111112222222222333333333
		//012345678901234567890123    012345678901234567890123456789012345678
    	{"################### Power-On Status ####################" , " "     , 0, *cmdComment   },
		{"rfdcPoweronStatus"       , "Display Power Status"                  , 0, *rfdcPoweronStatus},
		{" "                       , " "                                       , 0, *cmdComment   },
	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}


/*****************************************************************************/
/**
*
* Power on status of the DAC's and ADC's
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void rfdcPoweronStatus (u32 *cmdVals) {
	u32 Tile_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 val;
    XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

	xil_printf("\r\n###############################################\r\n");
	xil_printf("The Power-on sequence step. 0xF is complete.\r\n");

	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
		if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {
  //  		val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_DAC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_DAC_DEBUG_RST_OFFSET);
 //   		if(val & XRFDC_DBG_RST_CAL_MASK) {
   // 			xil_printf("  DAC Tile: %d NOT enabled.\r\n", Tile_Id);
  //  		} else {
    		    xil_printf("   DAC Tile%d Power-on Sequence Step: 0x%08x\r\n",Tile_Id,
    		    	Xil_In32(RFDC_BASE + 0x0000C + 0x04000 + Tile_Id * 0x4000));
   // 		}
    	}
	}

	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
    	if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1) {
    		val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_ADC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_ADC_DEBUG_RST_OFFSET);
    		if(val & XRFDC_DBG_RST_CAL_MASK) {
    			xil_printf("  ADC Tile%d NOT ready.\r\n", Tile_Id);
    		} else {
    		    xil_printf("   ADC Tile%d Power-on Sequence Step: 0x%08x\r\n",Tile_Id,
    		    		Xil_In32(RFDC_BASE + 0x0000C + 0x14000 + Tile_Id * 0x4000));
    		}
    	}
	}


	xil_printf("###############################################\r\n\n");


	return;
}


