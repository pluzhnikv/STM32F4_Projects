/*******************************************************************************
* Console parser
*
* Author: 		Pluzhnik Victor
* Create date: 	05.06.15
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>                    /* character functions                 */

#include "globalvars.h"
#include "parser.h"

extern void TM_PWM_Init(int pulse);

/************************** Start of command description **************************/

/* Command Functions */
static void cmd_help (char *par);
static void ShowBoardInfo(char *par);
static void PrintHello(char *par);
static void cmd_SetPWMValue(char *par);
static void cmd_SetPWMValueTime(char *par);
static void cmd_SetPWMValueOnOff(char *par);
static void cmd_EmulateAlgorithm(char *par);

static const char help[] =
		"+ command ------------------+ function ---------------------------------+\n"
		"| HELP  or  ?               | displays this help                        |\n"
		"| BOARDINFO or BI           | displays board information                |\n"
		"| PR N                      | print 'Hello' N times                     |\n"
		"| PI N                      | set PWM on/off                            |\n"
		"| PWM N or P N              | set PWM value (1-9999)                    |\n"
		"| PT N1 N2                  | set PWM value N1 to N2 ms time            |\n"
		"| AL N                      | Emulate N algorithm                       |\n"
		"+---------------------------+-------------------------------------------+\n";

static const SCMD cmd[] = {
		"HELP",   	cmd_help,
		"?",      	cmd_help,
		"BOARDINFO",	ShowBoardInfo,
		"BI",		ShowBoardInfo,
		"PR",		PrintHello,
		"PI",		cmd_SetPWMValueOnOff,
		"PWM",		cmd_SetPWMValue,
		"P",		cmd_SetPWMValue,
		"PT",		cmd_SetPWMValueTime,
		"AL",		cmd_EmulateAlgorithm
};

//*************************** End of command description ***************************//

#define CMD_COUNT   (sizeof (cmd) / sizeof (cmd[0]))

void ParseLine(char *Line);

CParseString ParseString;

static const char LineBar[] = {"+-----------------------+-----------------------------+\n"};

/*----------------------------------------------------------------------------
 *        Process input string for long or short name entry
 *---------------------------------------------------------------------------*/
static char *get_entry (char *cp, char **pNext) 
{
	char *sp, lfn = 0, sep_ch = ' ';

	if (cp == NULL) {                          /* skip NULL pointers          */
		*pNext = cp;
		return (cp);
	}

	for ( ; *cp == ' ' || *cp == '\"'; cp++) { /* skip blanks and starting  " */
		if (*cp == '\"') { sep_ch = '\"'; lfn = 1; }
		*cp = 0;
	}
 
	for (sp = cp; *sp != CR && *sp != LF; sp++) {
		if ( lfn && *sp == '\"') break;
		if (!lfn && *sp == ' ' ) break;
	}

	for ( ; *sp == sep_ch || *sp == CR || *sp == LF; sp++) {
		*sp = 0;
		if ( lfn && *sp == sep_ch) { sp ++; break; }
	}

	*pNext = (*sp) ? sp : NULL;                /* next entry                  */
	return (cp);
}

void Parser(unsigned char Key)
{
		switch (Key) 
		{
			case CNTLQ:							// ignore Control S/Q
			case CNTLS:
            	break;
			case BACKSPACE:
			case DEL:
            	if (ParseString.Count > 0) {
					ParseString.String[--ParseString.Count] = 0; 	// decrement count
					printf("\010 \010");							// echo backspace
				};
				break;
			case ESC:
				ParseString.Count = 0;								// ESC - stop editing line & clear it
				printf("\n");
				break;
			case CR:
				Key = LF;
				printf("\n");
				ParseString.String[ParseString.Count] = 0;
				if (strlen(ParseString.String))
					ParseLine(ParseString.String);
				else
					printf(">");
				break;
			default:
				(ParseString.String[ParseString.Count] = Key);
				printf("%c", Key);									// echo and store character
				ParseString.Count++;								//increment line pointer
				break;
		}
		if (ParseString.Count == ParseStringLength)
		{
			ParseString.String[ParseString.Count] = 0;
			ParseLine(ParseString.String);	
		};
}

