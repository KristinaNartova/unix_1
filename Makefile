
INC=includes
OUT=bin
SRC=src

$(OUT)/signalTest: $(OUT)/main.o $(OUT)/dokill.o $(OUT)/dochild.o $(OUT)/doposix.o $(OUT)/dopipe.o $(OUT)/dostd.o
	gcc -o $(OUT)/signalTest $(OUT)/main.o $(OUT)/dokill.o $(OUT)/dochild.o $(OUT)/doposix.o $(OUT)/dopipe.o $(OUT)/dostd.o

$(OUT)/main.o: $(SRC)/main.c $(INC)/handlers.h
	gcc -I$(INC) -o $(OUT)/main.o -c $(SRC)/main.c

$(OUT)/dostd.o: $(SRC)/dostd.c $(INC)/handlers.h
	gcc -I$(INC) -o $(OUT)/dostd.o -c $(SRC)/dostd.c

$(OUT)/dochild.o: $(SRC)/dochild.c $(INC)/handlers.h
	gcc -I$(INC) -o $(OUT)/dochild.o -c $(SRC)/dochild.c

$(OUT)/doposix.o: $(SRC)/doposix.c $(INC)/handlers.h
	gcc -I$(INC) -o $(OUT)/doposix.o -c $(SRC)/doposix.c -g -O0 

$(OUT)/dokill.o: $(SRC)/dokill.c $(INC)/handlers.h
	gcc -I$(INC) -o $(OUT)/dokill.o -c $(SRC)/dokill.c

$(OUT)/dopipe.o: $(SRC)/dopipe.c $(INC)/handlers.h
	gcc -I$(INC) -o $(OUT)/dopipe.o -c $(SRC)/dopipe.c

clean:
	rm  $(OUT)/*;