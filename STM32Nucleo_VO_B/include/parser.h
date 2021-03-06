/*----------------------------------------------------------------------------
* Parser header file
------------------------------------------------------------------------------*/

#ifndef __PARSER_H
#define __PARSER_H

#define CNTLQ      0x11
#define CNTLS      0x13
#define DEL        0x7F
#define BACKSPACE  0x08
#define CR         0x0D
#define LF         0x0A
#define ESC        0x1B 

#define ParseStringLength 30

typedef struct {
	unsigned char Count;
	char String[ParseStringLength+1];
} CParseString;

/* Command definitions structure. */
typedef struct scmd {
   char val[12];
   void (*func)(char *par);
} SCMD;

extern CParseString ParseString;

extern void Parser(unsigned char Key);

#endif // __PARSER_H
