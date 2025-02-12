%{
	int c = 0;
	int v = 0;
%}

%%
[aeiouAEIOU] {v++;}
[a-zA-Z] {c++;}

%%

int main() {
	yylex();
	printf("this works");
}
int yywrap()
{
	return 1;
}