
#Path dei files da compilare a partire dall'interno della directory BUIL
SOURCECODEBUILDPATH= ./../src/pmanager_code.c ./../src/tree.c ./../src/map.c
SOURCEPROCESSBUILDPATH= ./../src/process.c

#Regole fittizie
.PHONY: help clean

#Regola di default
#Questa stampa una descrizione del progetto che contiene anche i nomi e i numeri di matricola dei membri del gruppo
help:
	@ echo Domenico Stefani , Matteo Tadiello, Nicoló Pomini, Ivan Martini;
	@ echo 178495 - 180008 - 180058 - 180087;
	@ echo --Descrizione del progetto--;

#Clean: elimina le eventuali directory create con le regole build e assets
clean:
	@ rm -rf build; rm -rf assets;

#Build: compila in maniera corretta il codice sorgente del processo
build: src/*.c src/*.h
	@ $(MAKE) clean;
	@ mkdir build; cd build; gcc -o processo $(SOURCEPROCESSBUILDPATH); gcc -o pmanager $(SOURCECODEBUILDPATH);
	@ echo Eseguire il programma con il comando "make run";

assets:
	@ $(MAKE) build
	@ mkdir assets;

test:
	@ $(MAKE) assets
	@ echo Test mode is under developement

run:
	@ cd build; ./pmanager;
