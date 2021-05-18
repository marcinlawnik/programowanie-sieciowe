.DEFAULT_GOAL := compile_all

compile_all:
	gcc -Wall ./sctpmh-client.c -o ./sctpmh-client -lsctp
	gcc -Wall ./sctpmh.c -o ./sctpmh -lsctp
	gcc -Wall ./sctpms-client.c -o ./sctpms-client -lsctp
	gcc -Wall ./sctpms-server.c -o ./sctpms-server -lsctp
	gcc -Wall ./sctpmh-server.c -o ./sctpmh-server -lsctp