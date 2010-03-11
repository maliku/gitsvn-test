TOP_DIR=..
INCLUDES = -I$(TOP_DIR)/include

SUBDIRS = misc stdlib
DIST_SUBDIRS = $(SUBDIRS)

lib_LTLIBRARIES = libmilui.la

COMMON_LIBADD =               \
    misc/libmisc.la \
	stdlib/libstdlib.la

libmilui_la_LIBADD =  \
    $(COMMON_LIBADD)
    
libmilui_la_SOURCES = 

libmilui_la_LDFLAGS = -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
CLEANFILES=*.a

