filtro: filtro.c energia.c
	gcc -O0 filtro.c energia.c -o filtroO0
	gcc -O2 filtro.c energia.c -o filtroO2
	gcc -Ofast filtro.c energia.c -o filtroOfast

clean:
	rm filtroO0 filtroO2 filtroOfast