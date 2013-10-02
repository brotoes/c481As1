main: main.c
	gcc main.c -O4 -pthread -o main

seq: seq.c
	gcc seq.c -O4 -o seq

test:
	bash test_scr

report: report.tex
	rm report.pdf
	latex2pdf report.tex
	evince report.pdf
