
#Path dei files da compilare a partire dall'interno della directory BUIL
SOURCECODEBUILDPATH= ./../src/pmanager_code.c ./../src/tree.c ./../src/map.c
SOURCEPROCESSBUILDPATH= ./../src/process.c
TESTCODEPATH= ./../src/test/test.c ./../src/test/list.c

#Regole fittizie
.PHONY: help clean

#Regola di default
#Questa stampa una descrizione del progetto che contiene anche i nomi e i numeri di matricola dei membri del gruppo
help:
	@ echo ════════════════════════════════════════════════════════════════════════════════;
	@ echo Progetto Sistemi operativi 2017 - Process Manager\(1\)
	@ echo Domenico Stefani , Matteo Tadiello, Nicoló Pomini, Ivan Martini;
	@ echo 178495 - 180008 - 180058 - 180087;
	@ echo ════════════════════════════════════════════════════════════════════════════════;
	@ echo Il software permette, attraverso riga di comando, di gestire una serie di
	@ echo operazioni sui processi tra le quali: creazione di nuovi processi, clonazione di
	@ echo processi, chiusura di processi, ecc.;
	@ echo Regole disponibili:
	@ echo make build - Esegue la compilazione del codice sorgente;
	@ echo make run - Esegue il progetto in maniera corretta;
	@ echo make test - Genera un file contenente comandi casuali ed esegue il progetto
	@ echo inserendo automaticamente quei comandi;
	@ echo make assets - Genera la cartella assets usata per i file temporanei;
	@ echo make clean - Rimuove le cartelle generate dalle regole precedenti;
	@ echo ════════════════════════════════════════════════════════════════════════════════;

#Clean: elimina le eventuali directory create con le regole build e assets
clean:
	@ rm -rf build; rm -rf assets;

#Build: compila in maniera corretta il codice sorgente del processo, esegue clean di default
build: src/*.c src/*.h
	@ $(MAKE) clean;
	@ mkdir build; cd ./build; mkdir fifo;
	@ cd ./build; gcc -o processo $(SOURCEPROCESSBUILDPATH);gcc -o pmanager $(SOURCECODEBUILDPATH);gcc -o test_generator $(TESTCODEPATH);
	@ echo Eseguire il programma con il comando "make run";

#Assets: crea la cartella che deve contenere i file temporanei di supporto ed esegue build
assets:
	@ $(MAKE) build;
	@ mkdir assets;
	@ cd build; ./test_generator; mv ./test2.txt ./../assets/; gcc -o test_generator $(TESTCODEPATH);

#Test: esegue assets di default(Quindi ricorsivamente clean e build) ed esegue il programma con parametro il file di test
test:
	@ $(MAKE) assets;
	@ cd ./build; ./pmanager ./../assets/test2.txt;

#Run: esegue il programma dalla directory corretta per non avere problemi di path relativo con l'exec in pmanager_code
run:
	@ $(MAKE) build;
	@ cd build; ./pmanager;
