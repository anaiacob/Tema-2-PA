build:
	gcc main.c functii.c -o clasament
run:
	./clasament test.in test.out test.out
clean:
	rm -f clasament
