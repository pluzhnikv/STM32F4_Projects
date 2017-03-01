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

#include "FreeRTOS.h"
#include "task.h"
#include "globalvars.h"
#include "parser.h"

/************************** Start of command description **************************/

/* Command Functions */
static void cmd_help (char *par);
static void ShowBoardInfo(char *par);
static void GetInteger(char *par);

static const char help[] =
		"+ command ------------------+ function ---------------------------------+\n"
		"| HELP  or  ?               | displays this help                        |\n"
		"| BOARDINFO or BI           | displays board information                |\n"
		"| GI N                      | get GetInteger                            |\n"
		"+---------------------------+-------------------------------------------+\n";

static const SCMD cmd[] = {
		"HELP",   	cmd_help,
		"?",      	cmd_help,
		"BOARDINFO",	ShowBoardInfo,
		"BI",		ShowBoardInfo,
		"GI",		GetInteger
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
 
	for (sp = cp; *sp != 0; sp++) {
		if ( lfn && *sp == '\"') break;
		if (!lfn && *sp == ' ' ) break;
	}

	for ( ; *sp == sep_ch; sp++) {
		*sp = 0;
		if ( lfn && *sp == sep_ch) {sp++; break;}
	}

	*pNext = (*sp) ? sp : NULL;                /* next entry                  */
	return (cp);
}

int Parser(unsigned char Key)
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
		return 0;
}

/*----------------------------------------------------------------------------
 *        Get line arguments
 *---------------------------------------------------------------------------*/
int get_args(char *s)
{
	char *next;

	ParseString.argc = 0;
	while ((ParseString.argv[ParseString.argc] = get_entry (s, &next)) != NULL)
	{
		ParseString.argc++;
		if (next == NULL) break;
		s = next;
	}
	return ParseString.argc;
}

/*
	Commands: see HelpResource constant
*/
void ParseLine(char *in_line)
{
	char *cp;
	unsigned int i;

	if (get_args(in_line))
	{
		ParseString.argc--;
		for (cp = ParseString.argv[0]; *cp; cp++) {
			*cp = toupper (*cp);                 /* command to upper-case       */
		}
		for (i = 0; i < CMD_COUNT; i++) {
			if (strcmp (ParseString.argv[0], (const char *)&cmd[i].val)) {
				continue;
			}
			cmd[i].func (ParseString.argv[1]);                  /* execute command function    */
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
 *       Set r g b (percent) and time (hours)
 *---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *        GetInteger - get 1 integer value
 *---------------------------------------------------------------------------*/
static void GetInteger(char *par)
{
	int Value;
	int Res = 0;

	Res = sscanf(par, "%d", &Value);
	if ((Res == 0) || (Res == EOF))
		printf("Need 1 integer parameter\n");
	else
	{
		if (Value > 0)
		{
			// do something
			printf("Value is %d\n", Value);
		}
		else
		{
			// do something
			printf("Value is %d\n", Value);
		}
	};
}
