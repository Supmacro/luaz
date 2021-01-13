
bin_PROGRAMS = luaz

luaz_SOURCES = sl_driver.c sl_driver.h sl_list.c sl_list.h sl_load.c sl_load.h sl_option.c \
				sl_option.h sl_pipe.c sl_pipe.h sl_require.c sl_odbc.c sl_odbc.h sl_xgsql.c \
				sl_xgsql.h

luaz_LDADD = -lluajit-5.1 -ldl -lxgci -lodbc -lodbcinst
luaz_LDFLAGS = -rdynamic

