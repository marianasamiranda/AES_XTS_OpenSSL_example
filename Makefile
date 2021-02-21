program: example.o xts_encryption.o
	gcc -lssl -lcrypto -L/usr/local/opt/openssl/lib -o program example.o xts_encryption.o 

example.o: example.c xts_encryption.h
	gcc -c example.c  -O2 -I/usr/local/opt/openssl/include  

xts_encryption.o: xts_encryption.c xts_encryption.h
	gcc -c xts_encryption.c  -O2 -I/usr/local/opt/openssl/include 

clean:
	rm -f $(binaries) *.o
