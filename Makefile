#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITPRO)/libnx/switch_rules
include $(TOPDIR)/deps.mk

#---------------------------------------------------------------------------------
# TARGET is the name of the output
#---------------------------------------------------------------------------------
TARGET		:=	$(PLSR_TARGET)
SOURCES		:=	$(addprefix $(PLSR_DIR), $(PLSR_SOURCES))
INCLUDES	:=	$(addprefix $(PLSR_DIR), $(PLSR_INCLUDES))
BUILD		:=	build
OUT			:=	lib

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIC -ftls-model=local-exec

CFLAGS	:=	-g -Wall -Werror \
			-ffunction-sections \
			-fdata-sections \
			$(ARCH) \
			$(BUILD_CFLAGS)

CFLAGS	+=	$(INCLUDE)

CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	:=	-g $(ARCH)

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS := $(PORTLIBS) $(LIBNX)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD_TYPE),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export TOPDIR	:=	$(CURDIR)
export VPATH	:=	$(foreach dir,$(SOURCES),$(dir))

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))

export LD	:=	$(CC)
export OFILES_SRC	:=	$(CFILES:.c=.o) 
export OFILES 	:=	$(OFILES_SRC)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include)

.PHONY: clean all

#---------------------------------------------------------------------------------
all: debug release

$(OUT):
	@[ -d $@ ] || mkdir -p $@

$(OUT)/lib$(TARGET).a : $(OUT) $(SOURCES) $(INCLUDES)
	@[ -d $(BUILD)/release ] || mkdir -p $(BUILD)/release
	@$(MAKE) BUILD_TYPE=release OUTPUT=$(CURDIR)/$@ \
	BUILD_CFLAGS="-DNDEBUG=1 -O2" \
	DEPSDIR=$(CURDIR)/$(BUILD)/release \
	--no-print-directory -C $(BUILD)/release \
	-f $(TOPDIR)/Makefile

$(OUT)/lib$(TARGET)d.a : $(OUT) $(SOURCES) $(INCLUDES)
	@[ -d $(BUILD)/debug ] || mkdir -p $(BUILD)/debug
	@$(MAKE) BUILD_TYPE=debug OUTPUT=$(CURDIR)/$@ \
	BUILD_CFLAGS="-DDEBUG=1 -Og" \
	DEPSDIR=$(CURDIR)/$(BUILD)/debug \
	--no-print-directory -C $(BUILD)/debug \
	-f $(TOPDIR)/Makefile

debug: $(OUT)/lib$(TARGET).a

release: $(OUT)/lib$(TARGET)d.a

dist-bin: all
	@tar --exclude=*~ -cjf lib$(TARGET).tar.bz2 include lib

dist-src:
	@tar --exclude=*~ -cjf lib$(TARGET)-src.tar.bz2 include src Makefile

dist: dist-src dist-bin

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) lib *.bz2

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT)	:	$(OFILES)

#---------------------------------------------------------------------------------
%_bin.h %.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)


-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
