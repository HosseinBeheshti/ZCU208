
#ifndef SRC_URAM_PLAY_CAP_H_   /* prevent circular inclusions */
#define SRC_URAM_PLAY_CAP_H_   /* by using protection macros */


/***************************** Include Files *********************************/
#include "xil_types.h"

/************************** Constant Definitions *****************************/
#define URAM_PLAY_EN 1
#define URAM_CAP_EN 1
#define MULTI_BOARD 0

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void cli_uram_play_cap_init(void);

#if URAM_PLAY_EN == 1
void uramPlay(u32 *cmdVals);
void uramStop (u32 *cmdVals);
#endif

#if URAM_CAP_EN == 1
void uramCap(u32 *cmdVals);
#if MULTI_BOARD == 1
void capMaster (u32 *cmdVals);
#endif
#endif

/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/


#endif /* SRC_URAM_PLAY_CAP_H_ */


