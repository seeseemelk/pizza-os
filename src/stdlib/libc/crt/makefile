include $(DEFAULTMAKE)

SRC = $(wildcard *.asm)
OBJ = $(SRC:%.asm=$(BUILDDIR)/crt/%.o)

$(CRT_OBJ): $(OBJ)
	$(STATUS_LD)
	$(LD) -i -o $@ $^

$(BUILDDIR)/crt/%.o: %.asm
	$(STATUS_KASM)
	$(MKDIR)
	$(ASM) -f elf32 -MD -o $@ $<
	
-include $(OBJ:.o=.d)
