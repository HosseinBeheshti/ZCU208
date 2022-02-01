
#ifndef SRC_CMD_FUNC_DAC_ADC_INT_DEC_H_   /* prevent circular inclusions */
#define SRC_CMD_FUNC_DAC_ADC_INT_DEC_H_   /* by using protection macros */


/***************************** Include Files *********************************/
#include "xil_types.h"
#include "xrfdc.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void cli_DacAdcIntDec_init(void);

void adcGetDecimation(u32 *cmdVals);
void dacGetInterpolation(u32 *cmdVals);


/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/


#endif /* SRC_CMD_FUNC_DAC_ADC_INT_DEC_H_ */


