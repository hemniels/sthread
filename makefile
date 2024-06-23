CC = gcc
CFLAGS = -Wall -g
SRC_DIR = datastruct
TEST_DIR = tests
BUILD_DIR = build

all: $(BUILD_DIR)/test_sthread

$(BUILD_DIR)/test_sthread: $(TEST_DIR)/test_sthread.c $(SRC_DIR)/sthread.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)
