#
#  Copyright (C) 2024 Bernhard Schelling
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with this program; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#

ifeq ($(ISWIN),)
ISWIN      := $(findstring :,$(firstword $(subst \, ,$(subst /, ,$(abspath .)))))
endif

ifeq ($(ISMAC),)
ISMAC      := $(wildcard /Applications)
endif

PIPETONULL := $(if $(ISWIN),>nul 2>nul,>/dev/null 2>/dev/null)

SOURCES := vidtest.c

CPUFLAGS := $(MAKE_CPUFLAGS)
STRIPCMD := $(or $(STRIP),strip) --strip-all
ifneq ($(ISWIN),)
  OUTNAME := vidtest_libretro.dll
  CC      ?= gcc
  LDFLAGS := -Wl,--gc-sections -fno-ident
  COMMONFLAGS += -pthread
else ifneq (,$(findstring ios,$(platform)))
  ifeq ($(IOSSDK),)
    IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
  endif
  OUTNAME := vidtest_libretro_ios.dylib
  MINVERSION :=
  COMMONFLAGS += -DDISABLE_DYNAREC=1 -DDBP_IOS
  ifeq ($(platform),ios-arm64)
    CC      = c++ -arch arm64 -isysroot $(IOSSDK)
  else
    CC      = c++ -arch armv7 -isysroot $(IOSSDK)
  endif
  LDFLAGS := -Wl,-dead_strip
  ifeq ($(platform),$(filter $(platform),ios9 ios-arm64))
    MINVERSION = -miphoneos-version-min=8.0
  else
    MINVERSION = -miphoneos-version-min=5.0
  endif
  COMMONFLAGS += $(MINVERSION) -Wno-ignored-optimization-argument -Wno-unknown-warning-option
  STRIPCMD := $(or $(STRIP),strip) -xS
else ifeq ($(platform),tvos-arm64)
  ifeq ($(IOSSDK),)
    IOSSDK := $(shell xcodebuild -version -sdk appletvos Path)
  endif
  OUTNAME := vidtest_libretro_tvos.dylib
  CC      = c++ -arch arm64 -isysroot $(IOSSDK)
  LDFLAGS := -Wl,-dead_strip
  COMMONFLAGS += -DDISABLE_DYNAREC=1 -Wno-unknown-warning-option
  STRIPCMD := $(or $(STRIP),strip) -xS
else ifneq ($(ISMAC),)
  OUTNAME := vidtest_libretro.dylib
  CC      ?= c++
  LDFLAGS := -Wl,-dead_strip
  COMMONFLAGS += -pthread -Wno-unknown-warning-option
  ifeq ($(CROSS_COMPILE),1)
    COMMONFLAGS  += -DDISABLE_DYNAREC=1
    TARGET_RULE   = -target $(LIBRETRO_APPLE_PLATFORM) -isysroot $(LIBRETRO_APPLE_ISYSROOT)
    COMMONFLAGS   += $(TARGET_RULE)
    LDFLAGS       += $(TARGET_RULE)
  endif
  COMMONFLAGS  += $(ARCHFLAGS)
  LDFLAGS      += $(ARCHFLAGS)
  STRIPCMD := $(or $(STRIP),strip) -xS
else ifeq ($(platform),windows) # For MSYS2 only
  OUTNAME := vidtest_libretro.dll
  CC      ?= gcc
  LDFLAGS := -Wl,--gc-sections -fno-ident
else ifeq ($(platform),vita)
  OUTNAME := vidtest_libretro_vita.a
  CC      := arm-vita-eabi-gcc
  AR      := arm-vita-eabi-ar
  COMMONFLAGS += -DVITA
  COMMONFLAGS += -mthumb -mcpu=cortex-a9 -mfloat-abi=hard -ftree-vectorize -ffast-math -fsingle-precision-constant -funroll-loops
  COMMONFLAGS += -mword-relocations
  COMMONFLAGS += -fno-optimize-sibling-calls
  STATIC_LINKING = 1
