program: example.o xts_encryption.o
	gcc  -I/usr/local/opt/openssl/include  -lssl -lcrypto -o program example.o xts_encryption.o -L/usr/local/opt/openssl/lib

example.o: example.c xts_encryption.h
	gcc -c example.c  -O2 -I/usr/local/opt/openssl/include  

xts_encryption.o: xts_encryption.c xts_encryption.h
	gcc -c xts_encryption.c  -O2 -I/usr/local/opt/openssl/include 

clean:
	rm -f $(binaries) *.o