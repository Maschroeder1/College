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

programa: decl
    ;

decl: data_section_declaration expression
    ;

data_section_declaration: KW_DATA '{' data_section '}';

data_section: data_section_variable ';' data_section ;

data_section_variable: regular_variable_intiation | vector_variable_declaration;

regular_variable_intiation: regular_variable_declaration '=' variable_literal_declaration;

regular_variable_declaration: type_declaration ':' TK_IDENTIFIER;

vector_variable_declaration: type_declaration '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER optional_vector_initialization;

optional_vector_initialization: '=' variable_literal_declaration vector_initialization | ;

vector_initialization: variable_literal_declaration vector_initialization | ;

type_declaration: KW_INT | KW_CHAR | KW_FLOAT;

variable_literal_declaration: LIT_INTEGER | LIT_CHAR;

functions_declarations: type_declaration ':' TK_IDENTIFIER '(' parameter_list ')' '{' command_block '}';

parameter_list: regular_variable_declaration parameter_list | ;

command_block: command ';' command_block | ;

command: attribuition | ;

attribuition: ;

expression: expression_leaf_adjacent | expression_branch | KW_READ;

expression_leaf_adjacent: expression_leaf binary_operator expression 
    | expression binary_operator expression_leaf 
    | expression_leaf binary_operator expression_leaf
    | unary_operator expression_leaf | expression_leaf;

expression_leaf: TK_IDENTIFIER | TK_IDENTIFIER '[' expression ']' | LIT_INTEGER | LIT_CHAR;

binary_operator: arithmetic_operator | logical_operator | unknown_operator;

arithmetic_operator: '+' | '-' | '*' | '/';

logical_operator: OPERATOR_GE | OPERATOR_LE | OPERATOR_EQ | OPERATOR_DIF;

unknown_operator: '|' | '>' | '<' | '&';

unary_operator: '~';

expression_branch: expression binary_operator expression | unary_operator expression | '(' expression ')';

%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d\n", getLineNumber() );
    exit(3);
}