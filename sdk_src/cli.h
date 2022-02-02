/*
 * cli.h
 *
 */

#ifndef SRC_CLI_H_   /* prevent circular inclusions */
#define SRC_CLI_H_   /* by using protection macros */

/***************************** Include Files *********************************/
#include "xil_types.h"

/******************** Constant Definitions **********************************/
//maximum number of values for a command
#define MAX_CMD_VALS    3
#define CMD_STRING_LEN 56
#define CMD_HELP_LEN   56

// Max cli command line length
#define LINEMAX 81

// Max number of cli command table indexes
#define TBL_PTR_MAX 100



/**************************** Type Definitions *******************************/

//cmd interface
typedef struct cmdStruct {
	char cmdString[CMD_STRING_LEN];
	char cmdHelp[CMD_HELP_LEN];
	unsigned char valCnt;
	void (*cmdFunc)();
} CMDSTRUCT;

//cli cmd table structure
typedef struct tblStruct {
	CMDSTRUCT *cmdPointer;
	int cmdCount;
} TBLSTRUCT;

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

extern u32 cmdVals[MAX_CMD_VALS];
extern CMDSTRUCT cmdtab[];
extern unsigned char numCmdVals;
extern int cmdExitVal;

/************************** Function Prototypes ******************************/

void cmdComment(void);		//null command

void cli_init(void);
void cli_addCmds(CMDSTRUCT *cmds2add, int numCmds);
void cli_cpyCmds(CMDSTRUCT *cmds2add, CMDSTRUCT *cmdsTable, int curCliCmds, int numCmds);

int my_sscanf(char *tokenPtr, u32 *value);
char* getAndParseInput( void );
void doHelp(void);
int cmdParseAndRun( void );
void doExit(void);

u8 myChar_getRx(u32 BaseAddress);
int myChar_checkRx(u32 BaseAddress);

char *my_strcpy(char *DestPtr, const char *SrcPtr);
char * my_strcat(char* Str1Ptr, const char* Str2Ptr);

#endif /* SRC_CLI_H_ */

