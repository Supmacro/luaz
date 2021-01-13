
target=sqluaz

VPATH=odbc:xgci
sqluaz : sl_require.o sl_load.o odbc.o xgsql.o sl_option.o sl_list.o sl_driver.o sl_pipe.o 
	gcc -o $@ $^ -rdynamic -L/usr/local/lib -lluajit-5.1 -ldl -lxgci -lodbc -lodbcinst
%.o : %.c
	gcc -g -c $<

.PHONY:clean
clean:
	rm -f *.o ${target} 
