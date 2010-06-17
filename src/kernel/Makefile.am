INCLUDES = -I../../include -I../include -I./

SUBDIRS = timer

noinst_LTLIBRARIES = libkernel.la

COMMON_LIBADD = \
    timer/libtimer.la

libkernel_la_LIBADD =  \
    $(COMMON_LIBADD)

libkernel_la_SOURCES = application.c signals.c object.c

EXTRA_DIST = application.c signals.c
