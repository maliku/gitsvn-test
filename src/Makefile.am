TOP_DIR=..
INCLUDES = -I$(TOP_DIR)/include

SUBDIRS = misc stdlib thread video kernel gdi
DIST_SUBDIRS = $(SUBDIRS)

lib_LTLIBRARIES = libmilui.la

COMMON_LIBADD = \
    misc/libmisc.la \
	stdlib/libstdlib.la \
	video/libvideo.la \
	thread/libthread.la\
	kernel/libkernel.la \
	gdi/libgdi.la

libmilui_la_LIBADD =  \
    $(COMMON_LIBADD)
    
libmilui_la_SOURCES = 

libmilui_la_LDFLAGS = -release $(LT_RELEASE) -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
CLEANFILES=*.a

