# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= NOTES
ICON ?= icon.png
DESCRIPTION ?= "A User-friendly text editor, v1.0"
COMPRESSED ?= YES
ARCHIVED ?= YES

CFLAGS ?= -Wall -Wextra -O1
# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

#------------------------------