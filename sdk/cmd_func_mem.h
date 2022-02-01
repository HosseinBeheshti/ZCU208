
#ifndef SRC_CMD_FUNC_MEM_H_   /* prevent circular inclusions */
#define SRC_CMD_FUNC_MEM_H_   /* by using protection macros */


/***************************** Include Files *********************************/
#include "xil_types.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void cli_cmd_func_mem_init(void);

void memread(u32 *cmdVals);
void memwrite(u32 *cmdVals);
void dacread(u32 *cmdVals);
void dacwrite(u32 *cmdVals);


/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/


#endif /* SRC_CMD_FUNC_MEM_H_ */
