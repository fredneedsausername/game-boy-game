###############################################################################
# GBDK + SDCC MAKEFILE — WINDOWS ONLY (cmd.exe)
# Produces:
#   MODE=debug  : bin\debug\game.gb  (+ .cdb/.map/[.noi] next to it)
#   MODE=release: bin\release\game.gb
# Intermediates:
#   build_files\debug\** or build_files\release\** (.rel, .d, .ihx, .map, .cdb, .noi, .adb)
###############################################################################

.DELETE_ON_ERROR:

# ----------------------------- REQUIRED MODE ---------------------------------
ifeq ($(origin MODE), undefined)
  $(error MODE is required. Use: make MODE=debug   or   make MODE=release)
endif
ifeq ($(MODE),debug)
  WANT_DEBUG := 1
else ifeq ($(MODE),release)
  WANT_DEBUG := 0
else
  $(error Invalid MODE="$(MODE)". Use exactly: debug  or  release)
endif

.DEFAULT_GOAL := all

# ----------------------------- Toolchain / Platform --------------------------
ifndef GBDK_HOME
	GBDK_HOME := C:/gbdk/
endif

PORT := sm83
PLAT := gb

SDCC     := $(GBDK_HOME)bin/sdcc
AS       := $(GBDK_HOME)bin/sdasgb
LD       := $(GBDK_HOME)bin/sdldgb
MAKEBIN  := $(GBDK_HOME)bin/makebin
IHXCHECK := $(GBDK_HOME)bin/ihxcheck

CRT0 := $(GBDK_HOME)lib/$(PLAT)/crt0.o

# Linker search paths + libs
LDSEARCH := -k $(GBDK_HOME)lib/$(PORT) -k $(GBDK_HOME)lib/$(PLAT)
LDLIBS   := -l sm83.lib -l gb.lib

# Base linker flags:
#  -n quiet, -w wide map, -m write .map, -i write .ihx
LDFLAGS_BASE := -n -w -m -i
# Debug symbol outputs:
#  -y => SDCC C debug (.cdb),  -j => NoICE (.noi) [optional]
ifeq ($(WANT_DEBUG),1)
  LDFLAGS := $(LDFLAGS_BASE) -y -j
else
  LDFLAGS := $(LDFLAGS_BASE)
endif

# Defaults lcc usually injects (fixes undefined symbol warnings)
#  _shadow_OAM (sprite RAM), _DATA (RAM start), .STACK (initial SP),
#  .refresh_OAM (HRAM trampoline for OAM DMA)
LDMEM := -g _shadow_OAM=0xC000 -b _DATA=0xC0A0 -g .STACK=0xE000 -g .refresh_OAM=0xFF80 -b _CODE=0x0200
LDFLAGS += $(LDMEM)

# ----------------------------- Project Paths ---------------------------------
PROJECTNAME := game
SRCDIR      := src

# Per-mode output roots (fully isolated)
BUILDDIR    := build_files/$(MODE)
BINDIR      := bin/$(MODE)

IHX := $(BUILDDIR)/$(PROJECTNAME).ihx
ROM := $(BINDIR)/$(PROJECTNAME).gb
MAP := $(BUILDDIR)/$(PROJECTNAME).map
CDB := $(BUILDDIR)/$(PROJECTNAME).cdb
NOI := $(BUILDDIR)/$(PROJECTNAME).noi

# ----------------------------- Flags / Opt Level -----------------------------
# SAME optimization for both modes (empty by default) → stable timing + quick builds.
OPTFLAGS ?=
CFLAGS := -m$(PORT) -D__PORT_$(PORT) -D__TARGET_$(PLAT) \
          -I$(GBDK_HOME)include -I$(SRCDIR) $(OPTFLAGS)
ifeq ($(WANT_DEBUG),1)
  CFLAGS += --debug
endif
ASFLAGS := -I$(GBDK_HOME)lib/$(PLAT)

