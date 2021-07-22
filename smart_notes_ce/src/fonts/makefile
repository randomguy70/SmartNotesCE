# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= DEMO
ICON ?= icon.png
DESCRIPTION ?= "CE C Toolchain Demo"
COMPRESSED ?= NO
ARCHIVED ?= NO

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# font options
FONTPACKNAME ?= EXTRA01
FONTDIR ?= $(SRCDIR)/fonts

DEPS ?= $(BINDIR)/$(FONTPACKNAME).bin $(BINDIR)/$(FONTPACKNAME).8xv

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

# ----------------------------

$(BINDIR)/$(FONTPACKNAME).bin: $(FONTDIR)/medium1.fnt $(FONTDIR)/medium2.fnt $(FONTDIR)/medium3.fnt $(FONTDIR)/medium4.fnt $(FONTDIR)/medium5.fnt
	$(Q)$(call MKDIR,$(@D))
	$(Q)convfont -o fontpack -N "smartNotesCE" -P "Calculator 1252" \
	-D "A font designed for maximum readability" \
	-V "15 April 2019" \
	-f $(FONTDIR)/medium1.fnt   -l 11 -x 5 -c 2 -a 1 -b 1 -w normal -s sans-serif -s upright -s proportional \
	-f $(FONTDIR)/medium2.fnt -l 11 -x 5 -c 2 -a 1 -b 1 -w normal -s 0 \
	-f $(FONTDIR)/medium3.fnt   -l 8  -x 3 -c 1 -a 1 -b 1 -w normal -s 0 \
	-f $(FONTDIR)/medium4.fnt -l 8  -x 3 -c 1 -a 1 -b 1 -w normal -s 0 \
	-f $(FONTDIR)/medium5.fnt   -l 7  -x 3 -c 1 -a 1 -b 1 -w normal -s 0 \
	$(BINDIR)/$(FONTPACKNAME).bin

$(BINDIR)/$(FONTPACKNAME).8xv: $(BINDIR)/$(FONTPACKNAME).bin
	$(Q)$(call MKDIR,$(@D))
	$(Q)convbin --iformat bin --input $(BINDIR)/$(FONTPACKNAME).bin --oformat 8xv --output $(BINDIR)/$(FONTPACKNAME).8xv --archive --name $(FONTPACKNAME)

