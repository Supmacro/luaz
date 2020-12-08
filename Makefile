
luaz : lz_require.o lz_loading.o
	gcc -o $@ $^ -L/usr/local/lib -llua -lm -ldl 
%.o : %.c
	gcc -g -c $<

.PHONY:clean
clean:
	rm -f *.o luaz 