/*
	Commands: see HelpResource constant
*/
void ParseLine(char *in_line)
{
	char *sp,*cp,*next;
	unsigned int i;

	sp = get_entry (in_line, &next);
	if (*sp != NULL)
	{
		for (cp = sp; *cp && *cp != ' '; cp++) {
			*cp = toupper (*cp);                 /* command to upper-case       */
		}
		for (i = 0; i < CMD_COUNT; i++) {
			if (strcmp (sp, (const char *)&cmd[i].val)) {
				continue;
			}
			cmd[i].func (next);                  /* execute command function    */
			break;
		}
		if (i == CMD_COUNT) {
			printf ("\nCommand error\n");
		}
	};
	printf(">");
	ParseString.Count = 0;
}

void ShowBoardInfo(char *par)
{
	//printf("         1         2         3         4         5         6         7         8\n");
	//printf("12345678901234567890123456789012345678901234567890123456789012345678901234567890\n");
	printf("%s", LineBar);	
	printf("| %-22s| %-28s|\n", "Variable", "Value");
	printf("%s", LineBar);
	// Variables	
	//printf("| %-22s| %-28d|\n", "", ); - this is template
	printf("| %-22s| %-28d|\n", "Device addres", 1);
	printf("| %-22s| %-28d|\n", "RS 485 BaudRate", 2);
	printf("| %-22s| %-28d|\n", "RS 232 BaudRate", 115200);
	// End of variables
	printf("%s", LineBar);
}


// Commands routines
/*----------------------------------------------------------------------------
 *        Display Command Syntax help
 *---------------------------------------------------------------------------*/
static void cmd_help (char *par) {
	printf (help);
}

/*----------------------------------------------------------------------------
 *        Set new RS-485 address
 *---------------------------------------------------------------------------*/
static void PrintHello(char *par)
{
	int Value;
	int i;
	int Res;

	Res = sscanf(par, "%d", &Value);
	if ((Res == 0) || (Res == EOF))
		printf("Need 1 integer parameter\n");
	else
	{
		if ((Value < 0) || (Value > 255))
			printf("Invalid count parameter (must be between 0..255)\n");
		else
		{
			for (i=0; i<Value; i++)
				printf("Hello %d\n", i);
			xQueueSend(xQueueLedsAlgorithm, &Value, portMAX_DELAY);
		};
	};
}

static void cmd_SetPWMValueOnOff(char *par)
{
	int Value;
	int Res;

	Res = sscanf(par, "%d", &Value);
	if ((Res == 0) || (Res == EOF))
		printf("Need 1 integer parameter\n");
	else
	{
		if ((Value < 0) || (Value > 9999))
			printf("Invalid count parameter (must be between 1..9999)\n");
		else
		{
			TM_PWM_Init(Value);
		};
	};
}

static void cmd_SetPWMValue(char *par)
{
	static int PrevValue = 2000;
	int Value;
	int i;
	int Delta;
	int Res;

	Res = sscanf(par, "%d", &Value);
	if ((Res == 0) || (Res == EOF))
		printf("Need 1 integer parameter\n");
	else
	{
		if ((Value < 1) || (Value > 9999))
			printf("Invalid count parameter (must be between 1..9999)\n");
		else
		{
			Delta = Value - PrevValue;
			if (Delta > 0)
				Delta = 1;
			else
				Delta = -1;
			while (PrevValue != Value)
			{
				PrevValue = PrevValue+Delta;
				TM_PWM_Set(PrevValue);
				vTaskDelay(5);
			}

			printf("PWM value is %d of 10000\n", Value);
			PrevValue = Value;
		};
	};
}

static void cmd_SetPWMValueTime(char *par)
{
	int Value, Time;
	int Res;

	Res = sscanf(par, "%d %d", &Value, &Time);
	if ((Res != 2) || (Res == EOF))
			printf("Need 2 integer parameters\n");
	else
	{
		if ((Value < 1) || (Value > 9999))
			printf("Invalid count parameter (must be between 1..9999)\n");
		else
		{
			TM_PWM_SetTime(Value, Time);
		};
	};
}

static void cmd_EmulateAlgorithm(char *par)
{
	int Value;
	int Res;

	Res = sscanf(par, "%d", &Value);
	if ((Res == 0) || (Res == EOF))
		printf("Need 1 integer parameter\n");
	else
	{
		if (Value > 128)
			printf("Invalid parameter (must be between 0..128)\n");
		else
		{
			xQueueSend(xQueueLedsAlgorithm, &Value, portMAX_DELAY);
			printf("Algorithm %d\n", Value);
		};
	};
}
