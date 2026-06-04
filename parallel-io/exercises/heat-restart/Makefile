# SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>
#
# SPDX-License-Identifier: MIT

# Build the heat-restart equation exercise or its model solution.
# Usage:
#   make                                  # build from c/
#   make TARGET=c/solution                # build C model solution
.DEFAULT_GOAL := all

# default to LUMI configuration
ifeq ($(COMP),)
COMP=lumi
endif

TARGET   ?= c
SRCDIR   := $(TARGET)
BUILDDIR := build/$(SRCDIR)

# Override before including so heat-shared artifacts land in our build tree, separately for each target
HEAT_COMMON_LIB := $(BUILDDIR)/heat_common.a

# Include the "heat-shared" Makefile: fills in HEAT_SHARED_CFLAGS and adds rule for building HEAT_COMMON_LIB (static)
HEAT_SHARED_DIR := ../../../heat-shared
include $(HEAT_SHARED_DIR)/Makefile-common

ifeq ($(COMP),gnu)
CXX=mpicxx
CC=mpicc
CCFLAGS=-g -O3 -march=native -Wall -DNDEBUG
LDFLAGS=
LIBS=
endif

ifeq ($(COMP),intel)
CXX=mpicxx
CC=mpicc
CCFLAGS=-g -O3 -xHost -DNDEBUG
LDFLAGS=
LIBS=
endif

ifeq ($(COMP),lumi)
CXX=CC
CC=cc
CCFLAGS=-O3 -Wall
LDFLAGS=
LIBS=
endif

vpath %.cpp $(SRCDIR)
vpath %.c   $(SRCDIR)
vpath %.h   $(SRCDIR)

# Figure out the target language so that we can set correct sources and linker executable
_LANG := $(firstword $(subst /, ,$(SRCDIR)))

ifeq ($(_LANG),c)
_SRCS   := $(wildcard $(SRCDIR)/*.c)
OBJS    := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(_SRCS))
_LINKER  = $(CC)
_LFLAGS  = $(CCFLAGS)
# Explicitly link math library (in C++ we get it through libstdc++)
LIBS    += -lm
else
_SRCS   := $(wildcard $(SRCDIR)/*.cpp)
OBJS    := $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(_SRCS))
_LINKER  = $(CXX)
_LFLAGS  = $(CCFLAGS)
endif

ifeq ($(_SRCS),)
$(error No source files found in '$(SRCDIR)'! Check that TARGET is correct)
endif

EXE = heat_restart
EXE_FULL = $(BUILDDIR)/$(EXE)
DEPS = $(OBJS:.o=.d)

all: $(EXE)

$(BUILDDIR):
	mkdir -p $@

# Just copies the produced executable to Makefile directory
$(EXE): $(EXE_FULL)
	cp $< $@

$(EXE_FULL): $(OBJS) $(HEAT_COMMON_LIB)
	$(_LINKER) $(_LFLAGS) $(OBJS) $(HEAT_COMMON_LIB) -o $@ $(LDFLAGS) $(LIBS)

# The -MMD -MP flags instruct the compiler to generate file dependency graphs,
# so that on consecutive builds we only need to rebuild objects that actually changed.

$(BUILDDIR)/%.o: %.cpp | $(BUILDDIR)
	$(CXX) $(CCFLAGS) $(HEAT_SHARED_CFLAGS) -I$(SRCDIR) -MMD -MP -c $< -o $@

$(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	$(CC) $(CCFLAGS) $(HEAT_SHARED_CFLAGS) -I$(SRCDIR) -MMD -MP -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean: heat-common-clean
	-/bin/rm -rf build $(EXE)
