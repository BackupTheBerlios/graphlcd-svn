#
# Makefile for the GraphLCD graphics library
#

-include ../Make.config

CXXFLAGS += -fPIC

VERMAJOR = 2
VERMINOR = 0
VERMICRO = 0

BASENAME = libglcdgraphics.so

LIBNAME = $(BASENAME).$(VERMAJOR).$(VERMINOR).$(VERMICRO)

OBJS = bitmap.o common.o font.o glcd.o image.o imagefile.o pbm.o

HEADERS = bitmap.h font.h glcd.h image.h imagefile.h pbm.h

### Implicit rules:

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $(DEFINES) $(INCLUDES) $<

# Dependencies:

MAKEDEP = g++ -MM -MG
DEPFILE = .dependencies
$(DEPFILE): Makefile
	@$(MAKEDEP) $(DEFINES) $(INCLUDES) $(OBJS:%.o=%.c) > $@

-include $(DEPFILE)

ifdef HAVE_FREETYPE2
	ifneq ($(shell which freetype-config),)
		INCLUDES += $(shell freetype-config --cflags)
		LIBS += $(shell freetype-config --libs)
	else
		INCLUDES += -I/usr/include/freetype -I/usr/local/include/freetype
		LIBS += -lfreetype
	endif
	DEFINES += -DHAVE_FREETYPE2
endif### Targets:

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $(OBJS) $(LIBS) -Wl,-soname="$(BASENAME).$(VERMAJOR)" -o $@
	ln -sf $(LIBNAME) $(BASENAME)

install: all
	install -d $(LIBDIR)
	install -m 755 $(LIBNAME) $(LIBDIR)/
	install -d $(INCDIR)/glcdgraphics
	install -m 644 $(HEADERS) $(INCDIR)/glcdgraphics/
	( cd $(LIBDIR); ln -sf $(LIBNAME) $(BASENAME).$(VERMAJOR); ln -sf $(LIBNAME) $(BASENAME) )

uninstall:
	rm -f $(LIBDIR)/$(BASENAME)
	rm -f $(LIBDIR)/$(BASENAME).$(VERMAJOR)
	rm -f $(LIBDIR)/$(LIBNAME)
	(for i in $(HEADERS); do rm -f $(INCDIR)/glcdgraphics/$$i; done)
	rmdir $(INCDIR)/glcdgraphics

clean:
	rm -f $(OBJS) $(DEPFILE) $(LIBNAME) $(BASENAME) *~

