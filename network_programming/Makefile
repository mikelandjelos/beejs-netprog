# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -pthread

# Directories
DIST_DIR = dist

# Default target when EXAMPLE is provided
all: $(DIST_DIR)/$(EXAMPLE)

# Create dist directory if it doesn't exist
$(DIST_DIR):
	mkdir -p $(DIST_DIR)

# Build the specified example
$(DIST_DIR)/$(EXAMPLE): $(EXAMPLE)/$(EXAMPLE).c | $(DIST_DIR)
	$(CC) $(CFLAGS) $< -o $@
	@echo "Built: $@"
	@echo "Run with: ./dist/$(EXAMPLE)"

# Show usage and available examples
help:
	@echo "Usage: make EXAMPLE=example_name"
	@echo "Examples:"
	@echo "  make EXAMPLE=ip_address_conversions"
	@echo ""
	@echo "Available examples:"
	@find . -maxdepth 1 -type d -name "*" ! -name "." ! -name "$(DIST_DIR)" | sed 's|./||' | sort

# Clean specific example
clean:
ifdef EXAMPLE
	rm -f $(DIST_DIR)/$(EXAMPLE)
else
	rm -rf $(DIST_DIR)
endif

# Clean everything
clean-all:
	rm -rf $(DIST_DIR)

# Rebuild specific example
rebuild: clean all

# Mark phony targets
.PHONY: all help clean clean-all rebuild