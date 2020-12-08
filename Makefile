
luaz : require.o loading.o connect.o
	gcc -o $@ $^ -L/usr/local/lib -llua -lm -ldl -lxgci 
%.o : %.c
	gcc -g -c $<

.PHONY:clean
clean:
	rm -f *.o luaz 