else ifeq ($(platform),ctr)
  OUTNAME := vidtest_libretro_ctr.a
  CC      := $(DEVKITARM)/bin/arm-none-eabi-gcc
  AR      := $(DEVKITARM)/bin/arm-none-eabi-ar
  COMMONFLAGS += -DARM11 -D_3DS -Os -s -I$(CTRULIB)/include/ -DHAVE_MKDIR
  COMMONFLAGS += -march=armv6k -mtune=mpcore -mfloat-abi=hard -mword-relocations
  COMMONFLAGS += -fomit-frame-pointer -fstrict-aliasing -ffast-math -fpermissive
  COMMONFLAGS += -I$(DEVKITPRO)/libctru/include
  STATIC_LINKING = 1
else ifeq ($(platform),ngc)
  OUTNAME := vidtest_libretro_ngc.a
  CC      := $(DEVKITPPC)/bin/powerpc-eabi-gcc
  AR      := $(DEVKITPPC)/bin/powerpc-eabi-ar
  COMMONFLAGS += -DGEKKO -DHW_DOL -mrvl -mcpu=750 -meabi -mhard-float -D__POWERPC__ -D__ppc__ -DMSB_FIRST -DWORDS_BIGENDIAN=1
  STATIC_LINKING = 1
else ifeq ($(platform),wii)
  OUTNAME := vidtest_libretro_wii.a
  CC      := $(DEVKITPPC)/bin/powerpc-eabi-gcc
  AR      := $(DEVKITPPC)/bin/powerpc-eabi-ar
  COMMONFLAGS += -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -fpermissive
  COMMONFLAGS += -U__INT32_TYPE__ -U__UINT32_TYPE__ -D__INT32_TYPE__=int -D__POWERPC__ -D__ppc__ -DMSB_FIRST -DWORDS_BIGENDIAN=1
  STATIC_LINKING = 1
else ifeq ($(platform),wiiu)
  OUTNAME := vidtest_libretro_wiiu.a
  CC      := $(DEVKITPPC)/bin/powerpc-eabi-gcc
  AR      := $(DEVKITPPC)/bin/powerpc-eabi-ar
  COMMONFLAGS += -DGEKKO -DWIIU -DHW_RVL -mcpu=750 -meabi -mhard-float
  COMMONFLAGS += -U__INT32_TYPE__ -U__UINT32_TYPE__ -D__INT32_TYPE__=int -D__POWERPC__ -D__ppc__ -DMSB_FIRST -DWORDS_BIGENDIAN=1 -DGX_PTHREAD_LEGACY
  STATIC_LINKING = 1
else ifeq ($(platform),libnx)
  OUTNAME := vidtest_libretro_libnx.a
  export DEPSDIR = $(CURDIR)
  include $(DEVKITPRO)/libnx/switch_rules
  COMMONFLAGS += -I$(LIBNX)/include/ -D__SWITCH__ -DHAVE_LIBNX
  COMMONFLAGS += -march=armv8-a -mtune=cortex-a57 -mtp=soft -fPIC
  STATIC_LINKING = 1
else ifeq ($(platform),gcw0)
  # You must used the toolchain built on or around 2014-08-20
  OUTNAME := vidtest_libretro.so
  CC      := /opt/gcw0-toolchain/usr/bin/mipsel-linux-gcc
  LDFLAGS := -Wl,--gc-sections -fno-ident
  CPUFLAGS := -ffast-math -march=mips32r2 -mtune=mips32r2 -mhard-float -fexpensive-optimizations -frename-registers
  COMMONFLAGS += -pthread
  STRIPCMD := /opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/bin/strip --strip-all
else ifneq ($(findstring Haiku,$(shell uname -s)),)
  OUTNAME := vidtest_libretro.so
  LDFLAGS := -Wl,--gc-sections -fno-ident -lroot -lnetwork
  ifneq ($(findstring BePC,$(shell uname -m)),)
    CC    ?= gcc-x86
  else
    CC    ?= gcc
  endif
