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

decl: KW_DATA '{' data_section
    ;

data_section: data_section_variable ';' data_section
    |
    '}'
    ;

data_section_variable: regular_variable_declaration
    | vector_variable_declaration
    ;

regular_variable_declaration: variable_type_declaration ':' TK_IDENTIFIER '=' variable_literal_declaration;

vector_variable_declaration: variable_type_declaration '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER optional_vector_initialization;

optional_vector_initialization: '=' variable_literal_declaration vector_initialization | ;

vector_initialization: variable_literal_declaration vector_initialization | ;

variable_type_declaration: KW_INT | KW_CHAR | KW_FLOAT;

variable_literal_declaration: LIT_INTEGER | LIT_CHAR;

%%

int yyerror() {
    fprintf(stderr, "Syntax error at line %d\n", getLineNumber() );
    exit(3);
}