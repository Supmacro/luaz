
VPATH=odbc:xgci
luaz : require.o load.o odbc.o xgsql.o option.o dblist.o driver.o io.o 
	gcc -o $@ $^ -rdynamic -L/usr/local/lib -lluajit-5.1 -ldl -lxgci -lodbc -lodbcinst
%.o : %.c
	gcc -g -c $<

.PHONY:clean
clean:
	rm -f *.o luaz 
