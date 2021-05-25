%{
int x=0;// marker  for A stream
int y=0;// marker for B stream
%}

%%
[' ''\t']* {}// Neglecting spaces and tabs
[Aa]* {
	if(y==0) x=1;
	else {
		printf("The text is invalid\n");
		return 0;
	}	
}
[Bb]* {y=1;}
.	 {printf("The text is invalid\n");return 0;}
\n {printf("The text is valid\n");return 0;}
%%
int yywrap(){}
int main(){
yylex();
return 0;
}
