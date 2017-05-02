SOURCECODE=../src/pmanager_code.c

.PHONY: help clean build assets test

#Regola di default
help:
	@ echo Domenco Stefani , Matteo Tadiello, Nicoló Pomini, Ivan Martini; 
	@ echo 178495 - 180008 - 180058 - 180087; 
	@ echo --Descrizione del progetto--;

clean:
	@ rm -rf build; rm -rf assets;

build: clean src/pmanager_code.c #compila sempre???
	@ mkdir build; cd build; gcc -o pmanager $(SOURCECODE);

assets: build
	@ mkdir assets;

test: assets
	@ echo Test mode is under developement
