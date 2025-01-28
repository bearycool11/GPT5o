# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Iinclude
LDFLAGS = -lssl -lcrypto -lcurl
DEBUG_CFLAGS = -g -DDEBUG

# Dynamic Directories
SRC_DIR = ./src
BUILD_DIR = ./build
INCLUDE_DIR = ./include
BINARIES_DIR = ./binaries
LOG_DIR = ./logs
INSTALL_DIR = /opt/pmll

# Source files (search dynamically in the source directory)
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Object files generated from source files
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Target binaries
TARGETS = pmll arc_agi_benchmark pmll_np_solver sat_test api_llama api vector_matrix silo_manager logic_loop custodian free

# Default target: Build all binaries
.PHONY: all
all: $(TARGETS)

# Define rules for each binary and its dependencies
pmll: $(BUILD_DIR)/unified_voice.o $(BUILD_DIR)/pml_logic_loop.o $(BUILD_DIR)/memory_silo.o \
      $(BUILD_DIR)/io_socket.o $(BUILD_DIR)/persistence.o $(BUILD_DIR)/cross_talk.o \
      $(BUILD_DIR)/custodian.o $(BUILD_DIR)/main.o $(BUILD_DIR)/vector_matrix.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

arc_agi_benchmark: $(BUILD_DIR)/arc_agi_benchmark.o $(BUILD_DIR)/vector_matrix.o $(BUILD_DIR)/io_socket.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

pmll_np_solver: $(BUILD_DIR)/pmll_np_solver.o $(BUILD_DIR)/vector_matrix.o $(BUILD_DIR)/io_socket.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

sat_test: $(BUILD_DIR)/SAT_Compare.o $(BUILD_DIR)/Pmll_NP_Solver.o $(BUILD_DIR)/MiniSAT.o \
          $(BUILD_DIR)/generate_3sat_instance.o $(BUILD_DIR)/SAT_Solver.o $(BUILD_DIR)/vector_matrix.o \
          $(BUILD_DIR)/io_socket.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

api_llama: $(BUILD_DIR)/API_Llama.o $(BUILD_DIR)/vector_matrix.o $(BUILD_DIR)/io_socket.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

api: $(BUILD_DIR)/API.o $(BUILD_DIR)/vector_matrix.o $(BUILD_DIR)/io_socket.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

vector_matrix: $(BUILD_DIR)/vector_matrix.o $(BUILD_DIR)/io_socket.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

silo_manager: $(BUILD_DIR)/silo_manager.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

logic_loop: $(BUILD_DIR)/logic_loop.o $(BUILD_DIR)/pml_logic_loop.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

custodian: $(BUILD_DIR)/custodian.o $(BUILD_DIR)/io_socket.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

free: $(BUILD_DIR)/free.o $(BUILD_DIR)/json.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BINARIES_DIR)/$@ $^

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR) $(BINARIES_DIR) $(LOG_DIR)
	@echo "Clean complete."

# Orchestrate Health Check, Payload, and Binary Deployment
.PHONY: orchestrate
orchestrate: all
	@echo "Running orchestration..."
	bash Orchestrate.sh
	@echo "Orchestration completed."

# Deployment to system directory
.PHONY: deploy
deploy: all
	@echo "Deploying binaries to $(INSTALL_DIR)..."
	@sudo mkdir -p $(INSTALL_DIR)
	@sudo install -m 755 $(BINARIES_DIR)/* $(INSTALL_DIR)/
	@echo "Deployment complete."

# Debug build
.PHONY: debug
debug: CFLAGS += $(DEBUG_CFLAGS)
debug: all

# Help
.PHONY: help
help:
	@echo "Makefile commands:"
	@echo "  all                  Build all binaries."
	@echo "  clean                Remove all build artifacts."
	@echo "  orchestrate          Run orchestration script (Orchestrate.sh)."
	@echo "  deploy               Deploy binaries to $(INSTALL_DIR)."
	@echo "  debug                Build with debug flags."
	@echo "  help                 Show this help message."
