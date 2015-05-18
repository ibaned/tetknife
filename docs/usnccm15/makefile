all: usnccm15.pdf

usnccm15.pdf: usnccm15.tex DATA
	pdflatex $^

DATA: best.out
	mpost ibanez_cse15.mp
	mpost flex.mp
	mpost mia.mp
	./getdat.sh

clean:
	rm -f *.txt *.svg *.dat *.snm *.toc *.aux
	rm -f *.log *.nav *.mpx *.png *.svg DATA
	rm -f usnccm15.out *.pdf *.vrb
