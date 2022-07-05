#Option zum erstellen mit
#Debug-Informationen für GDB
#Zeile 4 auskommentieren:
//DEBUG = -g

#Option zum Löschen von Objekt-
#und AutoSave-Dateien: 
#: make clean
.PHONY: clean

#Binary erstellen:
#: make
prak2: 01-input-data-types 02-input-data-forms 03-data-type-size 04-structs 05-union

01-input-data-types:	01-input-data-types.o
	gcc $(DEBUG) -Wall -o 01-input-data-types 01-input-data-types.o -std=c11

01-input-data-types.o:	01-input-data-types.c
	gcc $(DEBUG) -Wall -c prak2_1.c -std=c11
	
02-input-data-forms:	02-input-data-forms.o
	gcc $(DEBUG) -Wall -o 02-input-data-forms 02-input-data-forms.o -std=c11

02-input-data-forms.o:	02-input-data-forms.c
	gcc $(DEBUG) -Wall -c 02-input-data-forms.c -std=c11
	
03-data-type-size:	03-data-type-size.o
	gcc $(DEBUG) -Wall -o 03-data-type-size 03-data-type-size.o -std=c11
	
03-data-type-size.o:	03-data-type-size.c
	gcc $(DEBUG) -Wall -c 03-data-type-size.c -std=c11
	
04-structs:	04-structs.o
	gcc $(DEBUG) -Wall -o 04-structs 04-structs.o -std=c11	
	
04-structs.o:	04-structs.c
	gcc $(DEBUG) -Wall -c 04-structs.c -std=c11

05-union:	05-union.o
	gcc $(DEBUG) -Wall -o 05-union 05-union.o -std=c11	
	
05-union.o:	05-union.c
	gcc $(DEBUG) -Wall -c 05-union.c -std=c11
	
clean:
	rm -f *.o
	rm -f *~