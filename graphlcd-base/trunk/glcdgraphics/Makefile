#
# Makefile for the GraphLCD graphics library
#

-include ../Make.config

VERMAJOR = 1
VERMINOR = 0
VERMICRO = 0

BASENAME = libglcdgraphics.so

LIBNAME = $(BASENAME).$(VERMAJOR).$(VERMINOR).$(VERMICRO)

OBJS = bitmap.o common.o font.o glcd.o image.o

HEADERS = bitmap.h font.h glcd.h image.h

### Implicit rules:

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $(DEFINES) $(INCLUDES) $<

# Dependencies:

MAKEDEP = g++ -MM -MG
DEPFILE = .dependencies
$(DEPFILE): Makefile
	@$(MAKEDEP) $(DEFINES) $(INCLUDES) $(OBJS:%.o=%.c) > $@

-include $(DEPFILE)

### Targets:

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $(OBJS) $(LIBS) -Wl,-soname="$(BASENAME).$(VERMAJOR)" -o $@

install: all
	install -d $(DESTDIR)$(LIBDIR)
	install -m 755 $(LIBNAME) $(DESTDIR)$(LIBDIR)/
	install -d $(DESTDIR)$(INCDIR)/glcdgraphics
	install -m 644 $(HEADERS) $(DESTDIR)$(INCDIR)/glcdgraphics/
	( cd $(DESTDIR)$(LIBDIR); ln -sf $(LIBNAME) $(BASENAME).$(VERMAJOR); ln -sf $(LIBNAME) $(BASENAME) )

uninstall:
	rm -f $(DESTDIR)$(LIBDIR)/$(BASENAME)
	rm -f $(DESTDIR)$(LIBDIR)/$(BASENAME).$(VERMAJOR)
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME)
	(for i in $(HEADERS); do rm -f $(DESTDIR)$(INCDIR)/glcdgraphics/$$i; done)
	rmdir $(DESTDIR)$(INCDIR)/glcdgraphics

clean:
	rm -f $(OBJS) $(DEPFILE) $(LIBNAME) *~

