include $(DEFAULTMAKE)

SRC = $(wildcard *.asm)
OBJ = $(SRC:%.asm=$(BUILDDIR)/crt/%.o)

all: $(OBJ)

$(BUILDDIR)/crt/%.o: %.asm
	$(STATUS_KASM)
	$(MKDIR)
	$(ASM) -f elf32 -MD -o $@ $<
	
-include $(OBJ:.o=.d)
