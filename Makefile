test: a.out
	./a.out

a.out: main.c
	gcc $< -o $@
