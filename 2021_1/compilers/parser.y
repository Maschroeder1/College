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

%token TK_IDENTIFIER

%token LIT_INTEGER
%token LIT_CHAR
%token LIT_STRING

%token TOKEN_ERROR

%%

init: data_section_declaration functions_block;

data_section_declaration: KW_DATA '{' data_section '}';
data_section: data_section_variable ';' data_section | ;
data_section_variable: regular_variable_intiation | vector_variable_declaration;

regular_variable_intiation: regular_variable_declaration '=' variable_literal_declaration;

regular_variable_declaration: type_declaration ':' TK_IDENTIFIER;
vector_variable_declaration: type_declaration '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER optional_vector_initialization;
optional_vector_initialization: '=' variable_literal_declaration vector_initialization | ;
vector_initialization: variable_literal_declaration vector_initialization | ;

type_declaration: KW_INT | KW_CHAR | KW_FLOAT;
variable_literal_declaration: LIT_INTEGER | LIT_CHAR;



functions_block: functions_declarations functions_block | ;
functions_declarations: type_declaration ':' TK_IDENTIFIER '(' parameter_list_or_empty ')' '{' command_block '}';
parameter_list_or_empty: parameter_list | ;
parameter_list: regular_variable_declaration ',' parameter_list | regular_variable_declaration;

command_block: command ';' command_block | ;
command: command_attribuition | command_print | return_command | flow_control |;

command_attribuition: TK_IDENTIFIER '=' expression | TK_IDENTIFIER '[' expression ']';

command_print: KW_PRINT print_elements_or_empty;
print_elements_or_empty: print_elements | ;
print_elements: print_element ',' print_elements | print_element;
print_element: LIT_STRING | expression;

return_command: KW_RETURN expression;

label: TK_IDENTIFIER;



expression: expression_leaf_adjacent | expression_branch | KW_READ | expression_function;
expression_leaf_adjacent: expression_leaf binary_operator expression 
    | expression binary_operator expression_leaf 
    | expression_leaf binary_operator expression_leaf
    | unary_operator expression_leaf | expression_leaf;
expression_leaf: TK_IDENTIFIER | TK_IDENTIFIER '[' expression ']' | LIT_INTEGER | LIT_CHAR;

binary_operator: arithmetic_operator | logical_operator | unsure_right_now_operator;
arithmetic_operator: '+' | '-' | '*' | '/';
logical_operator: OPERATOR_GE | OPERATOR_LE | OPERATOR_EQ | OPERATOR_DIF;
unsure_right_now_operator: '|' | '>' | '<' | '&';
unary_operator: '~';

expression_branch: expression binary_operator expression | unary_operator expression | '(' expression ')';
expression_function: TK_IDENTIFIER '(' expession_function_with_or_without_arguments ')';
expession_function_with_or_without_arguments: expression_function_arguments | ;
expression_function_arguments: expression ',' expression_function_arguments | expression;


flow_control: if_block | until_block | come_from;
if_block: KW_IF '(' expression ')' command optional_else;
optional_else: KW_ELSE command | ;
until_block: KW_UNTIL '(' expression ')' command;
come_from: KW_COMEFROM ':' label;

%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d\n", getLineNumber() );
    exit(3);
}