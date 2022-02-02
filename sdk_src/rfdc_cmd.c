


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
#include "rfdc_cmd.h"


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

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

void cli_rfdc_cmd_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		//000000000011111111112222    000000000011111111112222222222333333333
		//012345678901234567890123    012345678901234567890123456789012345678
		{"#################### rfdc commands #####################" , " " , 0, *cmdComment   },
		{"rfdcReady"          , "- Display ready status for DAC/ADCs"     , 0, *rfdcReady},
		{"rfdcShutdown"       , "- Shut down the data converters"         , 0, *rfdcShutdown},
		{"rfdcStartup"        , "- Startup up the data converters"        , 0, *rfdcStartup},
		{"########### DAC and ADC Registers and Status ###########" , " " , 0, *cmdComment   },
		{"adcDumpRegs"        , "<tile> - Dump ADC registers for tile#"   , 1, *adcDumpRegs  },
		{"dacDumpRegs"        , "<tile> - Dump DAC registers for tile#"   , 1, *dacDumpRegs  },
		{"adcDumpStatus"      , "- Dump ADC status"                       , 0, *adcDumpStatus},
		{"dacDumpStatus"      , "- Dump DAC status"                       , 0, *dacDumpStatus},
		{"################## DAC and ADC Reset ###################" , " " , 0, *cmdComment   },
		{"dacReset"           , "<tile> - Reset DAC"                      , 1, *dacReset},
		{"adcReset"           , "<tile> - Reset ADC"                      , 1, *adcReset},
		{"dacResetAll"        , "- Reset all DAC's"                       , 0, *dacResetAll},
		{"adcResetAll"        , "- Reset all ADC's"                       , 0, *adcResetAll},
		{"################## DAC only commands ###################" , " " , 0, *cmdComment   },
		{"dacCurrent"         , "- Display DAC current for all tiles"     , 0, *dacCurrent},
//		{"############### DAC Gen3 only commands #################" , " " , 0, *cmdComment   },
//		{"dacSetVOP"         , "<tile> <dac> <setting> - Set VOP for Gen3", 3, *dacSetVOP},
//		{"############### ADC Gen3 only commands #################" , " " , 0, *cmdComment   },
//		{"adcSetDSA"         , "<tile> <adc> <setting> - Set DSA for Gen3", 3, *adcSetDSA},
//		{"adcGetDSA"         , "<tile> <adc> - Get DSA for Gen3"          , 2, *adcSetDSA},
		{" "                       , " "                                  , 0, *cmdComment   },

	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}



