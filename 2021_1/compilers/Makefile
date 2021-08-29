etapa2: y.tab.c lex.yy.c
	gcc lex.yy.c -o etapa2

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm etapa2 y.tab.c lex.yy.c
