build:
	gcc main.c functii.c stive.c cozi.c liste.c -o clasament
run:
	./clasament test.in test.out test.out
clean:
	rm -f clasament
