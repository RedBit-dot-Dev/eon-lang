exec = eonc
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g -Wall -lm -ldl -fPIC -rdynamic

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

#install:
#	make
#	make libeonc.a
#	mkdir -p /usr/local/include/eonc
#	cp -r ./src/include/* /usr/local/include/eonc/.
#	cp ./libeonc.a /usr/local/lib/.
#	cp ./eonc /usr/local/bin/eonc

pseudoclean:
	-rm src/*.o

clean:
	-rm eonc
#	-rm *.o
#	-rm *.a
	-rm src/*.o

lint:
	clang-tidy src/*.c /src/include/*.h