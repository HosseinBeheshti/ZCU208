/*
 * adcFreezeCal.h
 *
 *  Created on: Aug 10, 2019
 *      Author: jlantz
 */

#ifndef ADCFREEZECAL_H_
#define ADCFREEZECAL_H_

/***************************** Include Files *********************************/
#include "xil_types.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
void adcFreezeCalStatus(u32 *cmdVals);
void adcSetUnFreezeCal(u32 *cmdVals);
void adcSetFreezeCal(u32 *cmdVals);

/************************** Variable Definitions *****************************/

#endif /* ADCFREEZECAL_H_ */
