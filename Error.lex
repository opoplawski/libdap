
/*
 -*- mode: c++; c-basic-offset:4 -*-

 This file is part of libdap, A C++ implementation of the OPeNDAP Data
 Access Protocol.

 Copyright (c) 2002,2003 OPeNDAP, Inc.
 Author: James Gallagher <jgallagher@opendap.org>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.

 (c) COPYRIGHT URI/MIT 1994-1996,1998,1999
*/ 

/*
  Scanner for the Error object. It recognizes the five keywords in the
  persistent representation of the Error object plus some syntactic sugar
  (`=', `{', ...). The object's persistant representation uses a keyword =
  value notation, where the values are quoted strings or integers.

  The scanner is not reentrant, but can share name spaces with other
  scanners. It must be processed by GNU's flex scanner generator.
*/

%{

#include "config_dap.h"

static char rcsid[] not_used = {"$Id$"};

#include <string.h>
#include <assert.h>

#define YY_NO_UNPUT
#define YY_DECL int Errorlex YY_PROTO(( void ))

#include "Error.tab.h"

int error_line_num = 1;
static int start_line;		/* used in quote and comment error handlers */

void store_integer();
void store_string();

%}
    
%option noyywrap
%x quote
%x comment

SCAN_INT		[0-9]+

SCAN_ERROR		error|Error|ERROR
SCAN_CODE		code|Code|CODE
SCAN_MSG		message|Message|MESSAGE
SCAN_PTYPE		program_type|ProgramType|PROGRAM_TYPE|Program_Type
SCAN_PROGRAM	program|Program|PROGRAM

NEVER   [^a-zA-Z0-9_/.+\-{}:;,]

%%


{SCAN_ERROR}	store_string(); return SCAN_ERROR;

{SCAN_CODE}				store_string(); return SCAN_CODE;
{SCAN_MSG}				store_string(); return SCAN_MSG;
{SCAN_PTYPE}			store_string(); return SCAN_PTYPE;
{SCAN_PROGRAM}			store_string(); return SCAN_PROGRAM;

{SCAN_INT}				store_integer(); return SCAN_INT;

"{" 	    	    	return (int)*yytext;
"}" 	    	    	return (int)*yytext;
";" 	    	    	return (int)*yytext;
"="			return (int)*yytext;

[ \t]+
\n	    	    	++error_line_num;
<INITIAL><<EOF>>    	yy_init = 1; error_line_num = 1; yyterminate();

"#"	    	    	BEGIN(comment);
<comment>[^\n]*
<comment>\n		++error_line_num; BEGIN(INITIAL);
<comment><<EOF>>        yy_init = 1; error_line_num = 1; yyterminate();

\"			BEGIN(quote); start_line = error_line_num; yymore();
<quote>[^"\n\\]*	yymore();
<quote>[^"\n\\]*\n	yymore(); ++error_line_num;
<quote>\\.		yymore();
<quote>\"		{ 
    			  BEGIN(INITIAL); 
			  store_string();
			  return SCAN_STR;
                        }
<quote><<EOF>>		{
                          char msg[256];
			  sprintf(msg,
				  "Unterminated quote (starts on line %d)\n",
				  start_line);
			  YY_FATAL_ERROR(msg);
                        }

{NEVER}                 {
                          if (yytext) {	/* suppress msgs about `' chars */
                            fprintf(stderr, "Character `%c' is not", *yytext);
                            fprintf(stderr, " allowed (except within");
			    fprintf(stderr, " quotes) and has been ignored\n");
			  }
			}
%%

// These three glue routines enable DDS to reclaim the memory used to parse a
// DDS off the wire. They are here because this file can see the YY_*
// symbols; the file DDS.cc cannot.

void *
Error_buffer(FILE *fp)
{
    return (void *)Error_create_buffer(fp, YY_BUF_SIZE);
}

void
Error_switch_to_buffer(void *buf)
{
    Error_switch_to_buffer((YY_BUFFER_STATE)buf);
}

void
Error_delete_buffer(void *buf)
{
    Error_delete_buffer((YY_BUFFER_STATE)buf);
}

void
store_integer()
{
    Errorlval.integer = atoi(yytext);
}

void
store_string()
{
    Errorlval.string = yytext;
}

/* 
 * $Log: Error.lex,v $
 * Revision 1.13  2004/02/19 19:42:52  jimg
 * Merged with release-3-4-2FCS and resolved conflicts.
 *
 * Revision 1.11.2.1  2004/02/04 00:05:11  jimg
 * Memory errors: I've fixed a number of memory errors (leaks, references)
 * found using valgrind. Many remain. I need to come up with a systematic
 * way of running the tests under valgrind.
 *
 * Revision 1.12  2003/12/08 18:02:29  edavis
 * Merge release-3-4 into trunk
 *
 * Revision 1.11  2003/04/22 19:40:27  jimg
 * Merged with 3.3.1.
 *
 * Revision 1.10  2003/02/21 00:14:24  jimg
 * Repaired copyright.
 *
 * Revision 1.9.2.1  2003/02/21 00:10:07  jimg
 * Repaired copyright.
 *
 * Revision 1.9  2003/01/23 00:22:24  jimg
 * Updated the copyright notice; this implementation of the DAP is
 * copyrighted by OPeNDAP, Inc.
 *
 * Revision 1.8  2001/08/24 17:46:22  jimg
 * Resolved conflicts from the merge of release 3.2.6
 *
 * Revision 1.7.4.1  2001/08/16 17:26:19  edavis
 * Use "%option noyywrap" instead of defining yywrap() to return 1.
 *
 * Revision 1.7  2000/09/22 02:17:20  jimg
 * Rearranged source files so that the CVS logs appear at the end rather than
 * the start. Also made the ifdef guard symbols use the same naming scheme and
 * wrapped headers included in other headers in those guard symbols (to cut
 * down on extraneous file processing - See Lakos).
 *
 * Revision 1.6  2000/06/07 18:06:58  jimg
 * Merged the pc port branch
 *
 * Revision 1.5.20.1  2000/06/02 18:21:27  rmorris
 * Mod's for port to Win32.
 *
 * Revision 1.5  1999/04/29 02:29:29  jimg
 * Merge of no-gnu branch
 *
 * Revision 1.4.2.1  1999/02/05 09:32:34  jimg
 * Fixed __unused__ so that it not longer clashes with Red Hat 5.2 inlined math code
 *
 * Revision 1.4  1998/08/06 16:14:47  jimg
 * Fixed the cvs log. It cannot have the $ Log $ and the comment thing on the
 * same line...
 *
 * Revision 1.3  1996/10/28 23:46:55  jimg
 * Added define of YY_NO_UNPUT to suppress creation of that function.
 *
 * Revision 1.2  1996/08/13 18:18:17  jimg
 * Added not_used to rcsid definition.
 * Added include of assert.h.
 *
 * Revision 1.1  1996/05/31 23:18:16  jimg
 * Added.
 */
