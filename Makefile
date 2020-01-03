#test:test.c
#	gcc -o test test.c -lsqlite3


LDFLAGS+=-lsqlite3
OBJ=mysqlite
SRC=mysqlite.c

$(OBJ):$(SRC)
	gcc -o $@  $^ $(LDFLAGS)

clean: 
	rm ./$(OBJ)