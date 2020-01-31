include $(DEFAULTMAKE)

SRC = $(patsubst ./%, %, $(shell find . -name '*.c' -print) $(shell find . -name '*.cpp' -print))
OBJ = $(SRC:%=$(BUILDDIR)/%.o)
CRTBEGIN = $(shell $(CXX) --print-file-name=crtbegin.o)
CRTEND = $(shell $(CXX) --print-file-name=crtend.o)

$(BUILDDIR)/mcp: $(OBJ) $(wildcard $(USER_COMMON)) $(wildcard $(USER_LIBC))
	$(STATUS_LD)
	$(MKDIR)
	$(LD) -o $@ $(CRTI) $(CRTBEGIN) $(LDFLAGS_USER) $(OBJ) $(CRTEND) $(CRTN)

$(BUILDDIR)/%.c.o: %.c
	$(STATUS_CC)
	$(MKDIR)
	$(CC) $(CFLAGS_USER) -MMD -c -o $@ $<
