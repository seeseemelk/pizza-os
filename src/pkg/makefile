include $(DEFAULTMAKE)

APPS := mcp
LIBS := libc

APP_EXECUTABLES := $(APPS:%=$(BUILDDIR)/%)

all: $(APPS)

_ignore:
.PHONY: _ignore
$(APPS) $(LIBS): _ignore
	+BUILDDIR=$(BUILDDIR)/$(notdir $@) $(MAKE) -C $(notdir $@) $(BUILDDIR)/$(notdir $@)/$(notdir $@)

mcp: libc