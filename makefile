CC = gcc
CFLAGS = -Wall -g
SRC_DIR = datastruct
TEST_DIR = tests
BUILD_DIR = build

all: $(BUILD_DIR)/test_dll

$(BUILD_DIR)/test_dll: $(TEST_DIR)/test_dll.c $(SRC_DIR)/dll.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)
