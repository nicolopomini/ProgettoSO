
#Path dei files da compilare a partire dall'interno della directory BUIL
SOURCECODEBUILDPATH= ./../src/pmanager_code.c ./../src/tree.c ./../src/map.c
SOURCEPROCESSBUILDPATH= ./../src/process.c
TESTCODEPATH= ./../src/test/test.c ./../src/test/list.c

#Regole fittizie
.PHONY: help clean

#Regola di default
#Questa stampa una descrizione del progetto che contiene anche i nomi e i numeri di matricola dei membri del gruppo
help:
	@ echo Domenico Stefani , Matteo Tadiello, Nicoló Pomini, Ivan Martini;
	@ echo 178495 - 180008 - 180058 - 180087;
	@ echo -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
	@ echo Il software permette, attraverso riga di comando, di gestire una serie di operazioni
	@ echo sui processi tra le quali: creazione di nuovi processi, clonazione di processi, chiusura di processi, ecc.
	@ echo I processi vengono gestiti con l ausilio di un albero centralizzato sul manager
	@ echo permettendo quindi una gestione dei processi logicamente più ordinata.
	@ echo Per compilare il codice scrivere: make build
	@ echo Per eseguirlo scrivere: make run
	@ echo Per la modalità di test scrivere: make test
	@ echo Per pulire la cartella del progetto scrivere: make clean;

#Clean: elimina le eventuali directory create con le regole build e assets
clean:
	@ rm -rf build; rm -rf assets;

#Build: compila in maniera corretta il codice sorgente del processo
build: src/*.c src/*.h src/test/*
	@ $(MAKE) clean;
	@ mkdir build; cd ./build; mkdir fifo;
	@ cd ./build; gcc -o processo $(SOURCEPROCESSBUILDPATH);gcc -o pmanager $(SOURCECODEBUILDPATH);gcc -o test_generator $(TESTCODEPATH);
	@ echo Eseguire il programma con il comando "make run";

assets:
	@ $(MAKE) build;
	@ mkdir assets;
	@ cd build; ./test_generator; mv ./test1.txt ./../assets/; gcc -o test_generator $(TESTCODEPATH);

test:
	@ $(MAKE) assets;
	@ cd ./build; ./pmanager ./../assets/test1.txt;

run:
	@ $(MAKE) build;
	@ cd build; ./pmanager;
