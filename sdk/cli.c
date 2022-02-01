/*
 * cli.c
 *
 */


/***************************** Include Files *********************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xcoresightpsdcc.h"
#include "xuartps_hw.h"
#include <xil_types.h>
#include "cli.h"
#include "xstatus.h"


/******************** Constant Definitions **********************************/
/* DCC Status Bits */
#define XCORESIGHTPS_DCC_STATUS_RX (1 << 30)


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/
void cmdComment(void) { return; }		//null command


/************************** Function Prototypes ******************************/
u32 my_XCoresightPs_DccGetStatus(void);


/************************** Variable Definitions *****************************/

char lineOld[LINEMAX];
char line[LINEMAX];		// buffer for command line
char seps[] = " ,\t\n\r";
char *cmdPtr;

u32 cmdVals[MAX_CMD_VALS];
int cmdExitVal  = 0;

unsigned char numCmdVals = 0;
int numTablePtrs = 0;        // index for the cliCmdTable

CMDSTRUCT cliCmds_builtIn[] = {
	//CMD_STRING_LEN=25      CMD_HELP_LEN=40
	//000000000011111111112222    000000000011111111112222222222333333333
	//012345678901234567890123    012345678901234567890123456789012345678
	{"###################### Built In ########################", " "       , 0, *cmdComment   },
	{"#"                       , "- Comment on cmdLine"                    , 0, *cmdComment   },
	{"exit"                    , "- Exit and shutdown data converters"     , 0, *doExit       },
	{"?"                       , "- This Menu"                             , 0, *doHelp       },
	{" "                       , " "                                       , 0, *cmdComment   },
};

TBLSTRUCT cliCmdTable[TBL_PTR_MAX];

/*****************************************************************************/
/**
*
* cli_init Initializes memory for command line structure
*
* @param	TBD
*
* @return	TBD
*
* @note		TBD
*
******************************************************************************/
void cli_init(void)
{
	cli_addCmds(cliCmds_builtIn, sizeof(cliCmds_builtIn)/sizeof(cliCmds_builtIn[0]));
}

/*****************************************************************************/
/**
*
* cli_addCmds increase and add commands to command line structure
*
* @param	TBD
*
* @return	TBD
*
* @note		TBD
*
******************************************************************************/
void cli_addCmds(CMDSTRUCT *cmds2add, int numCmds)
{
	if (numTablePtrs < TBL_PTR_MAX) {
		cliCmdTable[numTablePtrs].cmdPointer = cmds2add;
		cliCmdTable[numTablePtrs].cmdCount = numCmds;
		numTablePtrs++;
	}
	else xil_printf("\r\nError: max number of command indexes reached\r\n");
}

/*****************************************************************************/
/**
*
* my_sscanf
*
* @param	TBD
*
* @return	TBD
*
* @note		TBD
*
******************************************************************************/

int my_sscanf(char *tokenPtr, u32 *value) {
    u32 oldval;
    u8 val8;
    int len;

    len = 0;
    oldval = 0;
    while( (val8 = *tokenPtr) ) {
    	len++;
        tokenPtr++;
        val8  -= 0x30;
        if (val8 > 9) val8  = (val8 & 0xdf) - 7;
        if(val8 > 0xf) {
        	len = 0;
        	oldval = 0;
                break;
        }
        oldval = oldval*16 + val8;
    }
    *value = oldval;
    return len;
}

