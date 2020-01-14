include $(DEFAULTMAKE)

APPS := mcp

APP_EXECUTABLES := $(APPS:%=$(BUILDDIR)/%)

all: $(APP_EXECUTABLES)

_ignore:
.PHONY: _ignore
$(APP_EXECUTABLES): _ignore
	+BUILDDIR=$(BUILDDIR)/$(notdir $@) $(MAKE) -C $(notdir $@) $(BUILDDIR)/$(notdir $@)/$(notdir $@)
