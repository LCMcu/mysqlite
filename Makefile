#test:test.c
#	gcc -o test test.c -lsqlite3


LDFLAGS+=-lsqlite3
OBJ=mysqlite
SRC=mysqlite.c

$(OBJ):$(SRC)
	gcc -o $@  $^ $(LDFLAGS)
	
lib:
	gcc $(SRC)  -fPIC -shared -o libmysqlite.so
	cp ./libmysqlite.so ../mylib

clean: 
	rm ./$(OBJ)