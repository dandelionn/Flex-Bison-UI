@echo off
cd %~dp0
MinGW\bin\gcc lex.yy.c yacc.tab.c compiler.c -o result