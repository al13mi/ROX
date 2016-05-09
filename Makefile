all:
	make -f Makefile.test clean
	make -f Makefile.test
	make -f Makefile.app clean
	make -f Makefile.app
	./test
