#
# Makefile for the GraphLCD graphics library
#

-include ../Make.config


LIBNAME = libglcdgraphics.so

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
	$(CXX) $(CXXFLAGS) -shared $(OBJS) $(LIBS) -o $@

install: all
	install -d $(DESTDIR)$(LIBDIR)
	install -m 644 $(LIBNAME) $(DESTDIR)$(LIBDIR)/
	install -d $(DESTDIR)$(INCDIR)/glcdgraphics
	install -m 644 $(HEADERS) $(DESTDIR)$(INCDIR)/glcdgraphics/
	$(LDCONFIG)

uninstall:
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME)
	(for i in $(HEADERS); do rm -f $(DESTDIR)$(INCDIR)/glcdgraphics/$$i; done)
	rmdir $(DESTDIR)$(INCDIR)/glcdgraphics
	$(LDCONFIG)

clean:
	rm -f $(OBJS) $(DEPFILE) *.so *.a *~

