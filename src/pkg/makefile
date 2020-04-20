include $(DEFAULTMAKE)
include includes.make

export CRTI := $(BUILDDIR)/libc/crti.o
export CRTN := $(BUILDDIR)/libc/crtn.o

# ==========================================
# === Start of user configurable options ===
# ==========================================
export CFLAGS_USER := $(CFLAGS) -I $(INCLUDE_LIBC)
export LDFLAGS_USER := $(LDFLAGS) -T $(PKG_DIR)/libc/linker.ld \
	-L$(BUILDDIR)/libc -L$(BUILDDIR)/libcxx --whole-archive -lc \
	--gc-sections

APPS := mcp
LIBS := libc

# ========================================
# === End of user configurable options ===
# ========================================

export PKGBUILDDIR := $(BUILDDIR)
export INCLUDES := $(abspath includes.make)
APP_EXECUTABLES := $(APPS:%=$(BUILDDIR)/%)

all: $(APPS)

mcp: libc

_always:
.PHONY: always $(APPS) $(LIBS)
$(APPS) $(LIBS): always
	+BUILDDIR=$(PKGBUILDDIR)/$(notdir $@) $(MAKE) -C $(notdir $@)

