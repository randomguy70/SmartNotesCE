# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= NOTES
ICON ?= icon.png
DESCRIPTION ?= "A Text Editor For the TI-84 PCE"
COMPRESSED ?= NO # will change this for release. it's a little faster this way for each beta build
ARCHIVED ?= NO   # will change this (also) for release

CFLAGS ?= -Wall -Wextra -Oz
# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

#------------------------------