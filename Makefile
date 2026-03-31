# =============================================================================
# Makefile — HALIX Assembler v4.0
# Works on: Ubuntu (Colab / CI) and MSYS2 UCRT64 (Windows local)
#
# Targets:
#   make            Build the assembler binary
#   make clean      Remove build artefacts
#   make test       Build then run the full test suite
#   make test-p0    Build then run Pass 0 suite only
#   make test-p1    ...and so on for p1, p2a, p2b, p3, p4, int
#   make rebuild    clean + build
# =============================================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic
SRCDIR   := src
OBJDIR   := build

# Detect OS: Windows (MSYS2) vs Linux/macOS
ifeq ($(OS),Windows_NT)
    BINARY := halixAssembler.exe
    RM     := rm -f
else
    BINARY := halixAssembler
    RM     := rm -f
endif

# ── Source and object lists ───────────────────────────────────────────────────
SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# ── Default target ────────────────────────────────────────────────────────────
.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build OK → $@"

# ── Compile each .cpp to its .o ───────────────────────────────────────────────
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# ── Clean ─────────────────────────────────────────────────────────────────────
.PHONY: clean
clean:
	$(RM) $(OBJS) $(BINARY)
	@rmdir $(OBJDIR) 2>/dev/null || true

.PHONY: rebuild
rebuild: clean all

# ── Test targets ──────────────────────────────────────────────────────────────
.PHONY: test
test: all
	bash tests/run_all_tests.sh

.PHONY: test-p0
test-p0: all
	bash tests/run_all_tests.sh --pass P0

.PHONY: test-p1
test-p1: all
	bash tests/run_all_tests.sh --pass P1

.PHONY: test-p2a
test-p2a: all
	bash tests/run_all_tests.sh --pass P2a

.PHONY: test-p2b
test-p2b: all
	bash tests/run_all_tests.sh --pass P2b

.PHONY: test-p3
test-p3: all
	bash tests/run_all_tests.sh --pass P3

.PHONY: test-p4
test-p4: all
	bash tests/run_all_tests.sh --pass P4

.PHONY: test-int
test-int: all
	bash tests/run_all_tests.sh --pass INT

# ── Help ──────────────────────────────────────────────────────────────────────
.PHONY: help
help:
	@echo "Targets:"
	@echo "  make          Build halixAssembler"
	@echo "  make clean    Remove build artefacts"
	@echo "  make rebuild  clean + build"
	@echo "  make test     Build + run all tests"
	@echo "  make test-p0  Run Pass 0 suite only"
	@echo "  make test-p1  Run Pass 1 suite only"
	@echo "  make test-p2a Run Pass 2a suite only"
	@echo "  make test-p2b Run Pass 2b suite only"
	@echo "  make test-p3  Run Pass 3 suite only"
	@echo "  make test-p4  Run Pass 4 suite only"
	@echo "  make test-int Run integration suite only"
