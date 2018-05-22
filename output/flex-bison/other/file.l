%{
#include <stdlib.h>
#include <string.h>
%}
%%
1*(0|(01))*  ;  printf("Cuv. acceptat%s\n",yytext);
[^\t\n]+   ;
%%

int yywrap(void){
    return 1;
}

int main()
{
yylex();
return 0;
}