include $(BASEMAKE)
include $(PKGDIR)/mcp/include.mk

$(MCP): $(BUILDDIR)/main.o $(LIBC)
	$(STATUS_LD)
	$(MKDIR)
	$(LD) $(LDFLAGS_USER) -o $@ $<

$(BUILDDIR)/main.o: main.c
	$(STATUS_CC)
	$(MKDIR)
	$(CC) $(CFLAGS_USER) $(LIBC_CFLAGS) -MMD -c -o $@ $<
