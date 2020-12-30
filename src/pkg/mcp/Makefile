include $(BASEMAKE)
include $(PKGDIR)/mcp/Makefile.include

$(MCP): $(BUILDDIR)/main.o $(LIBS)
	$(STATUS_LD)
	$(MKDIR)
	$(LD) $(LDFLAGS_USER) -o $@ $<

$(BUILDDIR)/main.o: main.c
	$(STATUS_CC)
	$(MKDIR)
	$(CC) $(CFLAGS_USER) -MMD -c -o $@ $<
