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
prak2: display-conversions formatted-conversions data-type-sizes struct-memory union-memory

display-conversions:	display-conversions.o
	gcc $(DEBUG) -Wall -o display-conversions display-conversions.o -std=c11

display-conversions.o:	display-conversions.c
	gcc $(DEBUG) -Wall -c prak2_1.c -std=c11
	
formatted-conversions:	formatted-conversions.o
	gcc $(DEBUG) -Wall -o formatted-conversions formatted-conversions.o -std=c11

formatted-conversions.o:	formatted-conversions.c
	gcc $(DEBUG) -Wall -c formatted-conversions.c -std=c11
	
data-type-sizes:	data-type-sizes.o
	gcc $(DEBUG) -Wall -o data-type-sizes data-type-sizes.o -std=c11
	
data-type-sizes.o:	data-type-sizes.c
	gcc $(DEBUG) -Wall -c data-type-sizes.c -std=c11
	
struct-memory:	struct-memory.o
	gcc $(DEBUG) -Wall -o struct-memory struct-memory.o -std=c11	
	
struct-memory.o:	struct-memory.c
	gcc $(DEBUG) -Wall -c struct-memory.c -std=c11

union-memory:	union-memory.o
	gcc $(DEBUG) -Wall -o union-memory union-memory.o -std=c11	
	
union-memory.o:	union-memory.c
	gcc $(DEBUG) -Wall -c union-memory.c -std=c11
	
clean:
	rm -f *.o
	rm -f *~