Source = src/jianhong-zhang.c src/pbc_inshort.c main.c
Target = out
Object = jianhong-zhang.o pbc_inshort.o main.o

DIR_I = /usr/local/include/pbc
DIR_L = /usr/local/lib
FLAG = -I $(DIR_I) -L $(DIR_L) -Wl,-rpath $(DIR_L) -lpbc -lgmp

$(Target):$(Object)
	gcc -o $@ $^ $(FLAG)
	@echo "Succeed!"

$(Object):	$(Source) 
	gcc -c $^ $(FLAG)

clean:
	rm $(Target)
	rm $(Object)
