.DEFAULT_GOAL := compile_all

compile_all:
	gcc -Wall ./01-read-alarm.c -o ./01-read-alarm
	gcc -Wall ./02-read-select.c -o ./02-read-select
	gcc -Wall ./03-read-rcvtimeo.c -o ./03-read-rcvtimeo
	gcc -Wall ./04-read-nonblock.c -o ./04-read-nonblock
	gcc -Wall ./05-read-sigio.c -o ./05-read-sigio
	gcc -Wall ./06-read-aio.c -o ./06-read-aio -lrt
	gcc -Wall ./server.c -o ./server