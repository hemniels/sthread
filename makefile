CC = gcc
CFLAGS = -Wall -g
SRC_DIR = datastruct
TEST_DIR = tests
BUILD_DIR = build

all: $(BUILD_DIR)/test_procqueue

$(BUILD_DIR)/test_procqueue: $(TEST_DIR)/test_procqueue.c $(SRC_DIR)/procqueue.c $(SRC_DIR)/dll.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)
