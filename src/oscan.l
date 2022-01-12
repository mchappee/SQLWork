/* Statement scanner for PL/SQL
   Basically the job of this scanner is to find the end of a PL/SQL statement
   or block. The intention is to maintain only enough context to avoid
   terminating at the end of a statement which is actually part of a larger
   block (such as a procedure or function).*/

%option stack
%{
#include <glib.h>
#define YY_DECL int yylex(char **ostmt, int *stmt_len)
#define APPEND_TOK do {input_len += yyleng; g_string_append(stmt, yytext);} while (0)
%}

%x leadingwhite
%x comment
%x strlit
%s declare
%s block
%s func
%s package
%s pbody
%s pproc
%s pinit

NAME            [A-Za-z][A-Za-z0-9]*
QNAME           \"[^\"]*\"
SQLID           ({NAME}|{QNAME})
iDECLARE        [dD][eE][cC][lL][aA][rR][eE]
iBEGIN          [bB][eE][gG][iI][nN]
iEND            [eE][nN][dD]
iCREATE         [cC][rR][eE][aA][tT][eE]
iPACKAGE        [pP][aA][cC][kK][aA][gG][eE]
iBODY           [bB][oO][dD][yY]
iPROC           [pP][rR][oO][cC][eE][dD][uU][rR][eE]
iFUNC           [fF][uU][nN][cC][tT][iI][oO][nN]
CPACKAGE        {iCREATE}[[:space:]]+{iPACKAGE}
CPCKBODY        {iCREATE}[[:space:]]+{iPACKAGE}[[:space:]]+{iBODY}
CFUNC           {iCREATE}[[:space:]]+{iFUNC}
%%
                             GString *stmt = g_string_new("");
                             int     input_len = 0;

                             BEGIN(leadingwhite);
<leadingwhite>[[:space:]]  { input_len++; }
<leadingwhite>[^[:space:]] { BEGIN(INITIAL);
                             yyless(0); }
"REM"("ARK")?.*            { input_len += yyleng;}
"--".*                     { APPEND_TOK; }
<*>"/*"                    { APPEND_TOK;
                             yy_push_state(comment); }
<comment>.|\n              { APPEND_TOK; }
<comment>"*/"              { APPEND_TOK;
                             yy_pop_state(); }
{QNAME}                    { APPEND_TOK; }
'                          { APPEND_TOK;
                             yy_push_state(strlit);}
<strlit>''                 { APPEND_TOK; }
<strlit>[^']*              { APPEND_TOK; }
<strlit>'                  { APPEND_TOK;
                             yy_pop_state();}
{CFUNC}                    { APPEND_TOK;
                             yy_push_state(func);}
{CPCKBODY}                 { APPEND_TOK;
                             yy_push_state(pbody);}
{CPACKAGE}                 { APPEND_TOK;
                             yy_push_state(package);}
<pbody>({iPROC}|{iFUNC})   { APPEND_TOK;
                             yy_push_state(pproc);}
{iDECLARE}                 { APPEND_TOK;
                             if (YYSTATE == pproc) {
                                 BEGIN(declare);
                             } else {
                                 yy_push_state(declare);
                             }
                           }
{iBEGIN}                   { APPEND_TOK;
                             if (YYSTATE == declare ||
                                 YYSTATE == pproc   ||
                                 YYSTATE == func) {
                                 BEGIN(block);
                             } else if (YYSTATE == pbody) {
                                 BEGIN(pinit);
                             } else {
                                 yy_push_state(block);
                             }
                           }
{iEND}([[:space:]]+{SQLID})?;   { APPEND_TOK;
                             yy_pop_state();
                             if (YYSTATE == INITIAL) {
                                 *stmt_len = input_len;
                                 *ostmt = stmt->str;
                                 g_string_free(stmt, 0);
                                 return 0;
                             }
                           }
<INITIAL>\;                { *stmt_len = input_len;
                             *ostmt = stmt->str;
                             g_string_free(stmt, 0);
                             return 0; }
<*><<EOF>>                 { *stmt_len = input_len;
                             *ostmt = stmt->str;
                             g_string_free(stmt, 0);
                             return 0; }
.                          { g_string_append_c(stmt, *yytext);
                             input_len++; }
\n                         { g_string_append_c(stmt, *yytext);
                             input_len++; }