/*****************************************************************************/
/**
*
* Dump DAC status via API
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacDumpStatus(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	int Status;
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc_BlockStatus blockStatus;
	XRFdc_Mixer_Settings GetMixer_Settings;
	u32 InterpolationFactor;
	XRFdc_QMC_Settings GetQMCSettings;
	XRFdc_CoarseDelay_Settings GetCoarseDelaySettings;
	u32 GetDecoderMode;
	u32 GetNyquistZone;
	u32 GetFabricRate;

    // Calling this function gets the status of the IP
    XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

    for (Tile_Id=0; Tile_Id<=3; Tile_Id++) {
        xil_printf("=================================================\r\n");
    	if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {
			xil_printf("Tile: %d DAC Enabled\r\n", Tile_Id);
			xil_printf("  BlockStatus:  0x%x\r\n", ipStatus.DACTileStatus[Tile_Id].BlockStatusMask);
			xil_printf("  TileState:    0x%08x\r\n", ipStatus.DACTileStatus[Tile_Id].TileState);
			xil_printf("  PowerUpState: 0x%08x\r\n", ipStatus.DACTileStatus[Tile_Id].PowerUpState);
			xil_printf("  PLLState:     0x%08x\r\n", ipStatus.DACTileStatus[Tile_Id].PLLState);

			for(Block_Id=0; Block_Id<=3; Block_Id++) {
				if(XRFdc_IsDACBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
					xil_printf("  ***********************************\r\n");
					xil_printf("  Block: %d Enabled\r\n", Block_Id);

					//////////////////////////////////////////////////////////////////////////////
					// blockStatus
					XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &blockStatus);
					printf("    SamplingFreq:          %f\r\n", blockStatus.SamplingFreq);
					xil_printf("    DigitalDataPathStatus: %d\r\n", blockStatus.DigitalDataPathStatus);
					xil_printf("    AnalogDataPathStatus:  %d\r\n", blockStatus.AnalogDataPathStatus);
					xil_printf("    IsFIFOFlagsEnabled:    %d\r\n", blockStatus.IsFIFOFlagsEnabled);
					xil_printf("    IsFIFOFlagsAsserted:   %d\r\n", blockStatus.IsFIFOFlagsAsserted);
					xil_printf("    DataPathClocksStatus:  %d\r\n", blockStatus.DataPathClocksStatus);


					//////////////////////////////////////////////////////////////////////////////
					// DAC Interpolation factor
					Status = XRFdc_GetInterpolationFactor(RFdcInstPtr, Tile_Id, Block_Id, &InterpolationFactor);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetInterpolationFactor() failed\r\n");
						return;
					}
					xil_printf("    Interpolation Factor:  %d\r\n",InterpolationFactor);

					//////////////////////////////////////////////////////////////////////////////
					// DAC Fabric Rate
					Status = XRFdc_GetFabWrVldWords(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &GetFabricRate);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetFabWrVldWords() failed\r\n");
						return;
					}
					xil_printf("    Fabric Rate         :  %d\r\n",GetFabricRate);

					//////////////////////////////////////////////////////////////////////////////
					// DAC Decoder Mode
					Status = XRFdc_GetDecoderMode(RFdcInstPtr, Tile_Id, Block_Id, &GetDecoderMode);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetDecoderMode() failed\n\r");
						return;
					}
					xil_printf("    Decoder Mode        :  %d\r\n",GetDecoderMode);


					//////////////////////////////////////////////////////////////////////////////
					// DAC Nyquist Zone
					Status = XRFdc_GetNyquistZone(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &GetNyquistZone);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetNyquistZone() failed\n\r");
						return;
					}
					xil_printf("    Nyquist Zone        :  %d\r\n",GetNyquistZone);


					//////////////////////////////////////////////////////////////////////////////
					// MixerSettings
			        Status =  XRFdc_GetMixerSettings(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &GetMixer_Settings);
					if (Status != XST_SUCCESS) {
				      xil_printf("Getting Fine Mixer failed\r\n");
					  	return ;
					}

					xil_printf("    **********Mixer Settings*********\r\n");
			        printf("    FREQ:              %f\r\n", GetMixer_Settings.Freq);
			        printf("    PHASE OFFSET:      %f\r\n", GetMixer_Settings.PhaseOffset);
			        xil_printf("    EVENT SOURCE:      %d\r\n", GetMixer_Settings.EventSource);
			        xil_printf("    MIXER MODE:        %d: ", GetMixer_Settings.MixerMode);
			        switch(GetMixer_Settings.MixerMode) {
			        case XRFDC_MIXER_MODE_OFF: xil_printf("OFF");
			        	break;
			        case XRFDC_MIXER_MODE_C2C: xil_printf("C2C");
			        	break;
			        case XRFDC_MIXER_MODE_C2R: xil_printf("C2R");
			        	break;
			        case XRFDC_MIXER_MODE_R2C: xil_printf("R2C");
			        	break;
			        default: xil_printf("unknown");
			        }
			        xil_printf("\r\n");

			        xil_printf("    COARSE MIXER FREQ: %d\r\n", GetMixer_Settings.CoarseMixFreq);


					//////////////////////////////////////////////////////////////////////////////
					// QMC Settings
			        Status = XRFdc_GetQMCSettings(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &GetQMCSettings);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetQMCSettings() failed\n\r");
						return;
					}

					xil_printf("    **********QMC Settings***********\r\n");
			        printf("    GainCorrectionFactor:   %f\r\n", GetQMCSettings.GainCorrectionFactor);
			        printf("    PhaseCorrectionFactor:  %f\r\n", GetQMCSettings.PhaseCorrectionFactor);
			        xil_printf("    EnablePhase:            %d\r\n", GetQMCSettings.EnablePhase);
			        xil_printf("    EnableGain:             %d\r\n", GetQMCSettings.EnableGain);
			        xil_printf("    OffsetCorrectionFactor: %d\r\n", GetQMCSettings.OffsetCorrectionFactor);
			        xil_printf("    EventSource:            %d\r\n", GetQMCSettings.EventSource);


					//////////////////////////////////////////////////////////////////////////////
					// Coarse Delay Settings
					Status = XRFdc_GetCoarseDelaySettings(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id, Block_Id, &GetCoarseDelaySettings);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetCoarseDelaySettings() failed\n\r");
						return;
					}
					xil_printf("    ******Coarse Delay Settings******\r\n");
			        xil_printf("    CoarseDelay:            %d\r\n", GetCoarseDelaySettings.CoarseDelay);
			        xil_printf("    EventSource:            %d\r\n", GetCoarseDelaySettings.EventSource);


				} else {
					xil_printf("  ***********************************\r\n");
					xil_printf("  Block: %d Disabled\r\n", Block_Id);
				}
			}
    	} else {
			xil_printf("Tile: %d DAC Disabled\r\n", Tile_Id);
    	}
    }


    return;
}


/*****************************************************************************/
/**
*
* Dump DAC registers via API
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacDumpRegs (u32 *cmdVals) {
	u32 Tile_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;

	Tile_Id = cmdVals[0];

    // Calling this function gets the status of the IP
    XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

	xil_printf("\n\r###############################################\n\r");
    if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {
		xil_printf("Tile: %d DAC Enabled\r\n", Tile_Id);
		XRFdc_DumpRegs(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id);
    } else {
		xil_printf("Tile: %d DAC DISABLED\r\n", Tile_Id);
    }

	xil_printf("###############################################\r\n\n");

	return;
}


/*****************************************************************************/
/**
*
* Dump DAC registers via API
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcDumpRegs (u32 *cmdVals) {
	u32 Tile_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;

	Tile_Id = cmdVals[0];

	// Calling this function gets the status of the IP
    XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

	xil_printf("\n\r###############################################\n\r");
   if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1) {
		xil_printf("Tile: %d ADC Enabled\r\n", Tile_Id);
		XRFdc_DumpRegs(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id);
    } else {
		xil_printf("Tile: %d ADC DISABLED\r\n", Tile_Id);
    }

	xil_printf("###############################################\r\n\n");

	return;
}


/*****************************************************************************/
/**
*
* Dump DAC status via API
*
* @param	TBD
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void rfSetMixerFreqOptPrint(u32 type, u32 Tile_Id, u32 Block_Id, double newFreq, int printEnable)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	int Status;
	XRFdc_Mixer_Settings GetMixer_Settings;

	Status = XRFdc_GetMixerSettings(RFdcInstPtr, type, Tile_Id, Block_Id, &GetMixer_Settings);

	if(printEnable) {
		xil_printf("Old Mixer Settings for %s: Tile:%d Block:%d\r\n", (type==XRFDC_DAC_TILE) ? "DAC" : "ADC", Tile_Id, Block_Id);
		printf("    FREQ:              %f\r\n", GetMixer_Settings.Freq);
		printf("    PHASE OFFSET:      %f\r\n", GetMixer_Settings.PhaseOffset);
		xil_printf("    EVENT SOURCE:      %d\r\n", GetMixer_Settings.EventSource);
		xil_printf("    MIXER MODE:        %d: ", GetMixer_Settings.MixerMode);
		switch(GetMixer_Settings.MixerMode) {
			case XRFDC_MIXER_MODE_OFF: xil_printf("OFF");
				break;
			case XRFDC_MIXER_MODE_C2C: xil_printf("C2C");
				break;
			case XRFDC_MIXER_MODE_C2R: xil_printf("C2R");
				break;
			case XRFDC_MIXER_MODE_R2C: xil_printf("R2C");
				break;
			default: xil_printf("unknown");
        }
        xil_printf("\r\n");
		xil_printf("    COARSE MIXER FREQ: %d\r\n", GetMixer_Settings.CoarseMixFreq);
		xil_printf("\r\n");
	}

	// change mixer freq
	GetMixer_Settings.Freq = (double)newFreq;

	// Set the new NCO frequency
	Status = XRFdc_SetMixerSettings(RFdcInstPtr, type, Tile_Id, Block_Id, &GetMixer_Settings);
	if (Status != XST_SUCCESS) {
		xil_printf("SetMixerSettings failed\r\n");
		return;
	}

	// Update event to update all the required registers
	XRFdc_UpdateEvent(RFdcInstPtr, type, Tile_Id, Block_Id, GetMixer_Settings.EventSource);

	// Read back and display MixerSettings
	Status =  XRFdc_GetMixerSettings(RFdcInstPtr, type, Tile_Id, Block_Id, &GetMixer_Settings);
	if (Status != XST_SUCCESS) {
	   xil_printf("Getting Fine Mixer failed\r\n");
	   return ;
	}

	if(printEnable) {
		xil_printf("New Mixer Settings for %s: Tile:%d Block:%d\r\n", (type==XRFDC_DAC_TILE) ? "DAC" : "ADC", Tile_Id, Block_Id);
		printf("    FREQ:              %f\r\n", GetMixer_Settings.Freq);
		printf("    PHASE OFFSET:      %f\r\n", GetMixer_Settings.PhaseOffset);
		xil_printf("    EVENT SOURCE:      %d\r\n", GetMixer_Settings.EventSource);
        xil_printf("    MIXER MODE:        %d: ", GetMixer_Settings.MixerMode);
        switch(GetMixer_Settings.MixerMode) {
        case XRFDC_MIXER_MODE_OFF: xil_printf("OFF");
        	break;
        case XRFDC_MIXER_MODE_C2C: xil_printf("C2C");
        	break;
        case XRFDC_MIXER_MODE_C2R: xil_printf("C2R");
        	break;
        case XRFDC_MIXER_MODE_R2C: xil_printf("R2C");
        	break;
        default: xil_printf("unknown");
        }
        xil_printf("\r\n");
		xil_printf("    COARSE MIXER FREQ: %d\r\n", GetMixer_Settings.CoarseMixFreq);
	}


    return;
}

/****************************************************************************/
/**
*
* This function verifies the DAC has been configured for the correct AXIS width.
*
* @param	RFdcDeviceId is the XPAR_<XRFDC_instance>_DEVICE_ID value
*		from xparameters.h.
*
* @return   None
*
* @note   	None
*
****************************************************************************/
void RFdcCheckAxisWidth(XRFdc *RFdcInstPtr, u32 DacExpectedBits, u32 AdcExpectedBits)
{
	int Status;
	u16 Tile;
	u16 Block;
	u32 GetFabricRate;

	xil_printf("Verifying Fabric rate matches expected rate\r\n");

	for (Tile = 0; Tile <4; Tile++) {
		for (Block = 0; Block <4; Block++) {
			/* Check for DAC block Enable */
			if (XRFdc_IsDACBlockEnabled(RFdcInstPtr, Tile, Block)) {
				/* Get DAC fabric rate */
				Status = XRFdc_GetFabWrVldWords(RFdcInstPtr, XRFDC_DAC_TILE, Tile, Block, &GetFabricRate);
				if (Status != XST_SUCCESS) {
					xil_printf("Call to XRFdc_GetFabWrVldWords() failed\r\n");
					return;
				}
				if (GetFabricRate*16 != DacExpectedBits) {
					xil_printf("ERROR: Expected datawidth is %dbits wide but DAC tile configured as %d 16bit-words (%dbits)\r\n",
							DacExpectedBits, GetFabricRate, GetFabricRate*16);
				}
			}

			if (XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile, Block)) {
				/* Get ADC fabric rate */
				Status = XRFdc_GetFabRdVldWords(RFdcInstPtr, XRFDC_ADC_TILE, Tile, Block, &GetFabricRate);
				if (Status != XST_SUCCESS) {
					xil_printf("Call to XRFdc_GetFabRdVldWords() failed\r\n");
					return;
				}
				if (GetFabricRate*16 != AdcExpectedBits) {
					xil_printf("ERROR: Expected datawidth is %dbits wide but ADC tile configured as %d 16bit-words (%dbits)\r\n",
							AdcExpectedBits, GetFabricRate, GetFabricRate*16);
				}
			}
		}
	}
	return;
}