else
  OUTNAME := vidtest_libretro.so
  CC      ?= gcc
  LDFLAGS := -Wl,--gc-sections -fno-ident
  COMMONFLAGS += -pthread
  ifeq ($(CPUFLAGS),)
    # ARM optimizations
    UNAMEM := $(shell uname -m))
    ifeq ($(UNAMEM),aarch64)
      CPUFLAGS := -DPAGESIZE=$(or $(shell getconf PAGESIZE),4096)
    else ifeq ($(UNAMEM),armv7l)
      CPUFLAGS := -marm -mcpu=cortex-a72 -mfpu=neon-fp-armv8 -mfloat-abi=hard -ffast-math
    else ifeq ($(ARM_RPI4), 1)
      CPUFLAGS := -marm -mcpu=cortex-a72 -mfpu=neon-fp-armv8 -mfloat-abi=hard -ffast-math
    else
      ifeq ($(CORTEX_A7), 1)
        CPUFLAGS += -marm -mcpu=cortex-a7
        ifeq ($(ARM_NEON), 1)
          CPUFLAGS += -mfpu=neon-vfpv4
        endif
      endif
      ifeq ($(ARM_HARDFLOAT), 1)
        CPUFLAGS += -mfloat-abi=hard
      endif
    endif
  endif
  CC_VER := $(shell $(CC) -v 2>&1)
endif

ifeq ($(BUILD),DEBUG)
  BUILDDIR := debug
  CFLAGS   := -DDEBUG -D_DEBUG -g -O0
else ifeq ($(BUILD),PROFILE)
  BUILDDIR := profile
  CFLAGS   := -DNDEBUG -O2
else ifeq ($(BUILD),RELEASEDBG)
  BUILDDIR := releasedbg
  CFLAGS   := -DNDEBUG -ggdb -O2
  LDFLAGS  += -ggdb -O2
else ifeq ($(BUILD),ASAN)
  BUILDDIR := asan
  CFLAGS   := -DDEBUG -D_DEBUG -g -O0 -fsanitize=address -fno-omit-frame-pointer
  LDFLAGS  += -fsanitize=address -g -O0
else
  BUILD    := RELEASE
  BUILDDIR := release
  ifeq ($(platform),vita)
    CFLAGS   := -DNDEBUG -O3 -fno-ident -fno-partial-inlining
  else
    CFLAGS   := -DNDEBUG -O2 -fno-ident
  endif
  LDFLAGS  += -O2
endif

CFLAGS  += $(CPUFLAGS) -fomit-frame-pointer -fexceptions -Wall -pedantic -Wno-unused-function
ifneq ($(STATIC_LINKING), 1)
  CFLAGS  += -fpic
else
  CFLAGS  += -DSTATIC_LINKING
endif

CFLAGS  += -fvisibility=hidden -ffunction-sections
CFLAGS  += -D__LIBRETRO__ -Iinclude -D_FILE_OFFSET_BITS=64
CFLAGS  += $(COMMONFLAGS)
#CFLAGS  += -fdata-sections #saves around 32 bytes on most platforms but wrongfully adds up to 60MB on msys2

LDLIBS  := -lpthread
LDFLAGS += $(CPUFLAGS) -shared
#LDFLAGS += -static-libstdc++ -static-libgcc #adds 1MB to output and still dynamically links against libc and libm

.PHONY: all clean
all: $(OUTNAME)

$(info Building $(OUTNAME) with $(BUILD) configuration (obj files stored in build/$(BUILDDIR)) ...)
SOURCES := $(wildcard $(SOURCES))
$(if $(findstring ~,$(SOURCES)),$(error SOURCES contains a filename with a ~ character in it - Unable to continue))
$(if $(wildcard build),,$(shell mkdir "build"))
$(if $(wildcard build/$(BUILDDIR)),,$(shell mkdir "build/$(BUILDDIR)"))
OBJS := $(addprefix build/$(BUILDDIR)/,$(subst /,~,$(patsubst %,%.o,$(SOURCES))))
-include $(OBJS:%.o=%.d)
$(foreach F,$(OBJS),$(eval $(F): $(subst ~,/,$(patsubst build/$(BUILDDIR)/%.o,%,$(F))) ; $$(call COMPILE,$$@,$$<)))

clean:
	$(info Removing all build files ...)
	@$(if $(wildcard build/$(BUILDDIR)),$(if $(ISWIN),rmdir /S /Q,rm -rf) "build/$(BUILDDIR)" $(PIPETONULL))

$(OUTNAME) : $(OBJS)
ifeq ($(STATIC_LINKING), 1)
	$(info Static linking $@ ...)
	$(AR) rcs $@ $^
else
	$(info Linking $@ ...)
	@$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
ifneq ($(BUILD),DEBUG)
	$(STRIPCMD) $@
endif
endif

define COMPILE
	$(info Compiling $2 ...)
	@$(CC) $(CFLAGS) -MMD -MP -o $1 -c $2
endef
