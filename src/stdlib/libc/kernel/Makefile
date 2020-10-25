include $(DEFAULTMAKE)

SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=$(BUILDDIR)/kernel/%.o)

$(LIBKC): $(OBJ)
	$(STATUS_AR)
	$(AR) rc $@ $^

$(BUILDDIR)/kernel/%.o: %.c
	$(STATUS_KCC)
	$(MKDIR)
	$(CC) $(CFLAGS_KERNEL) -MMD -c -o $@ $<

-include $(OBJ:.o=.d)
