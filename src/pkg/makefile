include $(DEFAULTMAKE)
include libc/include.make

export CRTI := $(BUILDDIR)/libc/crti.o
export CRTN := $(BUILDDIR)/libc/crtn.o

# ==========================================
# === Start of user configurable options ===
# ==========================================
export CFLAGS_USER := $(CFLAGS) -I $(LIBC_INCLUDE)
export LDFLAGS_USER := $(LDFLAGS) -T $(PKGDIR)/libc/linker.ld \
	-L$(BUILDDIR)/libc -L$(BUILDDIR)/libcxx --whole-archive -lc \
	--gc-sections

APPS := mcp
LIBS := libc

# ========================================
# === End of user configurable options ===
# ========================================
export PKGBUILDDIR := $(BUILDDIR)
APP_EXECUTABLES := $(APPS:%=$(BUILDDIR)/%)

all: $(APPS)

mcp: libc

_always:
.PHONY: always $(APPS) $(LIBS)
$(APPS) $(LIBS): always
	+BUILDDIR=$(PKGBUILDDIR)/$(notdir $@) $(MAKE) -C $(notdir $@)

