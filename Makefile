BIN_NAME := floater

COMPTYPE ?= debug

OBJDIR := ./build/$(COMPTYPE)/obj
BINDIR := ./build/$(COMPTYPE)

INCLUDEDIR := ./include
SRCDIR := ./src
LIBDIR := ./lib

SRCS := $(wildcard $(SRCDIR)/*.cxx) $(wildcard $(LIBDIR)/*.cxx)
OBJS := $(subst $(LIBDIR)/,$(OBJDIR)/,$(subst $(SRCDIR)/,$(OBJDIR)/,$(SRCS:.cxx=.o)))
DEPS := $(subst $(LIBDIR)/,$(OBJDIR)/,$(subst $(SRCDIR)/,$(OBJDIR)/,$(SRCS:.cxx=.d)))

CXX := clang++
CXXFLAGS := -Wall -Wextra -std=c++17 -fcolor-diagnostics -fansi-escape-codes -I$(INCLUDEDIR)

ifeq ($(COMPTYPE), release)
CXXFLAGS += -O3
else ifeq ($(COMPTYPE), debug)
CXXFLAGS += -Og -g
else
$(error Bad compile type: $(COMPTYPE))
endif

MKDIR := @mkdir -p
RMDIR := @rm -fr

.PHONY: all build debug release clean list
.SUFFIXES:

list:
	@echo "Targets: all debug release clean list"

all: debug release

build: $(BINDIR)/$(BIN_NAME)

debug:
	$(MAKE) build COMPTYPE=debug

release:
	$(MAKE) build COMPTYPE=release

clean:
	$(RMDIR) ./build

$(BINDIR)/$(BIN_NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: */%.cxx $(OBJDIR)/%.d
	$(MKDIR) $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.d: */%.cxx
	$(MKDIR) $(OBJDIR)
	$(CXX) $(CXXFLAGS) -MMD -MF $@ -c -o $(subst .d,.o, $@) $<

ifneq ($(filter build obj,$(MAKECMDGOALS)),)
    -include $(DEPS)
endif
