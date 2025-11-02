# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -pthread

# Directories
DIST_DIR = .dist

# Get all .c files in the example directory
C_FILES = $(wildcard $(EXAMPLE)/*.c)
# Convert to executable names in dist/example_name/
EXECUTABLES = $(patsubst $(EXAMPLE)/%.c,$(DIST_DIR)/$(EXAMPLE)/%,$(C_FILES))

# Default target when EXAMPLE is provided
all: check-example $(EXECUTABLES)

# Check if EXAMPLE is defined
check-example:
ifndef EXAMPLE
	@echo "Error: EXAMPLE variable not set"
	@echo "Usage: make EXAMPLE=example_name"
	@echo "Run 'make help' for more information"
	@exit 1
endif

# Create dist directory structure
$(DIST_DIR)/$(EXAMPLE):
	mkdir -p $(DIST_DIR)/$(EXAMPLE)

# Build pattern rule for any .c file in the example directory
$(DIST_DIR)/$(EXAMPLE)/%: $(EXAMPLE)/%.c | $(DIST_DIR)/$(EXAMPLE)
	$(CC) $(CFLAGS) $< -o $@
	@echo "Built: $@"

# Show usage and available examples
help:
	@echo "Usage: make EXAMPLE=example_name"
	@echo "Examples:"
	@echo "  make EXAMPLE=ip_address_conversions"
	@echo ""
	@echo "This will compile all .c files in the example directory to:"
	@echo "  dist/example_name/file1"
	@echo "  dist/example_name/file2"
	@echo "  etc."
	@echo ""
	@echo "Available examples:"
	@find . -maxdepth 1 -type d -name "*" ! -name "." ! -name "$(DIST_DIR)" | sed 's|./||' | sort

# List what would be built for a specific example
list: check-example
	@echo "Example: $(EXAMPLE)"
	@echo "Source files found:"
	@for file in $(C_FILES); do echo "  $$file"; done
	@echo "Will create executables:"
	@for exe in $(EXECUTABLES); do echo "  $$exe"; done

# Run all executables for the example
run: check-example $(EXECUTABLES)
	@echo "Running all executables for $(EXAMPLE):"
	@for exe in $(EXECUTABLES); do \
		echo "=== Running $$exe ==="; \
		./$$exe; \
		echo; \
	done

# Clean specific example
clean: check-example
	rm -rf $(DIST_DIR)/$(EXAMPLE)

# Clean everything
clean-all:
	rm -rf $(DIST_DIR)

# Rebuild specific example
rebuild: clean all

# Mark phony targets
.PHONY: all check-example help list run clean clean-all rebuild