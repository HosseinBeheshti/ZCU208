/*
 * rfdc_poweron_status.h
 *
 *  Created on: Aug 9, 2019
 *      Author: jlantz
 */

#ifndef RFDC_POWERON_STATUS_H_
#define RFDC_POWERON_STATUS_H_


/***************************** Include Files *********************************/
#include "xil_types.h"


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

void cli_rfdc_poweron_status_init(void);

void rfdcPoweronStatus(u32 *cmdVals);


/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/






#endif /* RFDC_POWERON_STATUS_H_ */
