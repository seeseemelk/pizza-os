include $(BASEMAKE)
include include.mk

SRC = $(patsubst ./%, %, $(shell find . -name '*.c' -print) $(shell find . -name '*.asm' -print))
OBJ = $(SRC:%=$(BUILDDIR)/%.o)
CRTBEGIN = $(shell $(CXX) --print-file-name=crtbegin.o)
CRTEND = $(shell $(CXX) --print-file-name=crtend.o)

$(LIBC): $(OBJ) $(wildcard $(USER_COMMON)) $(wildcard $(USER_LIBC)) \
		$(BUILDDIR)/crti.o $(BUILDDIR)/crti.o
	$(STATUS_AR)
	$(MKDIR)
	rm -f $@
	$(AR) q $@ $(OBJ)

$(BUILDDIR)/%.c.o: %.c
	$(STATUS_CC)
	$(MKDIR)
	$(CC) $(CFLAGS_USER) $(LIBC_CFLAGS) -MMD -c -o $@ $<

$(BUILDDIR)/%.asm.o: %.asm
	$(STATUS_CC)
	$(MKDIR)
	$(ASM) -f elf32 -MD -o $@ $<

$(BUILDDIR)/crti.o: crti.asm
	$(STATUS_CC)
	$(MKDIR)
	$(ASM) -f elf32 -MD -o $@ $<

$(BUILDDIR)/crtn.o: crtn.asm
	$(STATUS_CC)
	$(MKDIR)
	$(ASM) -f elf32 -MD -o $@ $<
