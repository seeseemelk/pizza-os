export CRTI := $(BUILDDIR)/libc/crti.o
export CRTN := $(BUILDDIR)/libc/crtn.o

# ==========================================
# === Start of user configurable options ===
# ==========================================
export CFLAGS_USER := $(CFLAGS) -I $(LIBC_INCLUDE)
export LDFLAGS_USER := $(LDFLAGS) -T $(PKGDIR)/libc/linker.ld \
	-L$(BUILDDIR)/libc -L$(BUILDDIR)/libcxx --whole-archive -lc \
	--gc-sections

# ========================================
# === End of user configurable options ===
# ========================================
PKG_INCLUDES = $(wildcard */package.mk)
PKGS = $(PKG_INCLUDES:%/package.mk=%)
include $(PKG_INCLUDES)

export PKGBUILDDIR := $(BUILDDIR)
APP_EXECUTABLES := $(APPS:%=$(BUILDDIR)/%)

all: $(PKGS)

# Program dependencies
#mcp: libc
#pizzaos:
#libc:
# End of program dependencies

#_always:
#.PHONY: $(BUILD)
#$(BUILD):
#	+BUILDDIR=$(PKGBUILDDIR)/$(notdir $@) $(MAKE) -C $(notdir $@)

#pkg_%:
#	+BUILDDIR=$(PKGBUILDDIR)/$(notdir $@) $(MAKE) -C $($(notdir $@):build_%=%)

define BUILD_PACKAGE =
$(info Target added for $1)
pkg_$1:
	$(MAKE) BUILDDIR=$(PKGBUILDDIR)/$1 -C $1
endef

$(foreach PKG,$(PKGS),$(eval $(call BUILD_PACKAGE,$(PKG))))
