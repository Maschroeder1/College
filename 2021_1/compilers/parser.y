%{
    #include <stdlib.h>
    #include "hash.h"
    #include "ast.h"
    
    int yyerror();
    int getLineNumber();
    int yylex();
%}

%union {
    HASH_NODE *symbol;
    AST *ast;
    int integer;
}

%token KW_CHAR
%token KW_INT
%token KW_FLOAT
%token KW_DATA

%token KW_IF
%token KW_ELSE
%token KW_UNTIL
%token KW_COMEFROM
%token KW_READ
%token KW_PRINT
%token KW_RETURN

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF
%token OPERATOR_RANGE

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%token TOKEN_ERROR

%type<ast> expression expression_leaf command command_attribuition command_block command_print print_elements_or_empty
%type<ast> print_elements print_element return_command flow_control if_block optional_else identifier
%type<ast> optional_bracket_expression until_block label come_from expression_function expression_function_arguments
%type<ast> expession_function_with_or_without_arguments
%type<integer> binary_operator unary_operator

%left '~'
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF OPERATOR_RANGE '<' '>'
%left '+' '-'
%left '*' '/'

%%

init: data_section_declaration functions_block;

data_section_declaration: KW_DATA '{' data_section '}';
data_section: data_section_variable ';' data_section 
    | ;
data_section_variable: regular_variable_intiation 
    | vector_variable_declaration;

regular_variable_intiation: regular_variable_declaration '=' variable_literal_declaration;

regular_variable_declaration: type_declaration ':' identifier;
vector_variable_declaration: type_declaration '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' identifier optional_vector_initialization;
optional_vector_initialization: '=' variable_literal_declaration vector_initialization 
    | ;
vector_initialization: variable_literal_declaration vector_initialization 
    | ;

type_declaration: KW_INT 
    | KW_CHAR 
    | KW_FLOAT;
variable_literal_declaration: LIT_INTEGER
    | LIT_CHAR;



functions_block: functions_declarations functions_block 
    | ;
functions_declarations: type_declaration ':' identifier '(' parameter_list_or_empty ')' '{' command_block '}' { astPrint($8, 0); };
parameter_list_or_empty: parameter_list 
    | ;
parameter_list: regular_variable_declaration ',' parameter_list 
    | regular_variable_declaration;

command_block: command ';' command_block { $$ = astCreate(AST_CMD_BLOCK, 0, $1, $3, 0,0); }
    | { $$ = 0; };
command: command_attribuition { $$ = $1; }
    | command_print { $$ = $1; }
    | return_command { $$ = $1; }
    | flow_control { $$ = $1; }
    | '{' command_block '}' { $$ = $2; }
    | label { $$ = $1; }
    | { $$ = 0; };

command_attribuition: identifier optional_bracket_expression '=' expression { $$ = astCreate(AST_ATTR, 0, $1, $4, $2,0); }
optional_bracket_expression: '[' expression ']' { $$ = astCreate(AST_BRACKET_EXPR, 0, $2, 0,0,0); }
    | { $$ = 0; };

command_print: KW_PRINT print_elements_or_empty { $$ = $2; };
print_elements_or_empty: print_elements { $$ = $1; }
    | { $$ = 0; } ;
print_elements: print_element ',' print_elements { $$ = astCreate(AST_PRINT_BLOCK, 0, $1, $3, 0,0); }
    | print_element { $$ = $1; };
print_element: LIT_STRING { $$ = astCreate(AST_SYMBOL, $1, 0,0,0,0); }
    | expression { $$ = $1; };

return_command: KW_RETURN expression { $$ = astCreate(AST_RETURN, 0, $2, 0,0,0); };

label: identifier { $$ = $1; };



expression: '(' expression ')' { $$ = $2; }
    | expression_function { $$ = $1; }
    | expression binary_operator expression { $$ = astCreate($2, 0, $1, $3, 0,0); }
    | unary_operator expression { $$ = astCreate($1, 0, $2, 0,0,0); }
    | expression_leaf { $$ = $1; };
expression_leaf: identifier optional_bracket_expression { $$ = astCreate(AST_LEAF_BRACKET_OPTIONAL, 0, $1, $2, 0,0); }
    | LIT_INTEGER { $$ = astCreate(AST_SYMBOL, $1, 0,0,0,0); }
    | LIT_CHAR { $$ = astCreate(AST_SYMBOL, $1, 0,0,0,0); }
    | KW_READ { $$ = astCreate(AST_READ, 0,0,0,0,0); };

binary_operator: '+' { $$ = AST_ADD; }
    | '-' { $$ = AST_SUB; }
    | '*' { $$ = AST_MUL; }
    | '/' { $$ = AST_DIV; }
    | OPERATOR_GE { $$ = AST_GE; }
    | OPERATOR_LE { $$ = AST_LE; }
    | OPERATOR_EQ { $$ = AST_EQ; }
    | OPERATOR_DIF { $$ = AST_DIF; }
    | '|' { $$ = AST_PIPE; }
    | '>' { $$ = AST_G; }
    | '<' { $$ = AST_L; }
    | '&' { $$ = AST_AND; };
unary_operator: '~' { $$ = AST_TILDA; };

expression_function: identifier '(' expession_function_with_or_without_arguments ')' { $$ = astCreate(AST_FUN, 0, $1, $3, 0,0); };
expession_function_with_or_without_arguments: expression_function_arguments { $$ = $1; }
    | { $$ = 0; };
expression_function_arguments: expression ',' expression_function_arguments { $$ = astCreate(AST_FUN_ARG, 0, $1, $3, 0,0); }
    | expression { $$ = $1; };


flow_control: if_block { $$ = $1; }
    | until_block { $$ = $1; }
    | come_from { $$ = $1; };
if_block: KW_IF '(' expression ')' command optional_else { $$ = astCreate(AST_IF, 0, $3, $5, $6, 0);};
optional_else: KW_ELSE command { $$ = astCreate(AST_ELSE, 0, $2, 0,0,0); }
    | { $$ = 0; };
until_block: KW_UNTIL '(' expression ')' command { $$ = astCreate(AST_UNTIL, 0, $3, $5, 0,0); };
come_from: KW_COMEFROM ':' label { $$ = astCreate(AST_COME_FROM, 0, $3, 0,0,0); };

identifier: TK_IDENTIFIER { $$ = astCreate(AST_SYMBOL, $1, 0,0,0,0); };
%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d\n", getLineNumber() );
    exit(3);
}