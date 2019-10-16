include $(DEFAULTMAKE)

PIZZABUILD := $(BUILDDIR)/pizzaos

SRC = $(patsubst ./%, %, $(shell find . -name '*.c' -print) $(shell find . -name '*.cpp' -print) $(shell find . -name '*.asm' -print))
OBJ = $(SRC:%=$(PIZZABUILD)/%.o)
OBJ_TEST = $(SRC:%=$(PIZZABUILD)/%.test.o)
CRTBEGIN = $(shell $(CXX) --print-file-name=crtbegin.o)
CRTEND = $(shell $(CXX) --print-file-name=crtend.o)
LD_FLAGS = -T linker.ld -L$(BUILDDIR)/libc -L$(BUILDDIR)/libcxx --whole-archive -lkc -lkcxx 

$(PIZZAOS_ELF): $(OBJ) linker.ld
	$(STATUS_LD)
	$(MKDIR)
	$(LD) -o $@ $(LD_FLAGS) $(CRTI) $(CRTBEGIN) $(OBJ) $(CRTEND) $(CRTN)

$(PIZZAOS_TEST_ELF): $(OBJ_TEST) linker.ld
	$(STATUS_LD)
	$(MKDIR)
	$(LD) -o $@ $(LD_FLAGS) $(CRT_OBJ) $(OBJ_TEST)

$(PIZZABUILD)/%.cpp.o: %.cpp
	$(STATUS_KCXX)
	$(MKDIR)
	$(CXX) $(CXXFLAGS_KERNEL) -MMD -I$(INCLUDE_LIBKCXX) -I$(INCLUDE_LIBKC) -I. -c -o $@ $<

$(PIZZABUILD)/%.cpp.test.o: %.cpp
	$(STATUS_KCXX)
	$(MKDIR)
	$(CXX) $(CXXFLAGS_KERNEL) -MMD -I$(INCLUDE_LIBKCXX) -I$(INCLUDE_LIBKC) -I. -DTESTING=1 -c -o $@ $<
	
$(PIZZABUILD)/%.asm.o: %.asm
	$(STATUS_KASM)
	$(MKDIR)
	$(ASM) -f elf32 -MD -o $@ $<
	
$(PIZZABUILD)/%.asm.test.o: %.asm
	$(STATUS_KASM)
	$(MKDIR)
	$(ASM) -f elf32 -MD -DTESTING=1 -o $@ $<

-include $(OBJ:.o=.d)
