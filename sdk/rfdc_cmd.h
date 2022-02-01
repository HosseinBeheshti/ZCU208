
#ifndef SRC_CMD_FUNC_DAC_H_   /* prevent circular inclusions */
#define SRC_CMD_FUNC_DAC_H_   /* by using protection macros */


/***************************** Include Files *********************************/
#include "xil_types.h"
#include "xrfdc.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void cli_rfdc_cmd_init(void);

void dacDumpStatus(u32 *cmdVals);
void dacDumpRegs (u32 *cmdVals);
void adcDumpStatus(u32 *cmdVals);
void rfSetMixerFreqOptPrint(u32 type, u32 Tile_Id, u32 Block_Id, double newFreq, int printEnable);
void dacReset(u32 *cmdVals);
void adcReset(u32 *cmdVals);
void dacResetAll(u32 *cmdVals);
void adcResetAll(u32 *cmdVals);
void dacCurrent(u32 *cmdVals);
void dacSetVOP(u32 *cmdVals);
void adcSetDSA(u32 *cmdVals);
void adcGetDSA(u32 *cmdVals);
void rfdcReady (u32 *cmdVals);
void rfdcShutdown (u32 *cmdVals);
void rfdcStartup (u32 *cmdVals);
void adcDumpRegs (u32 *cmdVals);
void RFdcCheckAxisWidth(XRFdc *RFdcInstPtr, u32 DacExpectedBits, u32 AdcExpectedBits);


/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/


#endif /* SRC_CMD_FUNC_DAC_H_ */


