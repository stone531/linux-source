# The mpn functions need a #define for asm syntax flavor.
# Every i386 port in use uses gas syntax (I think).
asm-CPPFLAGS := $(asm-CPPFLAGS) -DGAS_SYNTAX

# The i386 `long double' is a distinct type we support.
long-double-fcts = yes

ifeq ($(subdir),elf)
CFLAGS-rtld.c += -Wno-uninitialized
CFLAGS-dl-load.c += -Wno-unused
CFLAGS-dl-reloc.c += -Wno-unused
endif
