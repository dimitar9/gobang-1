all:
	gcc -g main.c  gobang.c ui.c -o chess 
	gcc -g black.c gobang.c ui.c -o black
	gcc -g white.c gobang.c ui.c -o white
