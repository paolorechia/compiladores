$DEPURA=1

compilador: lex.yy.c y.tab.c compilador.o compilador.h tabela_simbolos.c tabela_simbolos.h
	gcc lex.yy.c compilador.tab.c compilador.o tabela_simbolos.c -o compilador -ll -ly -lc

lex.yy.c: compilador.l compilador.h 
	flex compilador.l

y.tab.c: compilador.y compilador.h
	bison compilador.y -d -v 

compilador.o : compilador.h compiladorF.c
	gcc -c compiladorF.c -o compilador.o

tabela_simbolos_test: tabela_simbolos_test.c tabela_simbolos.o tabela_simbolos.h
	gcc tabela_simbolos.o tabela_simbolos_test.c -o tabela_simbolos_test

tabela_simbolos.o: tabela_simbolos.c tabela_simbolos.h
	gcc -c tabela_simbolos.c -o tabela_simbolos.o

clean : 
	rm -f compilador.tab.* lex.yy.c  *.o compilador compilador.output tabela_simbolos_test