/*****************************************************************************/
/**
*
* getAndParseInput
*
* @param	TBD
*
* @return	TBD
*
* @note		TBD
*
******************************************************************************/
char* getAndParseInput( void )
{
    u8 temp, temp1;
    char *cmdValPtr;
    int i = 0;

    while(1) {
    	temp = inbyte();
//    	xil_printf("'inbyte=0x%02x'", temp);
        switch(temp) {
        		case 0:	   goto EXIT_WHILE;   //jtagterminal seems to pass \000 so treat as \n
        		   	   	   break;
        		case 0x0D: goto EXIT_WHILE;   //enter
        				   break;
        		case 0x0A:
        				   break;
                case 0x08: xil_printf("\b \b");
                           i--;
                           break;
                case 0x1b: if((temp1 = inbyte()) == 0x5b) temp1=inbyte();       //Control Key. Weird, if I don't print, I get extra char 0x5b after 0x1b
                           switch(temp1) {
                                case 0x41: strncpy(line, lineOld, LINEMAX);     //up arrow
                                           xil_printf("\r> %s", line);
                                           i = strlen(line);
                                           break;
                                case 0x44: xil_printf("\b \b");                 //back arrow
                                           i--;
                                           break;
                                default:   break;
                           }
                           break;
                default:
                        xil_printf("%c",temp);		//echo key entered
                        line[i++] = temp;
        }
        if(i >= (LINEMAX-1)) {
        	xil_printf("max line length reached \r\n");
        	goto EXIT_WHILE;
    }
    }
    EXIT_WHILE:
//    xil_printf("\n\r");
    line[i]= '\0';


    strncpy(lineOld, line, LINEMAX);			// make copy of line to history buffer

    // Command scanned into *line so time to scan command
    //
    cmdPtr     = strtok( line, seps );

    numCmdVals = 0;
    for(int i=0; i<MAX_CMD_VALS; i++) {
    	cmdValPtr = strtok( NULL, seps );
    	if(!cmdValPtr) break;

    	// Process Hex entry if value starts with "0x"
        if( (cmdValPtr[0]=='0') && (tolower(cmdValPtr[1])=='x') ) {
    		cmdValPtr += 2;
    		if(my_sscanf(cmdValPtr, &cmdVals[i])) numCmdVals++;

    	//Process signed int if starts with '-'
        } else if(cmdValPtr[0]=='-') {
        	cmdValPtr++;
    		cmdVals[i] = atoi(cmdValPtr);
    		cmdVals[i] *= -1;
    		numCmdVals++;

    	// Otherwize process as integer value
    	} else {
    		cmdVals[i] = atoi(cmdValPtr);
    		numCmdVals++;
    	}
    }

    return cmdPtr;
}

/*****************************************************************************/
/**
*
* Parse and run command
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
int cmdParseAndRun( void )
{
	int found;
	void (*func)();

    cmdPtr = getAndParseInput();
    found=0;
	if( cmdPtr != NULL ) {
		for(int i=0; i < numTablePtrs; i++) {
			for (int j = 0; j < cliCmdTable[i].cmdCount; j++) {
				if( !strcmp(cmdPtr, cliCmdTable[i].cmdPointer[j].cmdString) ) {
					func = cliCmdTable[i].cmdPointer[j].cmdFunc;
					found = 1;
					if(numCmdVals == cliCmdTable[i].cmdPointer[j].valCnt) {
						xil_printf("\r\n");
						(*func)(cmdVals);
						return XST_SUCCESS;
					} else {
						xil_printf("\r\nError: wrong number of values\r\n");
						return XST_FAILURE;
					}
				}
			}
		}
		if(!found) {
			xil_printf("\r\nError: %s command not found\r\n", cmdPtr);
			return XST_FAILURE;
		}
	}
//	xil_printf("cmdPtr is NULL\r\n");
	return XST_NO_DATA;

}

/*****************************************************************************/
/**
*
* Help function for command interface
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void doHelp(void) {
	for(int i=0; i < numTablePtrs; i++) {
		for (int j = 0; j < cliCmdTable[i].cmdCount; j++) {
			xil_printf("    %s %s\r\n", cliCmdTable[i].cmdPointer[j].cmdString, cliCmdTable[i].cmdPointer[j].cmdHelp);
		}
	}
}

/*****************************************************************************/
/**
*
* Help function for command interface
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void doExit(void) {

    xil_printf("Exiting application...\r\n");
    cmdExitVal = 1;
}


/*****************************************************************************/
/**
*
* outbyte: Override for BSP outbyte that removes '\r' when printing to
*          Coresight UART
*          
* @param	c contains character to output to Coresight JTAG UART
*
* @return
*		- void
*
* @note		None.
*
******************************************************************************/
#if defined (STRIP_CHAR_CR) && STDIN_BASEADDRESS==0xFE800000
void outbyte(char c) {

	if(c == '\r') {
		return;
	}
	XCoresightPs_DccSendByte(STDOUT_BASEADDRESS, c);
}
#endif


