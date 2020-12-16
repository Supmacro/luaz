
luaz : require.o load.o odbc.o xgsql.o rows.o dlist.o 
	gcc -o $@ $^ -L/usr/local/lib -llua -lm -ldl -lxgci -lodbc -lodbcinst
%.o : %.c
	gcc -g -c $<

.PHONY:clean
clean:
	rm -f *.o luaz 
