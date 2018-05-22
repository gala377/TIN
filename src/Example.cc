//
// Created by gajus123 on 23.04.18.
//

#include <queue/FileStorage.h>

#include <iostream>

int main() {
    std::cout << "starting test\n";
    Queue::FileStorage storage("storage_tests");

    char c;
    std::cin >> c;

    storage.add(0, "test_0");
    storage.add(1, "test_1");
    storage.add(2, "test_2");
    storage.add(3, "test_3");
    storage.add(11, "test_4");

    for(Queue::FileStorage::Iterator it = storage.begin();
            it != storage.end();
            ++it) {
        std::cout << "next it\n";
    }
}