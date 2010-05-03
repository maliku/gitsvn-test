INCLUDES = -I../../include -I../include

SUBDIRS = pthread win32 dummy
#DIST_SUBDIRS = $(SUBDIRS)

noinst_LTLIBRARIES = libthread.la

COMMON_LIBADD = \
	dummy/libdummymutex.la \
	pthread/libpthreadmutex.la \
	win32/libwin32_mutex.la

libthread_la_LIBADD =  \
    $(COMMON_LIBADD)

libthread_la_SOURCES = MIL_mutex.c

