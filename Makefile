
all:
	gcc abuelo.c -o abuelo

run: all
	./abuelo

.PHONY: clean

clean:
	rm -f *.o abuelo
