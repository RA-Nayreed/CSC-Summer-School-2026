# SPDX-FileCopyrightText: 2026 CSC - IT Center for Science Ltd. <www.csc.fi>
#
# SPDX-License-Identifier: MIT

# Build the heat-restart equation exercise or its model solution.
# Usage:
#   make                                    # build the exercise code
#   make SOLUTION=solution-send-recv        # build model solution-send-recv

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
CXX=mpicxx
CC=mpicc
FC=mpif90
CCFLAGS=-g -O3 -march=native -Wall -DNDEBUG
FCFLAGS=-g -O3 -march=native -Wall
FCMODFLAGS=-J$(BUILDDIR)
LDFLAGS=
LIBS=
endif

ifeq ($(PLATFORM),mahti)
CXX=mpicxx
CC=mpicc
FC=mpif90
CCFLAGS=-O3 -march=native -Wall -DNDEBUG
FCFLAGS=-O3 -march=native -Wall
FCMODFLAGS=-J$(BUILDDIR)
LDFLAGS=
LIBS=
endif

ifeq ($(PLATFORM),lumi)
CXX=CC
CC=cc
FC=ftn
CCFLAGS=-O3 -Wall
FCFLAGS=-O3
FCMODFLAGS=-J$(BUILDDIR)
LDFLAGS=
LIBS=
endif

# Explicitly list objects (.o)
OBJS = $(addprefix $(BUILDDIR)/,heat_mod.o pngwriter_mod.o core.o utilities.o io.o setup.o main.o)

# List module dependencies so that the build proceeds in correct order
$(BUILDDIR)/pngwriter_mod.o: $(BUILDDIR)/heat_mod.o
$(BUILDDIR)/core.o:          $(BUILDDIR)/heat_mod.o
$(BUILDDIR)/utilities.o:     $(BUILDDIR)/heat_mod.o
$(BUILDDIR)/io.o:            $(BUILDDIR)/heat_mod.o $(BUILDDIR)/pngwriter_mod.o
$(BUILDDIR)/setup.o:         $(BUILDDIR)/heat_mod.o $(BUILDDIR)/utilities.o $(BUILDDIR)/io.o
$(BUILDDIR)/main.o:          $(BUILDDIR)/heat_mod.o $(BUILDDIR)/core.o $(BUILDDIR)/io.o $(BUILDDIR)/setup.o $(BUILDDIR)/utilities.o

ifeq ($(wildcard $(SRCDIR)/*.F90),)
$(error No source files found in '$(SRCDIR)'! check that SOLUTION is correct)
endif

EXE = heat_mpi
EXE_FULL = $(BUILDDIR)/$(EXE)
DEPS = $(OBJS:.o=.d)

all: $(EXE)

$(BUILDDIR):
	@mkdir -p $@

# Just copies the produced executable to Makefile directory
$(EXE): $(EXE_FULL)
	cp $< $@

$(EXE_FULL): $(OBJS) $(HEAT_COMMON_LIB)
	$(FC) $(FCFLAGS) $(LDFLAGS) $(OBJS) $(HEAT_COMMON_LIB) -o $@ $(LIBS)

# The -MMD -MP flags instruct the compiler to generate file dependency graphs,
# so that on consecutive builds we only need to rebuild objects that actually changed.

$(BUILDDIR)/%.o: $(SRCDIR)/%.F90 | $(BUILDDIR)
	$(FC) $(FCFLAGS) $(FCMODFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean: heat-common-clean
	-/bin/rm -rf build $(EXE)
