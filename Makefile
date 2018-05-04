MODULE_big = pg_kmcb
OBJS = pg_function.o se_std.o \
	aes.o pg_crypto.o \
	pg_rand.o  se_rand.o  \
	pg_round.o se_round.o \
	pg_string.o se_string.o simil.o\
	se_datetime.o pg_datetime.o

VRESION=1.0		
EXTENSION = pg_kmcb
DATA = pg_kmcb--1.0.sql

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/pg_kmcb
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif