
zsql : driver.o list.o load.o odbc.o option.o \
		pipe.o require.o xgsql.o 
	gcc -o $@ $^ -rdynamic -lluajit-5.1 -ldl -lxgci -lodbc -lodbcinst
	@rm -f *.o

%.o : %.c
	gcc -g -c $< 

.PHONY:clean
clean:
	rm -f *.o zsql 
