SRC = ./src
INCLUDE = ./includes
OBJ = ./obj
APPS = ./apps

all: lib app
lib:
	x86_64-w64-mingw32-gcc -c $(SRC)/arquivos.c -I $(INCLUDE) -o $(OBJ)/arquivos.o
	x86_64-w64-mingw32-gcc -c $(SRC)/tela.c -I $(INCLUDE) -c -o $(OBJ)/tela.o
app:
	x86_64-w64-mingw32-gcc $(APPS)/app.c $(OBJ)/*.o -I $(INCLUDE) -o app
run:
	./app.exe
clean:
	rm $(OBJ)/* ./*.exe