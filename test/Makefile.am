noinst_PROGRAMS=inherit object virtual duplicator rwops performance

MILUI_LIBS=-lmilui -ljpeg -lpng -lpthread

inherit_SOURCES=inherit.c
inherit_LDADD=$(MILUI_LIBS)

object_SOURCES=object.c
object_LDADD=$(MILUI_LIBS)

virtual_SOURCES=virtual.c
virtual_LDADD=$(MILUI_LIBS)

duplicator_SOURCES=duplicator.c
duplicator_LDADD=$(MILUI_LIBS)

rwops_SOURCES=rwops_test.c
rwops_LDADD=$(MILUI_LIBS)

performance_SOURCES=performance.c
performance_LDADD=$(MILUI_LIBS)
