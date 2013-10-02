main: main.c
	gcc main.c -O4 -pthread -o main

seq: seq.c
	gcc seq.c -O4 -o seq

test:
	bash test_scr

report: report.tex
	texi2pdf report.tex
	open report.pdf
