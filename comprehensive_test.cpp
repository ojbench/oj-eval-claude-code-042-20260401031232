#include "SpeedCircularList.h"
#include <iostream>
#include <cassert>
#include <vector>

void test_basic_construction() {
    std::cout << "Test 1: Basic construction" << std::endl;
    std::vector<int> bounds = {100, 200, 300, 400, 500};
    SpeedCircularLinkedList<int, 500> list(bounds);
    assert(list.size() == 5);
    std::cout << "PASSED" << std::endl << std::endl;
}

void test_single_node() {
    std::cout << "Test 2: Single node" << std::endl;
    std::vector<int> bounds = {100};
    SpeedCircularLinkedList<int, 100> list(bounds);
    assert(list.size() == 1);
    list.put("key1", 10);
    assert(list.get("key1") == 10);
    list.print();
    std::cout << "PASSED" << std::endl << std::endl;
}

void test_put_get_operations() {
    std::cout << "Test 3: Put and Get operations" << std::endl;
    std::vector<int> bounds = {100, 200, 300, 400};
    SpeedCircularLinkedList<int, 400> list(bounds);

    // Insert multiple values
    for (int i = 0; i < 20; ++i) {
        list.put("key" + std::to_string(i), i * 10);
    }

    // Verify retrieval
    for (int i = 0; i < 20; ++i) {
        int val = list.get("key" + std::to_string(i));
        assert(val == i * 10);
    }

    list.print();
    std::cout << "PASSED" << std::endl << std::endl;
}

void test_large_data() {
    std::cout << "Test 4: Large data" << std::endl;
    std::vector<int> bounds;
    for (int i = 1; i <= 100; ++i) {
        bounds.push_back(i * 10);
    }

    SpeedCircularLinkedList<int, 1000> list(bounds);
    assert(list.size() == 100);

    // Insert 500 key-value pairs
    for (int i = 0; i < 500; ++i) {
        list.put("data" + std::to_string(i), i);
    }

    // Verify some values
    for (int i = 0; i < 500; i += 10) {
        int val = list.get("data" + std::to_string(i));
        assert(val == i);
    }

    std::cout << "PASSED" << std::endl << std::endl;
}

void test_string_values() {
    std::cout << "Test 5: String values" << std::endl;
    std::vector<int> bounds = {50, 100, 150, 200};
    SpeedCircularLinkedList<std::string, 200> list(bounds);

    list.put("name", "Alice");
    list.put("city", "Shanghai");
    list.put("country", "China");

    assert(list.get("name") == "Alice");
    assert(list.get("city") == "Shanghai");
    assert(list.get("country") == "China");

    list.print();
    std::cout << "PASSED" << std::endl << std::endl;
}

void test_nonexistent_keys() {
    std::cout << "Test 6: Nonexistent keys" << std::endl;
    std::vector<int> bounds = {100, 200};
    SpeedCircularLinkedList<int, 200> list(bounds);

    // Get non-existent key should return default value
    int val = list.get("nonexistent");
    assert(val == 0);

    std::cout << "PASSED" << std::endl << std::endl;
}

int main() {
    test_basic_construction();
    test_single_node();
    test_put_get_operations();
    test_string_values();
    test_nonexistent_keys();
    test_large_data();

    std::cout << "================================" << std::endl;
    std::cout << "All tests passed successfully!" << std::endl;
    std::cout << "================================" << std::endl;

    return 0;
}
