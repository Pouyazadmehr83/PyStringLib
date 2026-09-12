CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O3 -Iinclude -Isingle_include

BIN_DIR = bin

all: test examples benchmark

directories:
	@mkdir -p $(BIN_DIR)

test: directories
	$(CXX) $(CXXFLAGS) tests/test_all.cpp -o $(BIN_DIR)/test_all
	@echo "==> Running PyStringLib Test Suite:"
	./$(BIN_DIR)/test_all

repl: directories
	$(CXX) $(CXXFLAGS) tools/pystring_repl.cpp -o $(BIN_DIR)/pystring-repl
	@echo "==> Built REPL: ./$(BIN_DIR)/pystring-repl"

examples: directories
	$(CXX) $(CXXFLAGS) examples/basic_usage.cpp -o $(BIN_DIR)/example_basic
	$(CXX) $(CXXFLAGS) examples/slicing_demo.cpp -o $(BIN_DIR)/example_slicing
	$(CXX) $(CXXFLAGS) examples/chaining_demo.cpp -o $(BIN_DIR)/example_chaining
	@echo "==> Built examples in $(BIN_DIR)/"

benchmark: directories
	$(CXX) $(CXXFLAGS) examples/benchmark.cpp -o $(BIN_DIR)/benchmark
	@echo "==> Running benchmark:"
	./$(BIN_DIR)/benchmark

amalgamate:
	python3 scripts/amalgamate.py

clean:
	rm -rf $(BIN_DIR) test_runner test_single

.PHONY: all directories test examples benchmark amalgamate clean