/*****************************************************************************/
/**
*
* Dump ADC status via API
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcDumpStatus(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	int Status;
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc_BlockStatus blockStatus;
	XRFdc_Mixer_Settings GetMixer_Settings;
	XRFdc_QMC_Settings GetQMCSettings;
	XRFdc_CoarseDelay_Settings GetCoarseDelaySettings;
	u32 GetNyquistZone;
	u32 DecimationFactor;

    // Calling this function gets the status of the IP
    XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

    for (Tile_Id=0; Tile_Id<=3; Tile_Id++) {
        xil_printf("=================================================\r\n");
    	if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1) {
			xil_printf("Tile: %d ADC Enabled\r\n", Tile_Id);
			xil_printf("  BlockStatus:  0x%x\r\n", ipStatus.ADCTileStatus[Tile_Id].BlockStatusMask);
			xil_printf("  TileState:    0x%08x\r\n", ipStatus.ADCTileStatus[Tile_Id].TileState);
			xil_printf("  PowerUpState: 0x%08x\r\n", ipStatus.ADCTileStatus[Tile_Id].PowerUpState);
			xil_printf("  PLLState:     0x%08x\r\n", ipStatus.ADCTileStatus[Tile_Id].PLLState);

			for(Block_Id=0; Block_Id<=3; Block_Id++) {
				if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
					xil_printf("  ***********************************\r\n");
					xil_printf("  Block: %d Enabled\r\n", Block_Id);

					//////////////////////////////////////////////////////////////////////////////
					// blockStatus
					XRFdc_GetBlockStatus(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &blockStatus);
					printf("    SamplingFreq:          %f\r\n", blockStatus.SamplingFreq);
					xil_printf("    DigitalDataPathStatus: %d\r\n", blockStatus.DigitalDataPathStatus);
					xil_printf("    AnalogDataPathStatus:  %d\r\n", blockStatus.AnalogDataPathStatus);
					xil_printf("    IsFIFOFlagsEnabled:    %d\r\n", blockStatus.IsFIFOFlagsEnabled);
					xil_printf("    IsFIFOFlagsAsserted:   %d\r\n", blockStatus.IsFIFOFlagsAsserted);
					xil_printf("    DataPathClocksStatus:  %d\r\n", blockStatus.DataPathClocksStatus);


					//////////////////////////////////////////////////////////////////////////////
					// Nyquist Zone
					Status = XRFdc_GetNyquistZone(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &GetNyquistZone);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetNyquistZone() failed\n\r");
						return;
					}
					xil_printf("    Nyquist Zone        :  %d\r\n", GetNyquistZone);


					//////////////////////////////////////////////////////////////////////////////
					// MixerSettings
			        Status =  XRFdc_GetMixerSettings(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &GetMixer_Settings);
					if (Status != XST_SUCCESS) {
				      xil_printf("Getting Fine Mixer failed\r\n");
					  	return ;
					}

					xil_printf("    **********Mixer Settings*********\r\n");
			        printf("    FREQ:              %f\r\n", GetMixer_Settings.Freq);
			        printf("    PHASE OFFSET:      %f\r\n", GetMixer_Settings.PhaseOffset);
			        xil_printf("    EVENT SOURCE:      %d\r\n", GetMixer_Settings.EventSource);
			        xil_printf("    MIXER MODE:        %d: ", GetMixer_Settings.MixerMode);
			        switch(GetMixer_Settings.MixerMode) {
			        case XRFDC_MIXER_MODE_OFF: xil_printf("OFF");
			        	break;
			        case XRFDC_MIXER_MODE_C2C: xil_printf("C2C");
			        	break;
			        case XRFDC_MIXER_MODE_C2R: xil_printf("C2R");
			        	break;
			        case XRFDC_MIXER_MODE_R2C: xil_printf("R2C");
			        	break;
			        default: xil_printf("unknown");
			        }
			        xil_printf("\r\n");
			        xil_printf("    COARSE MIXER FREQ: %d\r\n", GetMixer_Settings.CoarseMixFreq);


					//////////////////////////////////////////////////////////////////////////////
					// QMC Settings
			        Status = XRFdc_GetQMCSettings(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &GetQMCSettings);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetQMCSettings() failed\n\r");
						return;
					}

					xil_printf("    **********QMC Settings***********\r\n");
			        printf("    GainCorrectionFactor:   %f\r\n", GetQMCSettings.GainCorrectionFactor);
			        printf("    PhaseCorrectionFactor:  %f\r\n", GetQMCSettings.PhaseCorrectionFactor);
			        xil_printf("    EnablePhase:            %d\r\n", GetQMCSettings.EnablePhase);
			        xil_printf("    EnableGain:             %d\r\n", GetQMCSettings.EnableGain);
			        xil_printf("    OffsetCorrectionFactor: %d\r\n", GetQMCSettings.OffsetCorrectionFactor);
			        xil_printf("    EventSource:            %d\r\n", GetQMCSettings.EventSource);


					//////////////////////////////////////////////////////////////////////////////
					// Coarse Delay Settings
					Status = XRFdc_GetCoarseDelaySettings(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id, Block_Id, &GetCoarseDelaySettings);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetCoarseDelaySettings() failed\n\r");
						return;
					}
					xil_printf("    ******Coarse Delay Settings******\r\n");
			        xil_printf("    CoarseDelay:            %d\r\n", GetCoarseDelaySettings.CoarseDelay);
			        xil_printf("    EventSource:            %d\r\n", GetCoarseDelaySettings.EventSource);

   					//////////////////////////////////////////////////////////////////////////////
   					// Decimation Factor
					Status = XRFdc_GetDecimationFactor(RFdcInstPtr, Tile_Id, Block_Id, &DecimationFactor);
					if (Status != XST_SUCCESS) {
						xil_printf("XRFdc_GetDecimationFactor() failed\r\n");
						return;
					}

					xil_printf("    ******Decimation Factor**********\r\n");
					xil_printf("    Decimation Factor:      %d\r\n", DecimationFactor);

				} else {
					xil_printf("  ***********************************\r\n");
					xil_printf("  Block: %d Disabled\r\n", Block_Id);
				}
			}
    	} else {
			xil_printf("Tile: %d DAC Disabled\r\n", Tile_Id);
    	}
    }

    return;
}


/*****************************************************************************/
/**
*
* Reset DAC block via API
*
* @param	cmdVals[0] tile
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacReset(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 Tile_Id;
	Tile_Id = cmdVals[0];
    int Status;
	XRFdc_IPStatus ipStatus;
    XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);


	xil_printf("\r\n###############################################\r\n");
	//Reset DAC tile

	if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {

		Status = XRFdc_Reset(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id);
		if (Status != XST_SUCCESS) {
			xil_printf("XRFdc_Reset() failed for DAC Tile%d.\r\n",Tile_Id);
			return;
		}
		else{
  	  		xil_printf("DAC Tile%d reset.\r\n",Tile_Id);}
	}
	else{xil_printf("DAC Tile%d is not available.\n\r", Tile_Id);
	}
	xil_printf("###############################################\r\n\n");


    return;
}

void dacResetAll(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 Tile_Id;
	Tile_Id = cmdVals[0];
    int Status;
	XRFdc_IPStatus ipStatus;
    XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);


	xil_printf("\r\n###############################################\r\n");
	//Reset DAC tile
	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
		if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {
			Status = XRFdc_Reset(RFdcInstPtr, XRFDC_DAC_TILE, Tile_Id);
			if (Status != XST_SUCCESS) {
				xil_printf("XRFdc_Reset() failed for DAC Tile%d.\r\n",Tile_Id);
				return;
			}
			else{
				xil_printf("DAC Tile%d reset.\r\n",Tile_Id);}
		}
		else{xil_printf("DAC Tile%d is not available.\n\r", Tile_Id);
		}
	}
	xil_printf("###############################################\r\n\n");

    return;
}
/*****************************************************************************/
/**
*
* Reset ADC block via API
*
* @param	cmdVals[0] tile
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcReset(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 Tile_Id = cmdVals[0];
    int Status;
	XRFdc_IPStatus ipStatus;
    XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);


	xil_printf("\r\n###############################################\r\n");
	//Reset ADC tile
	if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1) {

		Status = XRFdc_Reset(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id);

		if (Status != XST_SUCCESS) {
			xil_printf("XRFdc_Reset() failed for ADC Tile%d.\r\n",Tile_Id);
			return;
		}
		else{
			xil_printf("ADC Tile%d reset.\r\n",Tile_Id);}
		}
	else{xil_printf("ADC Tile%d is not available.\n\r", Tile_Id);
	}
	xil_printf("###############################################\r\n\n");


    return;
}

void adcResetAll(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 Tile_Id = cmdVals[0];
    int Status;
	XRFdc_IPStatus ipStatus;
    XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);


	xil_printf("\r\n###############################################\r\n");
	//Reset ADC tiles
	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
		if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1) {

			Status = XRFdc_Reset(RFdcInstPtr, XRFDC_ADC_TILE, Tile_Id);

			if (Status != XST_SUCCESS) {
				xil_printf("XRFdc_Reset() failed for ADC Tile%d.\r\n",Tile_Id);
				return;
			}
			else{
				xil_printf("ADC Tile%d reset.\r\n",Tile_Id);}
			}
		else{xil_printf("ADC Tile%d is not available.\n\r", Tile_Id);
		}
	}
	xil_printf("###############################################\r\n\n");


    return;
}


/*****************************************************************************/
/**
*
* Dump DAC current setting via API
*
* Updated for Gen3 - JL 06/29/2020
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacCurrent(u32 *cmdVals)
{
	int Status;
	XRFdc* RFdcInstPtr = &RFdcInst;
	XRFdc_IPStatus ipStatus;
	u32 Tile_Id;
	u32 Block_Id;
	u32 OutputCurr;
	u32 OutputCurr2;

//	u32 OutputCurrSet;
	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

	xil_printf("\r\n###############################################\r\n");
	xil_printf("=== Data Current Report ===\n\r");

	for ( Tile_Id=0; Tile_Id<=1; Tile_Id++) {
    	if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {
			for ( Block_Id=0; Block_Id<=3; Block_Id++) {
				if (XRFdc_IsDACBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id) != 0U) {
					Status = XRFdc_GetOutputCurr(RFdcInstPtr, Tile_Id, Block_Id, &OutputCurr);
					if (RFdcInstPtr->RFdc_Config.IPType < XRFDC_GEN3) {
						if (Status != XST_SUCCESS) {
							xil_printf("XRFdc_GetOutputCurr() failed for DAC Tile%d Ch%d.\r\n",Tile_Id,Block_Id);
							return;
						}
						switch(OutputCurr) {
							case XRFDC_OUTPUT_CURRENT_20MA:
								xil_printf("   DAC Tile%d Ch%d output current is 20mA. DAC_AVTT should be 2.5V\r\n",Tile_Id,Block_Id);
								break;
							case XRFDC_OUTPUT_CURRENT_32MA:
								xil_printf("   DAC Tile%d Ch%d output current is 32mA. DAC_AVTT should be 3.0V\r\n",Tile_Id,Block_Id);
								break;
							default:
								xil_printf("DAC output current is not recognized.  Channel may not be enabled.\r\n");
								break;
						}
						}// gen3
						else{
							Status = XRFdc_GetDACCompMode(RFdcInstPtr, Tile_Id, Block_Id, &OutputCurr2);
							if (Status != XST_SUCCESS) {
								xil_printf("XRFdc_GetDACCompMode() failed for DAC Tile%d Ch%d.\r\n",Tile_Id,Block_Id);
								return;
								}
								switch(OutputCurr2) {
									case 0:
										xil_printf("   DAC Tile%d Ch%d output current mode is set to RFSoC Gen3.\r\n",Tile_Id,Block_Id);
										xil_printf("   DAC Tile%d Ch%d output current is set to %d uA.\r\n",Tile_Id,Block_Id,OutputCurr);
										break;
									case 1:
										xil_printf("   DAC Tile%d Ch%d output current mode is set to RFSoC Gen1/2.\r\n",Tile_Id,Block_Id);
										break;
									default:
										xil_printf("DAC output current is not recognized.  Channel may not be enabled.\r\n");
										break;
								}

						}
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
* Dac Sync Start
*
* @param	None
*
* @return	None
*
* @note		Pulled from xrfdc_mts_example.c
*
******************************************************************************/
void dacAdcSyncStart(u32 *cmdVals)
{
	XRFdc* RFdcInstPtr = &RFdcInst;

	XRFdc_ClrSetReg(RFdcInstPtr, XRFDC_ADC_TILE_DRP_ADDR(1) + XRFDC_HSCOM_ADDR,  0xB0, 0x0F, 0x01);
	XRFdc_ClrSetReg(RFdcInstPtr, XRFDC_ADC_TILE_DRP_ADDR(3) + XRFDC_HSCOM_ADDR,  0xB0, 0x0F, 0x01);

	/* Initialize DAC and ADC MTS Settings */
	XRFdc_MultiConverter_Init (&ADC_Sync_Config, 0, 0);
	XRFdc_MultiConverter_Init (&DAC_Sync_Config, 0, 0);


	return;
}

