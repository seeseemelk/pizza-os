include $(DEFAULTMAKE)
include $(PKGDIR)/libc/include.make

$(BUILDDIR)/mcp: $(BUILDDIR)/main.o $(LIBC_OBJ)
	$(STATUS_LD)
	$(MKDIR)
	$(LD) $(LDFLAGS_USER) -o $@ $<

$(BUILDDIR)/main.o: main.c
	$(STATUS_CC)
	$(MKDIR)
	$(CC) $(CFLAGS_USER) -MMD -c -o $@ $<
