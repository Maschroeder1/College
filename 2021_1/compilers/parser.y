%{
    // trabalho de Mikael Arnez schroeder
    #include <stdlib.h>
    #include "hash.h"
    #include "ast.h"
    #include "tacs.h"
    
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
%type<ast> expession_function_with_or_without_arguments init functions_block functions_declarations
%type<ast> parameter_list_or_empty parameter_list data_section_declaration data_section data_section_variable
%type<ast> regular_variable_intiation regular_variable_declaration vector_initialization
%type<ast> vector_variable_declaration vector_range optional_vector_initialization variable_literal_declaration
%type<ast> lit_char lit_int

%left '~'
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF OPERATOR_RANGE '<' '>'
%left '+' '-'
%left '*' '/'

%%

start_here: init { globalAst = $1; TAC* temp = generateCode($1); tacPrintBackwards(temp); }

init: data_section_declaration functions_block { $$ = astCreate(AST_INIT, 0, $1, $2, 0,0); };

data_section_declaration: KW_DATA '{' data_section '}' { $$ = astCreate(AST_DATA_SECT, 0, $3, $3,0,0); };
data_section: data_section_variable ';' data_section { $$ = astCreate(AST_DATA_SECT, 0, $1, $3, 0,0); }
    | { $$ = 0; };
data_section_variable: regular_variable_intiation { $$ = $1; }
    | vector_variable_declaration { $$ = $1; };

regular_variable_intiation: regular_variable_declaration '=' variable_literal_declaration { $$ = astCreate(AST_VAR, 0, $1, $3, 0,0); };

regular_variable_declaration: KW_INT ':' identifier { $$ = astCreate(AST_INT, 0, $3, 0,0,0); }
    | KW_CHAR ':' identifier { $$ = astCreate(AST_CHAR, 0, $3, 0,0,0); }
    | KW_FLOAT ':' identifier { $$ = astCreate(AST_FLOAT, 0, $3, 0,0,0); };
vector_variable_declaration: KW_INT '[' vector_range ']' ':' identifier optional_vector_initialization { $$ = astCreate(AST_VET_INT, 0, $3, $6, $7, 0); }
    | KW_CHAR '[' vector_range ']' ':' identifier optional_vector_initialization { $$ = astCreate(AST_VET_CHAR, 0, $3, $6, $7, 0); }
    | KW_FLOAT '[' vector_range ']' ':' identifier optional_vector_initialization { $$ = astCreate(AST_VET_FLOAT, 0, $3, $6, $7, 0); };
vector_range: lit_int OPERATOR_RANGE lit_int { $$ = astCreate(AST_VET_RANGE, 0, $1, $3, 0,0); };
optional_vector_initialization: '=' variable_literal_declaration vector_initialization { $$ = astCreate(AST_VET_INIT, 0, $2, $3, 0,0); }
    | { $$ = 0; };
vector_initialization: variable_literal_declaration vector_initialization { $$ = astCreate(AST_VET_INIT, 0, $1, $2, 0,0); }
    | { $$ = 0; };

variable_literal_declaration: lit_int { $$ = $1; }
    | lit_char { $$ = $1; };



functions_block: functions_declarations functions_block { $$ = astCreate(AST_FUN_BLOCK, 0, $1, $2, 0,0); }
    | { $$ = 0; };
functions_declarations: KW_INT ':' identifier '(' parameter_list_or_empty ')' '{' command_block '}' { $$ = astCreate(AST_FUN_INT, 0, $3, $5, $8, 0); }
    | KW_CHAR ':' identifier '(' parameter_list_or_empty ')' '{' command_block '}' { $$ = astCreate(AST_FUN_CHAR, 0, $3, $5, $8, 0); }
    | KW_FLOAT ':' identifier '(' parameter_list_or_empty ')' '{' command_block '}' { $$ = astCreate(AST_FUN_FLOAT, 0, $3, $5, $8, 0); };
parameter_list_or_empty: parameter_list { $$ = $1; }
    | { $$ = 0; };
parameter_list: regular_variable_declaration ',' parameter_list { $$ = astCreate(AST_FUN_DEC_PARAM, 0, $1, $3, 0,0); }
    | regular_variable_declaration { $$ = $1; };

command_block: command ';' command_block { $$ = astCreate(AST_CMD_BLOCK, 0, $1, $3, 0,0); }
    | { $$ = 0; };
command: command_attribuition { $$ = $1; }
    | command_print { $$ = $1; }
    | return_command { $$ = $1; }
    | flow_control { $$ = $1; }
    | '{' command_block '}' { $$ = astCreate(AST_CMD_CURLY, 0, $2, 0,0,0); }
    | label { $$ = $1; }
    | { $$ = 0; };

command_attribuition: identifier optional_bracket_expression '=' expression { $$ = astCreate(AST_ATTR, 0, $1, $4, $2,0); }
optional_bracket_expression: '[' expression ']' { $$ = astCreate(AST_BRACKET_EXPR, 0, $2, 0,0,0); }
    | { $$ = 0; };

command_print: KW_PRINT print_elements_or_empty { $$ = astCreate(AST_PRINT, 0, $2, 0,0,0); };
print_elements_or_empty: print_elements { $$ = $1; }
    | { $$ = 0; } ;
print_elements: print_element ',' print_elements { $$ = astCreate(AST_PRINT_BLOCK, 0, $1, $3, 0,0); }
    | print_element { $$ = $1; };
print_element: LIT_STRING { $$ = astCreate(AST_SYMBOL, $1, 0,0,0,0); }
    | expression { $$ = $1; };

return_command: KW_RETURN expression { $$ = astCreate(AST_RETURN, 0, $2, 0,0,0); };

label: identifier { $$ = $1; };



expression: '(' expression ')' { $$ = astCreate(AST_EXPR_PARENT, 0, $2, 0,0,0); }
    | expression_function { $$ = $1; }
    | expression '+' expression { $$ = astCreate(AST_ADD, 0, $1, $3, 0,0); }
    | expression '-' expression { $$ = astCreate(AST_SUB, 0, $1, $3, 0,0); }
    | expression '*' expression { $$ = astCreate(AST_MUL, 0, $1, $3, 0,0); }
    | expression '/' expression { $$ = astCreate(AST_DIV, 0, $1, $3, 0,0); }
    | expression OPERATOR_GE expression { $$ = astCreate(AST_GE, 0, $1, $3, 0,0); }
    | expression OPERATOR_LE expression { $$ = astCreate(AST_LE, 0, $1, $3, 0,0); }
    | expression OPERATOR_EQ expression { $$ = astCreate(AST_EQ, 0, $1, $3, 0,0); }
    | expression OPERATOR_DIF expression { $$ = astCreate(AST_DIF, 0, $1, $3, 0,0); }
    | expression '|' expression { $$ = astCreate(AST_PIPE, 0, $1, $3, 0,0); }
    | expression '>' expression { $$ = astCreate(AST_G, 0, $1, $3, 0,0); }
    | expression '<' expression { $$ = astCreate(AST_L, 0, $1, $3, 0,0); }
    | expression '&' expression { $$ = astCreate(AST_AND, 0, $1, $3, 0,0); }
    | '~' expression { $$ = astCreate(AST_TILDA, 0, $2, 0,0,0); }
    | expression_leaf { $$ = $1; };
expression_leaf: identifier optional_bracket_expression { $$ = astCreate(AST_LEAF_BRACKET_OPTIONAL, 0, $1, $2, 0,0); }
    | lit_int { $$ = $1; }
    | lit_char { $$ = $1; }
    | KW_READ { $$ = astCreate(AST_READ, 0,0,0,0,0); };

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
lit_int: LIT_INTEGER { $$ = astCreate(AST_SYMBOL, $1, 0,0,0,0); }
lit_char: LIT_CHAR { $$ = astCreate(AST_SYMBOL, $1, 0,0,0,0); }
%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d\n", getLineNumber() );
    exit(3);
}