# ----------------------------- ROM HEADER (makebin) --------------------------
# Basic, standard cart: ROM ONLY (0x00), 32 KiB total, no ext RAM, title "GAME"
MAKEBIN_FLAGS := -yt0x00 -yo2 -ya0 -yn"GAME"

# ----------------------- Windows helpers / source scan -----------------------
SHELL := cmd
.SHELLFLAGS := /C
NULL := NUL
ROOT := $(subst \,/,$(CURDIR))

# Find sources recursively (absolute), then strip ROOT and normalize slashes
WIN_CSRC_ABS   := $(subst \,/,$(shell dir /s /b "$(SRCDIR)\*.c" 2>nul))
WIN_ASMSRC_ABS := $(subst \,/,$(shell dir /s /b "$(SRCDIR)\*.s" 2>nul))
CSOURCES   := $(patsubst $(ROOT)/%,%,$(WIN_CSRC_ABS))
ASMSOURCES := $(patsubst $(ROOT)/%,%,$(WIN_ASMSRC_ABS))

# Map src -> build; separate namespaces to avoid stem collisions
COBJS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/c/%,$(CSOURCES:.c=.rel))
AOBJS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/asm/%,$(ASMSOURCES:.s=.rel))
OBJS  := $(COBJS) $(AOBJS)

DEPS := $(COBJS:.rel=.d)

# Windows shell helpers
MKDIR_P        = if not exist "$(subst /,\,$1)" mkdir "$(subst /,\,$1)"
RM_RF 		   = if exist "$(subst /,\,$1)" rmdir /S /Q "$(subst /,\,$1)" 2>nul
COPYF          = copy /Y "$(subst /,\,$1)" "$(subst /,\,$2)" >NUL
COPY_IF_EXISTS = if exist "$(subst /,\,$1)" copy /Y "$(subst /,\,$1)" "$(subst /,\,$2)" >NUL

# ----------------------------- Parallelism -----------------------------------
# Respect user's -j/--jobs if provided; otherwise default to CPU count (fallback 4)
ifneq ($(filter -j% -j --jobs% --jobs,$(MAKEFLAGS)),)
  # user already set the job count; don't override
else
  ifndef J
    J := $(NUMBER_OF_PROCESSORS)
    ifeq ($(strip $(J)),)
      J := 4
    endif
  endif
  MAKEFLAGS += -j$(J)
endif

# ----------------------- Preflight (status + checks) -------------------------
NORMCURDIR := $(subst \,/,$(CURDIR))
.PHONY: preflight
preflight:
	@echo === GBDK Build ===
	@echo MODE=$(MODE)
	@echo OS=Windows
	@echo CORES=$(J)
	@echo GBDK_HOME=$(GBDK_HOME)
	@echo OUT_BIN=$(BINDIR)
	@echo OUT_BUILD=$(BUILDDIR)
	@echo SDCC=$(SDCC)
	@echo SDASGB=$(AS)
	@echo SDLDGB=$(LD)
	@echo MAKEBIN=$(MAKEBIN)
	@echo Sources: C=$(words $(CSOURCES))  ASM=$(words $(ASMSOURCES))
ifneq ($(filter C:/% c:/%,$(NORMCURDIR)), $(NORMCURDIR))
	@echo [WARNING] Building from "$(CURDIR)" (NOT on drive C:).
	@echo           Avoid SPACES in folder names; consider moving to C:\ .
endif
	@(if not exist "$(SDCC).exe"     (echo [ERROR] sdcc not found at "$(SDCC).exe" & exit /b 1)     else (echo sdcc OK.))
	@(if not exist "$(AS).exe"       (echo [ERROR] sdasgb not found at "$(AS).exe" & exit /b 1)     else (echo sdasgb OK.))
	@(if not exist "$(LD).exe"       (echo [ERROR] sdldgb not found at "$(LD).exe" & exit /b 1)     else (echo sdldgb OK.))
	@(if not exist "$(MAKEBIN).exe"  (echo [ERROR] makebin not found at "$(MAKEBIN).exe" & exit /b 1) else (echo makebin OK.))
	@(if not exist "$(IHXCHECK).exe" (echo [ERROR] ihxcheck not found at "$(IHXCHECK).exe" & exit /b 1) else (echo ihxcheck OK.))
	@(if not exist "$(CRT0)"         (echo [ERROR] crt0.o not found at "$(CRT0)" & exit /b 1)       else (echo crt0 OK.))
	@if "$(words $(CSOURCES) $(ASMSOURCES))"=="0" echo [ERROR] No sources found under "$(SRCDIR)". & exit /b 1

