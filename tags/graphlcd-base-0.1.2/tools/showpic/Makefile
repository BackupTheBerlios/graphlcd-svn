#
# Makefile for the GraphLCD tool showpic
#

-include ../../Make.config

PRGNAME = showpic

OBJS = showpic.o

INCLUDES += -I../../
LIBDIRS += -L../../glcdgraphics/ -L../../glcddrivers/


all: $(PRGNAME)
.PHONY: all

# Implicit rules:

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $(DEFINES) $(INCLUDES) $<

# Dependencies:

MAKEDEP = $(CXX) -MM -MG
DEPFILE = .dependencies
$(DEPFILE): Makefile
	@$(MAKEDEP) $(DEFINES) $(INCLUDES) $(OBJS:%.o=%.c) > $@

-include $(DEPFILE)

# The main program:

$(PRGNAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -rdynamic $(OBJS) $(LIBS) $(LIBDIRS) -lglcdgraphics -lglcddrivers -lstdc++ -o $(PRGNAME)

install: $(PRGNAME)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 -o root -g root -s $(PRGNAME) $(DESTDIR)$(BINDIR)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(PRGNAME)

clean:
	@-rm -f $(OBJS) $(DEPFILE) $(PRGNAME) *~

