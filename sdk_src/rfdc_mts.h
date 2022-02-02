
#ifndef SRC_CMD_FUNC_DAC_ADC_MTS_H_   /* prevent circular inclusions */
#define SRC_CMD_FUNC_DAC_ADC_MTS_H_   /* by using protection macros */


/***************************** Include Files *********************************/
#include "xil_types.h"
#include "xrfdc.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void cli_cmd_mts_init(void);
void dacMTS(u32 *cmdVals);
void dacMTSwl(u32 *cmdVals);
void adcMTSwl(u32 *cmdVals);
void adcMTS(u32 *cmdVals);
void dacAdcMTSStatus(u32 *cmdVals);


/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/


#endif /* SRC_CMD_FUNC_DAC_ADC_MTS_H_ */


