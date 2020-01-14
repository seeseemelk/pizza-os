include $(DEFAULTMAKE)

COPY = $(shell find . -type f -not -name makefile -print)
APPS = mcp

$(info ${COPY})
$(INITRD_TAR): $(COPY)
	$(STATUS_TAR)
	$(MKDIR)
	tar cf $@ $(COPY) $(APPS:%=$(BUILDDIR)/apps/%)
