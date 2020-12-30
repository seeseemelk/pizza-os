include $(wildcard */Makefile.include)

export CRTI := $(BUILDDIR)/libc/crti.o
export CRTN := $(BUILDDIR)/libc/crtn.o

# ==========================================
# === Start of user configurable options ===
# ==========================================
export CFLAGS_USER := $(CFLAGS) -I $(LIBC_INCLUDE)
export LDFLAGS_USER := $(LDFLAGS) -T $(PKGDIR)/libc/linker.ld \
	-L$(BUILDDIR)/libc -L$(BUILDDIR)/libcxx --whole-archive -lc \
	--gc-sections

BUILD := mcp

# ========================================
# === End of user configurable options ===
# ========================================
export PKGBUILDDIR := $(BUILDDIR)
APP_EXECUTABLES := $(APPS:%=$(BUILDDIR)/%)

all: $(BUILD)

# Program dependencies
mcp: libc
pizzaos:
libc:
# End of program dependencies

_always:
.PHONY: $(BUILD)
$(BUILD):
	+BUILDDIR=$(PKGBUILDDIR)/$(notdir $@) $(MAKE) -C $(notdir $@)

