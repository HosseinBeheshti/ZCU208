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
#include "adcLoadCalCoefficients.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
void storedCoeffRead(XRFdc_Calibration_Coefficients *destCoeff, perAdcCoeffType *srcCoeffPtr, int coeff_set);
void coeffValidationReport(XRFdc_Calibration_Coefficients *calCoeff, perAdcCoeffType *storedCoeffPtr, int set, char *coeffSetName);


/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/


/*****************************************************************************/
/**
*
* cli_adcLoadCalCoeff_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_adcLoadCalCoeff_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		{"adcLoadBGCalCoeff"  , "- <tile> <adc> Load BG Cal Coeff for ch"    , 2, *adcLoadBGCalCoeff},
		{"adcLoadFGCalCoeff"  , "- <tile> <adc> Load FG Cal Coeff for ch"    , 2, *adcLoadFGCalCoeff},
	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
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
void adcLoadBGCalCoeff (u32 *cmdVals)
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
	xil_printf("=== ADC Load Coefficients Report ===\n\r");

	if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id) == 1) {

		///////////////////////////////////////////////////////////////
		// Process GCB coefficients

		// Read GCB cal coefficients from stored values
		xil_printf("   Loading Coeff Set: %d  from BaseAddr: 0x%08x Size: %04x\n\r", selectedCoeffSet, storedCoeffSets[selectedCoeffSet], sizeof(storedCoeffSets[selectedCoeffSet]));
		storedCoeffRead(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_GCB);

		// Write GCB coeff to ADC
		XRFdc_SetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id, XRFDC_CAL_BLOCK_GCB, &calCoeff);
		xil_printf("   Set GCB Coefficients complete for Tile%d ch%d. \r\n",Tile_Id,Block_Id);

		// reread coeff from ADC
		XRFdc_GetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id, XRFDC_CAL_BLOCK_GCB, &calCoeff);

		// Print coeff validation report for GCB
		xil_printf("=== Set Coefficients Validation Report ===. \r\n");
		coeffValidationReport(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_GCB, "GCB");


		///////////////////////////////////////////////////////////////
		// Process TSCB coefficients

		// Read TSCB cal coefficients from stored values
		storedCoeffRead(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_TSCB);

		// Write TSCB coeff to ADC
		XRFdc_SetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id, XRFDC_CAL_BLOCK_TSCB, &calCoeff);
		xil_printf("   Set TSCB Coefficients complete for Tile%d ch%d. \r\n",Tile_Id,Block_Id);

		// reread coeff from ADC
		XRFdc_GetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id, XRFDC_CAL_BLOCK_TSCB, &calCoeff);

		// Print coeff validation report for TSCB
		xil_printf("=== Set Coefficients Validation Report ===. \r\n");
		coeffValidationReport(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_TSCB, "TSCB");
		storedCoeffRead(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_OCB1);

		///////////////////////////////////////////////////////////////
		// Process OCB1 coefficients
		// Write OBC1 coeff to ADC
		XRFdc_SetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id, XRFDC_CAL_BLOCK_OCB1, &calCoeff);
		xil_printf("   Set OCB1 Coefficients complete for Tile%d ch%d. \r\n",Tile_Id,Block_Id);

		// reread coeff from ADC
		XRFdc_GetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id, XRFDC_CAL_BLOCK_OCB1, &calCoeff);

		// Print coeff validation report for OCB1
		xil_printf("=== Set Coefficients Validation Report ===. \r\n");
		coeffValidationReport(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_OCB1, "OCB1");


	} else {
		(xil_printf("Tile%d Ch%d is not available.\n\r",Tile_Id,Block_Id ));
	}
	xil_printf("###############################################\r\n\n");

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
void adcLoadFGCalCoeff (u32 *cmdVals)
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
	xil_printf("=== ADC Load FG Coefficients Report ===\n\r");

	if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id) == 1) {

		///////////////////////////////////////////////////////////////
		// Process OCB1 coefficients

		// Read OCB1 cal coefficients from stored values
		xil_printf("   Loading Coeff Set: %d  from BaseAddr: 0x%08x Size: %04x\n\r", selectedCoeffSet, storedCoeffSets[selectedCoeffSet], sizeof(storedCoeffSets[selectedCoeffSet]));


		///////////////////////////////////////////////////////////////
		// Process OCB2 coefficients

		// Read OCB2 cal coefficients from stored values
		storedCoeffRead(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_OCB2);

		// Write OCB2 coeff to ADC
		XRFdc_SetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id, XRFDC_CAL_BLOCK_OCB2, &calCoeff);
		xil_printf("   Set OCB2 Coefficients complete for Tile%d ch%d. \r\n",Tile_Id,Block_Id);

		// reread coeff from ADC
		XRFdc_GetCalCoefficients(RFdcInstPtr, Tile_Id, Block_Id, XRFDC_CAL_BLOCK_OCB2, &calCoeff);

		// Print coeff validation report for OCB2
		xil_printf("=== Set Coefficients Validation Report ===. \r\n");
		coeffValidationReport(&calCoeff, &storedCoeffSets[selectedCoeffSet][Tile_Id][Block_Id], COEFF_OCB2, "OCB2");

	} else {
		(xil_printf("Tile%d Ch%d is not available.\n\r",Tile_Id,Block_Id ));
	}
	xil_printf("###############################################\r\n\n");

	return;
}


void coeffValidationReport(XRFdc_Calibration_Coefficients *calCoeff, perAdcCoeffType *storedCoeffPtr, int set, char *coeffSetName)
{

	xil_printf("    %4s Coeff        Saved        Loaded          Diff\r\n", coeffSetName);
	xil_printf("    %4s Coeff0 %11d : %11d : %11d\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[0], calCoeff->Coeff0, calCoeff->Coeff0 - storedCoeffPtr->type[set].Coeff[0] );
	xil_printf("    %4s Coeff1 %11d : %11d : %11d\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[1], calCoeff->Coeff1, calCoeff->Coeff1 - storedCoeffPtr->type[set].Coeff[1] );
	xil_printf("    %4s Coeff2 %11d : %11d : %11d\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[2], calCoeff->Coeff2, calCoeff->Coeff2 - storedCoeffPtr->type[set].Coeff[2] );
	xil_printf("    %4s Coeff3 %11d : %11d : %11d\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[3], calCoeff->Coeff3, calCoeff->Coeff3 - storedCoeffPtr->type[set].Coeff[3] );
	xil_printf("    %4s Coeff4 %11d : %11d : %11d\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[4], calCoeff->Coeff4, calCoeff->Coeff4 - storedCoeffPtr->type[set].Coeff[4] );
	xil_printf("    %4s Coeff5 %11d : %11d : %11d\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[5], calCoeff->Coeff5, calCoeff->Coeff5 - storedCoeffPtr->type[set].Coeff[5] );
	xil_printf("    %4s Coeff6 %11d : %11d : %11d\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[6], calCoeff->Coeff6, calCoeff->Coeff6 - storedCoeffPtr->type[set].Coeff[6] );
	xil_printf("    %4s Coeff7 %11d : %11d : %11d\r\n\n", coeffSetName, storedCoeffPtr->type[set].Coeff[7], calCoeff->Coeff7, calCoeff->Coeff7 - storedCoeffPtr->type[set].Coeff[7] );

	xil_printf("    %4s Coeff        Saved        Loaded          Diff\r\n", coeffSetName);
	xil_printf("    %4s Coeff0 0x%08x : 0x%08x : 0x%08x\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[0], calCoeff->Coeff0, calCoeff->Coeff0 - storedCoeffPtr->type[set].Coeff[0] );
	xil_printf("    %4s Coeff1 0x%08x : 0x%08x : 0x%08x\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[1], calCoeff->Coeff1, calCoeff->Coeff1 - storedCoeffPtr->type[set].Coeff[1] );
	xil_printf("    %4s Coeff2 0x%08x : 0x%08x : 0x%08x\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[2], calCoeff->Coeff2, calCoeff->Coeff2 - storedCoeffPtr->type[set].Coeff[2] );
	xil_printf("    %4s Coeff3 0x%08x : 0x%08x : 0x%08x\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[3], calCoeff->Coeff3, calCoeff->Coeff3 - storedCoeffPtr->type[set].Coeff[3] );
	xil_printf("    %4s Coeff4 0x%08x : 0x%08x : 0x%08x\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[4], calCoeff->Coeff4, calCoeff->Coeff4 - storedCoeffPtr->type[set].Coeff[4] );
	xil_printf("    %4s Coeff5 0x%08x : 0x%08x : 0x%08x\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[5], calCoeff->Coeff5, calCoeff->Coeff5 - storedCoeffPtr->type[set].Coeff[5] );
	xil_printf("    %4s Coeff6 0x%08x : 0x%08x : 0x%08x\r\n",   coeffSetName, storedCoeffPtr->type[set].Coeff[6], calCoeff->Coeff6, calCoeff->Coeff6 - storedCoeffPtr->type[set].Coeff[6] );
	xil_printf("    %4s Coeff7 0x%08x : 0x%08x : 0x%08x\r\n\n", coeffSetName, storedCoeffPtr->type[set].Coeff[7], calCoeff->Coeff7, calCoeff->Coeff7 - storedCoeffPtr->type[set].Coeff[7] );


	if(storedCoeffPtr->type[set].written != 1 ) {
		xil_printf("    NOTE: Coeff was not stored using adcSaveCalCoeff()\r\n\n");
	}
	return;
}

void storedCoeffRead(XRFdc_Calibration_Coefficients *destCoeff, perAdcCoeffType *srcCoeffPtr, int coeff_set)
{
	destCoeff->Coeff0 = srcCoeffPtr->type[coeff_set].Coeff[0];
	destCoeff->Coeff1 = srcCoeffPtr->type[coeff_set].Coeff[1];
	destCoeff->Coeff2 = srcCoeffPtr->type[coeff_set].Coeff[2];
	destCoeff->Coeff3 = srcCoeffPtr->type[coeff_set].Coeff[3];
	destCoeff->Coeff4 = srcCoeffPtr->type[coeff_set].Coeff[4];
	destCoeff->Coeff5 = srcCoeffPtr->type[coeff_set].Coeff[5];
	destCoeff->Coeff6 = srcCoeffPtr->type[coeff_set].Coeff[6];
	destCoeff->Coeff7 = srcCoeffPtr->type[coeff_set].Coeff[7];
	return;
}


