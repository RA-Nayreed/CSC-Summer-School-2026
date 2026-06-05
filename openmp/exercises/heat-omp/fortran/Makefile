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
FC=gfortran
CCFLAGS=-fopenmp -g -O3 -march=native -Wall -DNDEBUG
FCFLAGS=-fopenmp -g -O3 -march=native -Wall
FCMODFLAGS=-J$(BUILDDIR)
LDFLAGS=-fopenmp
LIBS=
endif

ifeq ($(PLATFORM),mahti)
CXX=g++
CC=gcc
FC=gfortran
CCFLAGS=-fopenmp -O3 -march=native -Wall -DNDEBUG
FCFLAGS=-fopenmp -O3 -march=native -Wall
FCMODFLAGS=-J$(BUILDDIR)
LDFLAGS=-fopenmp
LIBS=
endif

ifeq ($(PLATFORM),lumi)
CXX=CC
CC=cc
FC=ftn
CCFLAGS=-fopenmp -O3 -Wall
FCFLAGS=-fopenmp -O3
FCMODFLAGS=-J$(BUILDDIR)
LDFLAGS=-fopenmp
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

EXE = heat_omp
EXE_FULL = $(BUILDDIR)/$(EXE)

all: $(EXE)

$(BUILDDIR):
	@mkdir -p $@

# Just copies the produced executable to Makefile directory
$(EXE): $(EXE_FULL)
	cp $< $@

$(EXE_FULL): $(OBJS) $(HEAT_COMMON_LIB)
	$(FC) $(FCFLAGS) $(LDFLAGS) $(OBJS) $(HEAT_COMMON_LIB) -o $@ $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.F90 | $(BUILDDIR)
	$(FC) $(FCFLAGS) $(FCMODFLAGS) -c $< -o $@

.PHONY: clean
clean: heat-common-clean
	-/bin/rm -rf build $(EXE)