/*****************************************************************************/
/**
*
* Display DAC and ADC ready status
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void rfdcReady (u32 *cmdVals) {
	u32 Tile_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 val;

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

	xil_printf("\r\n###############################################\r\n");
	xil_printf("=== Data Converter Status Report ===\n\r");


	xil_printf("DAC Status\r\n");
	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
    	if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {
    		val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_DAC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_ADC_DEBUG_RST_OFFSET);
    		if(val & XRFDC_DBG_RST_CAL_MASK) {
    			xil_printf("   Tile: %d NOT ready\r\n", Tile_Id);
    		} else {
    			xil_printf("   Tile: %d ready\r\n", Tile_Id);
    		}
    	}
	}

	xil_printf("ADC Status\r\n");
	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
    	if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1) {
    		val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_ADC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_ADC_DEBUG_RST_OFFSET);
    		if(val & XRFDC_DBG_RST_CAL_MASK) {
    			xil_printf("   Tile: %d NOT ready\r\n", Tile_Id);
    		} else {
    			xil_printf("   Tile: %d ready\r\n", Tile_Id);
    		}
    	}
	}
	xil_printf("###############################################\r\n\n");

	return;
}


/*****************************************************************************/
/**
*
* Shutdown DAC's and ADC's
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void rfdcShutdown (u32 *cmdVals) {
	u32 Tile_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
//	u32 val;

	xil_printf("\r\n###############################################\r\n");
	xil_printf("Shutdown in progress...\n\r");
	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

 	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
    	if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {
   // 		val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_DAC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_ADC_DEBUG_RST_OFFSET);
   // 		if(val & XRFDC_DBG_RST_CAL_MASK) {
   // 			xil_printf("  DAC Tile%d NOT ready\r\n", Tile_Id);
   // 		} else {
    			XRFdc_Shutdown(RFdcInstPtr,1,Tile_Id);
    			xil_printf("   DAC Tile%d shutdown.\r\n", Tile_Id);
   // 		}
    	}

	}

 	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
    	if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1) {
    //		val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_ADC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_ADC_DEBUG_RST_OFFSET);
    	//	if(val & XRFDC_DBG_RST_CAL_MASK) {
    	//		xil_printf("   ADC Tile%d NOT ready.\r\n", Tile_Id);
  			XRFdc_Shutdown(RFdcInstPtr,0,Tile_Id);
    		xil_printf("   ADC Tile%d shutdown.\r\n", Tile_Id);
    	} else {
    		xil_printf("   ADC Tile%d not enabled.\r\n", Tile_Id);
      //		}
    	}
	}
	xil_printf("All enabled DAC's and ADC's shutdown. \r\n");

	sleep(1);

	xil_printf("\r\nThe Power-on sequence step. 0xF is complete.\r\n");

	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
    	if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 0) {
    //		val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_ADC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_ADC_DEBUG_RST_OFFSET);
    //		if(val & XRFDC_DBG_RST_CAL_MASK) {
    			xil_printf("  Tile: %d NOT ready.\r\n", Tile_Id);
    	}else {
    		    xil_printf("   DAC Tile%d Power-on Sequence Step: 0x%08x\r\n",Tile_Id,
    		    		Xil_In32(RFDC_BASE + 0x0000C + 0x04000 + Tile_Id * 0x4000));
    		//}
    	}
	}

	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
    	if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 0) {
    //		val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_ADC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_ADC_DEBUG_RST_OFFSET);
    	//	if(val & XRFDC_DBG_RST_CAL_MASK) {
    			xil_printf("  ADC Tile%d NOT ready.\r\n", Tile_Id);
    		} else {
    		    xil_printf("   ADC Tile%d Power-on Sequence Step: 0x%08x\r\n",Tile_Id,
    		    		Xil_In32(RFDC_BASE + 0x0000C + 0x14000 + Tile_Id * 0x4000));
    		}
 //   	}
	}

	xil_printf("\n\rData Converter shutdown is complete!");
	xil_printf("\r\n###############################################\r\n\n");


	return;
}


/*****************************************************************************/
/**
*
* Startup DAC's and ADC's
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void rfdcStartup (u32 *cmdVals) {

	int Tile_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 val;

	// Calling this function gets the status of the IP
	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);

	xil_printf("\r\n###############################################\r\n");
	xil_printf("Data Converter startup up in progress...\n\r");

	Xil_Out32(RFDC_BASE + 0x0004, 1);
	xil_printf("RF Data Converters Powered up.\r\n");
	sleep(1);

	// startup
	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
		if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {
			val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_ADC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_ADC_DEBUG_RST_OFFSET);
			if(val & XRFDC_DBG_RST_CAL_MASK) {
				xil_printf("  Tile: %d NOT ready.\r\n", Tile_Id);
			} else {
				XRFdc_StartUp(RFdcInstPtr, 1, Tile_Id);
				usleep(200000);
			}
		}
	}

	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
		if (ipStatus.ADCTileStatus[Tile_Id].IsEnabled == 1) {
			val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_ADC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_ADC_DEBUG_RST_OFFSET);
			if(val & XRFDC_DBG_RST_CAL_MASK) {
				xil_printf("  ADC Tile%d NOT ready.\r\n", Tile_Id);
			} else {
				XRFdc_StartUp(RFdcInstPtr, 0, Tile_Id);
				usleep(200000);
			}
		}
	}

	xil_printf("\r\nThe Power-on sequence step. 0xF is complete.\r\n");


	for ( Tile_Id=0; Tile_Id<=3; Tile_Id++) {
		if (ipStatus.DACTileStatus[Tile_Id].IsEnabled == 1) {
			val = XRFdc_ReadReg16(RFdcInstPtr, XRFDC_ADC_TILE_CTRL_STATS_ADDR(Tile_Id), XRFDC_ADC_DEBUG_RST_OFFSET);
			if(val & XRFDC_DBG_RST_CAL_MASK) {
				xil_printf("  Tile: %d NOT ready.\r\n", Tile_Id);
			} else {
				xil_printf("   DAC Tile%d Power-on Sequence Step: 0x%08x\r\n",Tile_Id,
						Xil_In32(RFDC_BASE + 0x0000C + 0x04000 + Tile_Id * 0x4000));
			}
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


	xil_printf("\n\rData Converter power up is complete!");
	xil_printf("\r\n###############################################\r\n");

	return;
}



