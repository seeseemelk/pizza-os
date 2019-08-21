include $(DEFAULTMAKE)

SRC = $(patsubst ./%, %, $(shell find . -name '*.c' -print) $(shell find . -name '*.asm' -print))
OBJ = $(SRC:%=$(BUILDDIR)/user/%.o)

$(BUILDDIR)/libc.a: $(OBJ)
	$(STATUS_AR)
	$(MKDIR)
	$(AR) rc $@ $^

$(BUILDDIR)/user/%.c.o: %.c
	$(STATUS_KCC)
	$(MKDIR)
	$(CC) $(CFLAGS_USER) -I$(INCLUDE_LIBKC) -I. -MMD -c -o $@ $<

$(BUILDDIR)/user/%.asm.o: %.asm
	$(STATUS_ASM)
	$(MKDIR)
	$(ASM) -f elf32 -MD -o $@ $<

-include $(OBJ:.o=.d)
