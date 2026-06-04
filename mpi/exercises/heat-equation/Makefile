# SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>
#
# SPDX-License-Identifier: MIT

# Build the heat equation exercise or one of its model solutions.
# Usage:
#   make                                  # build from cpp/
#   make TARGET=cpp/solution-sendrecv     # build C++ solution
#   make TARGET=c                         # build C variant
#   make TARGET=c/solution                # build C solution
#   make TARGET=fortran                   # build Fortran variant
#   make TARGET=fortran/solution-sendrecv # build Fortran solution

.DEFAULT_GOAL := all

# default to LUMI configuration
ifeq ($(COMP),)
COMP=lumi
endif

TARGET   ?= cpp
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
FC=mpif90
CCFLAGS=-g -O3 -march=native -Wall -DNDEBUG
FCFLAGS=-O3 -Wall -march=native
FCMODFLAGS=-J$(BUILDDIR)
LDFLAGS=
LIBS=
endif

ifeq ($(COMP),intel)
CXX=mpicxx
CC=mpicc
FC=mpif90
CCFLAGS=-g -O3 -xHost -DNDEBUG
FCFLAGS=-O2 -xHost
FCMODFLAGS=-module $(BUILDDIR)
LDFLAGS=
LIBS=
endif

ifeq ($(COMP),lumi)
CXX=CC
CC=cc
FC=ftn
CCFLAGS=-O3 -Wall
FCFLAGS=-O3
FCMODFLAGS=-J$(BUILDDIR)
LDFLAGS=
LIBS=
endif

vpath %.cpp $(SRCDIR)
vpath %.hpp $(SRCDIR)
vpath %.c   $(SRCDIR)
vpath %.h   $(SRCDIR)
vpath %.F90 $(SRCDIR)


# Figure out the target language so that we can set correct sources and linker executable
_LANG := $(firstword $(subst /, ,$(SRCDIR)))

ifeq ($(_LANG),c)
_SRCS   := $(wildcard $(SRCDIR)/*.c)
OBJS    := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(_SRCS))
_LINKER  = $(CC)
_LFLAGS  = $(CCFLAGS)
# Explicitly link math library (in C++ we get it through libstdc++)
LIBS    += -lm
else ifeq ($(_LANG),fortran)
_SRCS   := $(wildcard $(SRCDIR)/*.F90)
OBJS    := $(patsubst $(SRCDIR)/%.F90,$(BUILDDIR)/%.o,$(_SRCS))
_LINKER  = $(FC)
_LFLAGS  = $(FCFLAGS)
else
_SRCS   := $(wildcard $(SRCDIR)/*.cpp)
OBJS    := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(_SRCS))
_LINKER  = $(CXX)
_LFLAGS  = $(CCFLAGS)
endif

EXE = heat_mpi
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

$(BUILDDIR)/%.o: %.F90 | $(BUILDDIR)
	$(FC) $(FCFLAGS) $(FCMODFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean: heat-common-clean
	-/bin/rm -rf build $(EXE)
