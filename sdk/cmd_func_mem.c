


/***************************** Include Files *********************************/
//#include "xil_types.h"
#include "xil_io.h"
#include <stdio.h>
#include "cli.h"
#include "xparameters.h"
#include "main.h"
#include "cmd_func_mem.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/


/*****************************************************************************/
/**
*
* cli_cmd_func_mem_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_cmd_func_mem_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		//000000000011111111112222    000000000011111111112222222222333333333
		//012345678901234567890123    012345678901234567890123456789012345678
		{"################ Memory Write and Reads ################" , " "      , 0, *cmdComment   },
		{"memwrite"                , "<addr> <value>"                          , 2, *memwrite     },
		{"memread"                 , "<addr>"                                  , 1, *memread      },
		{"rfwrite"                 , "<offset> <value>"                        , 2, *dacwrite     },
		{"rfread"                  , "<offset>"                                , 1, *dacread      },
		{" "                       , " "                                       , 0, *cmdComment   },
	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}

/*****************************************************************************/
/**
*
* Memwrite function for command interface
*
* @param	cmdVals[0]=addr cmdVals[1]=value
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void memwrite(u32 *cmdVals) {
	Xil_Out32(cmdVals[0],cmdVals[1]);
}


/*****************************************************************************/
/**
*
* Memread function for command interface
*
* @param	cmdVals[0]=addr
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void memread(u32 *cmdVals) {
	xil_printf("addr: 0x%08x data: 0x%08x\n\r", cmdVals[0], Xil_In32(cmdVals[0]));
}


/*****************************************************************************/
/**
*
* Memwrite function for command interface
*
* @param	cmdVals[0]=offset cmdVals[1]=value
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacwrite(u32 *cmdVals) {
	if(cmdVals[0] > 0x23FFF) {
		xil_printf("rfdc_write: offset too large\r\n");
	} else {
		Xil_Out32(RFDC_BASE + cmdVals[0],cmdVals[1]);
	}
}


/*****************************************************************************/
/**
*
* Memread function for command interface
*
* @param	cmdVals[0]=offset
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void dacread(u32 *cmdVals) {
	if(cmdVals[0] > 0x23FFF) {
		xil_printf("rfdc_read: offset too large\r\n");
	} else {
		xil_printf("addr: 0x%08x rfdc_read: 0x%08x\n\r", RFDC_BASE + cmdVals[0], Xil_In32(RFDC_BASE + cmdVals[0]));
	}
}





