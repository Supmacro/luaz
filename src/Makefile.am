
bin_PROGRAMS = luaz

luaz_SOURCES = lz_driver.c lz_driver.h lz_list.c lz_list.h lz_load.c lz_load.h lz_option.c \
				lz_option.h lz_pipe.c lz_pipe.h lz_require.c lz_odbc.c lz_odbc.h lz_xgsql.c \
				lz_xgsql.h

luaz_LDADD = -lluajit-5.1 -ldl -lxgci -lodbc -lodbcinst
luaz_LDFLAGS = -rdynamic

