@echo off
cd %~dp0
flex-bison\win_flex lex.l
flex-bison\win_bison -d yacc.y
MinGW\bin\gcc lex.yy.c yacc.tab.c interpreter.c -o resulti  2>log.txt

