#
# Makefile for the GraphLCD driver library
#

-include ../Make.config


LIBNAME = libglcddrivers.so

OBJS = common.o config.o driver.o drivers.o port.o simlcd.o framebuffer.o gu140x32f.o gu256x64-372.o gu256x64-3900.o hd61830.o ks0108.o image.o sed1330.o sed1520.o t6963c.o noritake800.o serdisp.o

HEADERS = config.h driver.h drivers.h port.h simlcd.h framebuffer.h gu140x32f.h gu256x64-372.h gu256x64-3900.h hd61830.h ks0108.h image.h sed1330.h sed1520.h t6963c.h noritake800.h serdisp.h


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
	$(CXX) $(CXXFLAGS) -shared $(OBJS) $(LIBS) -ldl -o $@

install: all
	install -d $(DESTDIR)$(LIBDIR)
	install -m 644 $(LIBNAME) $(DESTDIR)$(LIBDIR)/
	install -d $(DESTDIR)$(INCDIR)/glcddrivers
	install -m 644 $(HEADERS) $(DESTDIR)$(INCDIR)/glcddrivers/
	$(LDCONFIG)

uninstall:
	rm -f $(DESTDIR)$(LIBDIR)/$(LIBNAME)
	(for i in $(HEADERS); do rm -f $(DESTDIR)$(INCDIR)/glcddrivers/$$i; done)
	rmdir $(DESTDIR)$(INCDIR)/glcddrivers
	$(LDCONFIG)

clean:
	rm -f $(OBJS) $(DEPFILE) *.so *.a *~

