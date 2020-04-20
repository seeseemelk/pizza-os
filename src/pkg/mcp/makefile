include $(DEFAULTMAKE)
include $(INCLUDES)

$(BUILDDIR)/mcp: $(BUILDDIR)/main.o $(PKGBUILDDIR)/libc/libc.a
	$(STATUS_LD)
	$(MKDIR)
	$(LD) $(LDFLAGS_USER) -o $@ $<

$(BUILDDIR)/main.o: main.c
	$(STATUS_CC)
	$(MKDIR)
	$(CC) $(CFLAGS_USER) -MMD -c -o $@ $<
