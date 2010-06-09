INCLUDES = -I../../include -I../include -I../

SUBDIRS = paint_engine

noinst_LTLIBRARIES = libgdi.la

COMMON_LIBADD = \
    paint_engine/libpaint_engine.la

libgdi_la_LIBADD =  \
    $(COMMON_LIBADD)

libgdi_la_SOURCES = blit.c RLEaccel.c bmp.c pixels.c blit_0.c blit_1.c blit_N.c blit_A.c rect.c stretch.c \
					image.c bitmap.c gc.c img_loader.c gdi_base.c pixels_ops.c paint.c shape.c paint_device.c \
					raster_surface.c

EXTRA_DIST = blit.c RLEaccel.c bmp.c pixels.c blit_0.c blit_1.c blit_N.c blit_A.c rect.c stretch.c \
			 image.c bitmap.c gc.c img_loader.c gdi_base.c pixels_ops.c paint.c shape.c paint_device.c \
			 raster_surface.c
