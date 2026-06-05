# SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>
#
# SPDX-License-Identifier: MIT

# Build the heat-restart equation exercise or its model solution.
# Usage:
#   make                                    # build the exercise code
#   make SOLUTION=solution                  # build model solution

.DEFAULT_GOAL := all

# default to LUMI configuration
ifeq ($(PLATFORM),)
PLATFORM=lumi
endif

$(info PLATFORM: $(PLATFORM))

# SOLUTION specifies which model solution to build. If left empty, build the exercise code from Makefile directory
SOLUTION ?=
SRCDIR   := $(if $(SOLUTION),$(SOLUTION),.)
BUILDDIR := $(if $(SOLUTION),build/$(SOLUTION),build)

# Override before including so heat-shared artifacts land in our build tree, separately for each target
HEAT_COMMON_LIB := $(BUILDDIR)/heat_common.a

# Include the "heat-shared" Makefile: fills in HEAT_SHARED_CFLAGS and adds rule for building HEAT_COMMON_LIB (static)
HEAT_SHARED_DIR := ../../../../heat-shared
include $(HEAT_SHARED_DIR)/Makefile-common

# PLATFORM=generic is intended for laptops and local workstations. Should work on any Unix-like system provided that MPI is available
ifeq ($(PLATFORM),generic)
CXX=g++
CC=gcc
CCFLAGS=-fopenmp -g -O3 -march=native -Wall -DNDEBUG
LDFLAGS=-fopenmp
LIBS=
endif

ifeq ($(PLATFORM),mahti)
CXX=g++
CC=gcc
CCFLAGS=-fopenmp -O3 -march=native -Wall -DNDEBUG
LDFLAGS=-fopenmp
LIBS=
endif

ifeq ($(PLATFORM),lumi)
CXX=CC
CC=cc
CCFLAGS=-fopenmp -O3 -Wall
LDFLAGS=-fopenmp
LIBS=
endif

# Explicitly list objects (.o), one for each source file.
# Larger projects may want to automatize this with file globbing and wildcards
OBJS = $(addprefix $(BUILDDIR)/,main.o core.o heat.o setup.o io.o utilities.o)

ifeq ($(wildcard $(SRCDIR)/*.cpp),)
$(error No source files found in '$(SRCDIR)'! check that SOLUTION is correct)
endif

EXE = heat_omp
EXE_FULL = $(BUILDDIR)/$(EXE)
DEPS = $(OBJS:.o=.d)

all: $(EXE)

$(BUILDDIR):
	@mkdir -p $@

# Just copies the produced executable to Makefile directory
$(EXE): $(EXE_FULL)
	cp $< $@

$(EXE_FULL): $(OBJS) $(HEAT_COMMON_LIB)
	$(CXX) $(CCFLAGS) $(LDFLAGS) $(OBJS) $(HEAT_COMMON_LIB) -o $@ $(LIBS)

# The -MMD -MP flags instruct the compiler to generate file dependency graphs,
# so that on consecutive builds we only need to rebuild objects that actually changed.

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CCFLAGS) $(HEAT_SHARED_CFLAGS) -I$(SRCDIR) -MMD -MP -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean: heat-common-clean
	-/bin/rm -rf build $(EXE)
