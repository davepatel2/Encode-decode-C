all: hdecode hencode

hdecode: hdecode.c
	gcc -Wall -o hdecode hdecode.c
	
hencode: hencode.c
	gcc -Wall -o hencode hencode.c

	

clean:
	rm *.o all