# -------------------------------- Targets ------------------------------------
.PHONY: all clean
all: $(ROM)

# Ensure preflight runs BEFORE any parallel job:
$(ROM): preflight
$(IHX): preflight
$(BUILDDIR)/c/%.rel: preflight
$(BUILDDIR)/asm/%.rel: preflight
$(BINDIR): preflight
$(BUILDDIR): preflight

# Convert IHX -> GB; in debug, copy symbols next to ROM for Emulicious
$(ROM): $(IHX) | $(BINDIR)
	$(MAKEBIN) -Z $(MAKEBIN_FLAGS) "$(IHX)" "$@"
ifeq ($(WANT_DEBUG),1)
	@$(call COPYF,$(CDB),$(BINDIR))
	@$(call COPYF,$(MAP),$(BINDIR))
	@$(call COPY_IF_EXISTS,$(NOI),$(BINDIR))
endif

# Link: sdldgb writes build_files\<MODE>\game.{ihx,map,cdb,noi}
$(IHX): $(CRT0) $(OBJS) | $(BUILDDIR)
	$(LD) $(LDFLAGS) "$(BUILDDIR)/$(PROJECTNAME)" $(CRT0) $(OBJS) $(LDSEARCH) $(LDLIBS)
	$(IHXCHECK) "$(IHX)" > NUL 2>&1
	@rem Sweep any stray .adb (root) into build_files\<MODE>
	@for %%f in (*.adb) do @move /Y "%%f" "$(subst /,\,$(BUILDDIR))" >NUL 2>nul
	@rem Sweep any stray .adb under src\ into build_files\<MODE>
	@for /R "$(SRCDIR)" %%f in (*.adb) do @move /Y "%%f" "$(subst /,\,$(BUILDDIR))" >NUL 2>nul

# Compile C -> .rel + .d  (also move this module's .adb to build_files\<MODE>\)
$(BUILDDIR)/c/%.rel: $(SRCDIR)/%.c | $(BUILDDIR)
	$(call MKDIR_P,$(dir $@))
	$(SDCC) $(CFLAGS) -c -o "$@" "$<"
	$(SDCC) -E $(CFLAGS) -Wp-M -Wp-MT,"$@" -Wp-MF,"$(BUILDDIR)/c/$*.d" "$<" > $(NULL) 2>&1
	@if exist "$(notdir $(basename $<)).adb" move /Y "$(notdir $(basename $<)).adb" "$(subst /,\,$(BUILDDIR))" >NUL
	@if exist "$(subst /,\,$(dir $<))$(notdir $(basename $<)).adb" move /Y "$(subst /,\,$(dir $<))$(notdir $(basename $<)).adb" "$(subst /,\,$(BUILDDIR))" >NUL

# Assemble ASM -> .rel
$(BUILDDIR)/asm/%.rel: $(SRCDIR)/%.s | $(BUILDDIR)
	$(call MKDIR_P,$(dir $@))
	$(AS) -o "$@" $(ASFLAGS) "$<"

# Ensure output dirs
$(BINDIR):
	$(call MKDIR_P,$@)
$(BUILDDIR):
	$(call MKDIR_P,$@)

# Include auto-generated deps (per-mode)
-include $(DEPS)

# Cleanup (per-mode only)
clean:
	@$(call RM_RF,$(BUILDDIR))
	@$(call RM_RF,$(BINDIR))