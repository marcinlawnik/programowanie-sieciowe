.DEFAULT_GOAL := compile_all

compile_all:
	gcc -Wall ./bpffilter.c -o ./bpffilter
	gcc -Wall ./pcapfilter.c -o ./pcapfilter -lpcap
	gcc -Wall ./pcapsniff.c -o ./pcapsniff -lpcap