Source = src/pbc_inshort.c jianhong-zhang.c 
Target = out
Object = pbc_inshort.o jianhong-zhang.o

DIR_I = /usr/local/include/pbc
DIR_L = /usr/local/lib
FLAG = -I $(DIR_I) -L $(DIR_L) -Wl,-rpath $(DIR_L) -lpbc -lgmp

$(Target):$(Object)
	gcc -o $@ $^ $(FLAG)
	@echo "Succeed!"

.c.o:
	gcc -c $(Source) $(FLAG)
clean:
	rm $(Target)
	rm $(Object)
