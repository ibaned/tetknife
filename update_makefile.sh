for f in *.c; do cc -MM $f >> makefile; done
