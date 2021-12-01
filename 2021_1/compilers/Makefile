etapa3: main.o lex.yy.o y.tab.o hash.o ast.o tacs.o
	gcc main.o lex.yy.o y.tab.o hash.o ast.c tacs.o -o etapa5

main.o: main.c
	gcc -c main.c

hash.o: hash.c
	gcc -c hash.c

tacs.o: tacs.c
	gcc -c tacs.c

ast.o: ast.c
	gcc -c ast.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: scanner.l
	lex scanner.l

clean:
	rm etapa5 y.tab.c lex.yy.c *.o
