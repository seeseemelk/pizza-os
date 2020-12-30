MKDIR = mkdir -p $(dir $@)

STATUS = printf '%-12s  %s\n'
STATUS_LD = $(STATUS) LINK $@
STATUS_AR = $(STATUS) ARCHIVE $@
STATUS_KCC = $(STATUS) "COMPILE:K" $(abspath $<)
STATUS_KCXX = $(STATUS) "COMPILE:K" $(abspath $<)
STATUS_KASM = $(STATUS) "ASSEMBLE:K" $(abspath $<)
STATUS_TAR = $(STATUS) TAR $@