/*****************************************************************************/
/**
*
* myChar_checkRx: Checks UartPs or coresight jtag uart for available rx data
*          
* @param	BaseAddress of uart
*
* @return
*		- 1 if a character is available from the uart
*   - 0 if no char available
*
* @note		None.
*
******************************************************************************/
int myChar_checkRx(u32 BaseAddress)
{
	int val=0;

	//If mpsoc coresight uart
	if(BaseAddress == 0xFE800000) {
		if((my_XCoresightPs_DccGetStatus() & XCORESIGHTPS_DCC_STATUS_RX)) {
			val = 1;
		}
		dsb();

	//Do mpsoc PS uart
	} else {
		if(XUartPs_IsReceiveData(BaseAddress)) {
			val = 1;
		}
	}

	return val;
}


/*****************************************************************************/
/**
*
* myChar_getRx: Read char from UartPs or coresight jtag uart
*          
* @param	BaseAddress of uart
*
* @return
*		- Data from uart
*
* @note		None.
*
******************************************************************************/
u8 myChar_getRx(u32 BaseAddress)
{
	u8 Data;

	//If mpsoc coresight uart
	if(BaseAddress == 0xFE800000) {

		#ifdef __aarch64__
			asm volatile ("mrs %0, dbgdtrrx_el0" : "=r" (Data));
		#elif defined (__GNUC__) || defined (__ICCARM__)
			asm volatile("mrc p14, 0, %0, c0, c5, 0"
					: "=r" (Data));
		#else
			{
				volatile register u32 Reg __asm("cp14:0:c0:c5:0");
				Data = Reg;
			}
		#endif
		isb();

	//Do mpsoc PS uart
	} else {
		Data = (u8)(XUartPs_ReadReg(BaseAddress, XUARTPS_FIFO_OFFSET));
	}

	return Data;
}


/*****************************************************************************/
/**
*
* my_XCoresightPs_DccGetStatus: Check Coresight JTAG UART for valid data
*          
* @param	moid
*
* @return
*		- Status = 0 if no char available
*
* @note		None.
*
******************************************************************************/
u32 my_XCoresightPs_DccGetStatus(void)
{
	u32 Status = 0U;

#ifdef __aarch64__
	asm volatile ("mrs %0, mdccsr_el0" : "=r" (Status));
#elif defined (__GNUC__) || defined (__ICCARM__)
	asm volatile("mrc p14, 0, %0, c0, c1, 0"
			: "=r" (Status) : : "cc");
#else
	{
		volatile register u32 Reg __asm("cp14:0:c0:c1:0");
		Status = Reg;
	}
#endif
	return Status;
}


/*****************************************************************************/
/**
*
* my_strcpy
*
* @param	TBD
*
* @return	TBD
*
* @note		TBD
*
******************************************************************************/
char *my_strcpy(char *DestPtr, const char *SrcPtr)
{
	unsigned int Count;

	for (Count=0U; SrcPtr[Count] != '\0'; ++Count)
	{
		DestPtr[Count] = SrcPtr[Count];
	}
	DestPtr[Count] = '\0';

	return DestPtr;
}


/*****************************************************************************/
/**
*
* getAndParseInput
*
* @param	TBD
*
* @return	TBD
*
* @note		TBD
*
******************************************************************************/
char * my_strcat(char* Str1Ptr, const char* Str2Ptr)
{
	while( *Str1Ptr )
	{
		Str1Ptr++;
	}

	while( *Str2Ptr )
	{
		*Str1Ptr = *Str2Ptr;
		Str1Ptr++; Str2Ptr++;
	}

	*Str1Ptr = '\0';
	return --Str1Ptr;
}
