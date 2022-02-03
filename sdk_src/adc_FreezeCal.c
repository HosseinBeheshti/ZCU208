/*
 * adcFreezeCal.c
 *
 *  Created on: 06/23/2020
 *      Author: jlantz
 */
/***************************** Include Files *********************************/
#include "xil_io.h"
#include "xil_types.h"
#include <stdio.h>
#include "xparameters.h"
#include "main.h"
#include "xrfdc.h"
#include "adc_FreezeCal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/

/*****************************************************************************/
/**
*
* adcFreezeCalStatus TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcFreezeCalStatus(u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc *RFdcInstPtr = &RFdcInst;
	XRFdc_Cal_Freeze_Settings calFreezeSettings;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];

	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\n\r");
	xil_printf("=== ADC Cal Freeze Report ===\n\r");

	for (Tile_Id = 0; Tile_Id <= 3; Tile_Id++)
	{
		for (Block_Id = 0; Block_Id <= 3; Block_Id++)
		{
			if (XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id))
			{
				XRFdc_GetCalFreeze(RFdcInstPtr, Tile_Id, Block_Id, &calFreezeSettings);
				if (calFreezeSettings.CalFrozen == 0)
				{
					xil_printf("   ADC Tile%d ch%d Cal Frozen: %d (Not Frozen)\r\n", Tile_Id, Block_Id, calFreezeSettings.CalFrozen);
				}
				if (calFreezeSettings.CalFrozen == 1)
				{
					xil_printf("   ADC Tile%d ch%d Cal Frozen: %d (Frozen)\r\n", Tile_Id, Block_Id, calFreezeSettings.CalFrozen);
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
* Set Freeze Background Calibration TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcSetFreezeCal(u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc *RFdcInstPtr = &RFdcInst;
	XRFdc_Cal_Freeze_Settings calFreezeSettings;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];

	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\n\r");
	xil_printf("=== ADC Cal Freeze Report ===\n\r");

	if ((XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id) == 1) &&
		(Tile_Id < NUM_TILES) && (Block_Id < NUM_BLOCKS))
	{
		XRFdc_GetCalFreeze(RFdcInstPtr, Tile_Id, Block_Id, &calFreezeSettings);
		xil_printf("   ADC Tile%d ch%d Cal Freeze Status before command: %d\r\n", Tile_Id, Block_Id, calFreezeSettings.CalFrozen);
		calFreezeSettings.DisableFreezePin = 1;
		calFreezeSettings.FreezeCalibration = 1;
		XRFdc_SetCalFreeze(RFdcInstPtr, Tile_Id, Block_Id, &calFreezeSettings);
		XRFdc_GetCalFreeze(RFdcInstPtr, Tile_Id, Block_Id, &calFreezeSettings);
		sleep(1);
		XRFdc_GetCalFreeze(RFdcInstPtr, Tile_Id, Block_Id, &calFreezeSettings);
		xil_printf("   ADC Tile%d ch%d Cal Freeze Status after command: %d\r\n", Tile_Id, Block_Id, calFreezeSettings.CalFrozen);
	}
	else
	{
		xil_printf("ADC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
	}

	xil_printf("###############################################\r\n\n");

	return;
}

/*****************************************************************************/
/**
*
* Unset ADC Freeze Background Calibration TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcSetUnFreezeCal(u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc *RFdcInstPtr = &RFdcInst;
	XRFdc_Cal_Freeze_Settings calFreezeSettings;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];

	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\n\r");
	xil_printf("=== ADC Cal Freeze Report ===\n\r");

	if ((XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id) == 1) &&
		(Tile_Id < NUM_TILES) && (Block_Id < NUM_BLOCKS))
	{
		XRFdc_GetCalFreeze(RFdcInstPtr, Tile_Id, Block_Id, &calFreezeSettings);
		xil_printf("   ADC Tile%d ch%d Cal Freeze Status before command: %d\r\n", Tile_Id, Block_Id, calFreezeSettings.CalFrozen);
		calFreezeSettings.DisableFreezePin = 1;
		calFreezeSettings.FreezeCalibration = 0;
		XRFdc_SetCalFreeze(RFdcInstPtr, Tile_Id, Block_Id, &calFreezeSettings);
		XRFdc_GetCalFreeze(RFdcInstPtr, Tile_Id, Block_Id, &calFreezeSettings);
		sleep(1);
		XRFdc_GetCalFreeze(RFdcInstPtr, Tile_Id, Block_Id, &calFreezeSettings);
		xil_printf("   ADC Tile%d ch%d Cal Freeze Status after command: %d\r\n", Tile_Id, Block_Id, calFreezeSettings.CalFrozen);
	}
	else
	{
		xil_printf("ADC Tile%d ch%d is not available.\n\r", Tile_Id, Block_Id);
	}

	xil_printf("###############################################\r\n\n");

	return;
}
