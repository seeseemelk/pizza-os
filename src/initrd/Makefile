include $(DEFAULTMAKE)

APPS = mcp
COPY = $(shell find . -type f -not -name makefile -print) $(addprefix $(BUILDDIR)/pkg/,$(APPS)/$(APPS))

$(INITRD_TAR): $(COPY)
	$(STATUS_TAR)
	$(MKDIR)
	tar cf $@ $(COPY) --transform='s:.*/::'
