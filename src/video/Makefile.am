INCLUDES = -I../../include -I../include

SUBDIRS = dummy qvfb

noinst_LTLIBRARIES = libvideo.la

COMMON_LIBADD = \
    dummy/libdummyvideo.la \
    qvfb/libqvfbvideo.la

libvideo_la_LIBADD =  \
    $(COMMON_LIBADD)

libvideo_la_SOURCES = video_device.c surface.c

