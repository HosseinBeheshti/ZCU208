
#ifndef SRC_CMD_DSAVOP_H_   /* prevent circular inclusions */
#define SRC_CMD_DSAVOP_H_   /* by using protection macros */


/***************************** Include Files *********************************/
#include "xil_types.h"
#include "xrfdc.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void cli_rfdcDSAVOP_init(void);

//void dacCurrent(u32 *cmdVals);
void dacSetVOP(u32 *cmdVals);
void adcSetDSA(u32 *cmdVals);
void adcGetDSA(u32 *cmdVals);


/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/


#endif /* SRC_CMD_FUNC_DAC_H_ */


