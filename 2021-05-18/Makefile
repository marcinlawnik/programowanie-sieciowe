.DEFAULT_GOAL := compile_all

compile_all:
	gcc -Wall ./sctpmh-client.c -o ./sctpmh-client -lsctp
	gcc -Wall ./sctpmh.c -o ./sctpmh -lsctp
	gcc -Wall ./sctpms-client.c -o ./sctpms-client -lsctp
	gcc -Wall ./sctpms-server.c -o ./sctpms-server -lsctp
	gcc -Wall ./sctpmh-server.c -o ./sctpmh-server -lsctp
	gcc -Wall ./zadanie_05_sctpmh_server_multi_stream.c -o ./zadanie_05_sctpmh_server_multi_stream -lsctp
	gcc -Wall ./zadanie_05_sctpmh_client_multi_stream.c -o ./zadanie_05_sctpmh_client_multi_stream -lsctp