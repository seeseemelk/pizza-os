include $(PKGDIR)/pizzaos/include.mk

LIBC := $(PKGBUILDDIR)/libc/libc.a
LIBC_LIBS := $(PIZZAOS_LIBS) $(LIBC)
LIBC_CFLAGS := $(PIZZAOS_CFLAGS) -I$(PKGDIR)/libc/include
