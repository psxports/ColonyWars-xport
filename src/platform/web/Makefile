PROJECT_ROOT := ../../..
SHORT_NAME := CW
FULL_NAME := Colony Wars
CONFIG ?= Debug
APP_SOURCES := $(sort $(wildcard ../../batch_*.c)) ../../game_main.c ../../native_dispatch.c ../../native_stream_source.c
APP_DEFINES := -DFIELD_RATE=60 -DWND_TITLE=\"CW\" -DWND_WIDTH=960 -DWND_HEIGHT=720
WEB_EXCLUDE_FILES := DATA/*/ZZZ.ZZZ DATA/*/SYSTEM.CNF

ifeq ($(CONFIG),Release)
APP_DEFINES += -DXPORT_DISABLE_FUNCTION_MARKERS
endif

include ../../../../xport/src/platform/web/Makefile
LINK_FLAGS += -sSTACK_OVERFLOW_CHECK=2
LINK_FLAGS += -sASYNCIFY_STACK_SIZE=16384
