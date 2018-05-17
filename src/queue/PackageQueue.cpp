
#include "PackageQueue.h"

PackageQueue::PackageQueue(const std::string &backup_file) : backup_file(backup_file) {}

PackageQueue::PackageQueue(const std::string &backup_file, std::list<Package *> data) : backup_file(
        backup_file) {
    for (auto elem: data) {
        waiting.push_back(elem);
        FileReader::write(backup_file, elem);
    }
}

void PackageQueue::push(Package *pkg) {
    waiting.push_back(pkg);
    FileReader::write(backup_file, pkg);
}

Package* PackageQueue::pop() {
    if (!waiting.empty()) {
        sent.push_back(waiting.front());
        waiting.pop_front();
        return sent.back();
    }
}

void PackageQueue::remove(int id) {
    for (auto elem = sent.begin(); elem != sent.end(); elem++)
        if ((*elem)->getId() == id) {
            FileReader::remove(backup_file, *elem);
            sent.erase(elem);
            return;
        }
    for (auto elem = waiting.begin(); elem != waiting.end(); elem++)
        if ((*elem)->getId() == id) {
            FileReader::remove(backup_file, *elem);
            sent.erase(elem);
            return;
        }
}

bool PackageQueue::isEmpty() {
    return waiting.empty() && sent.empty();
}

bool PackageQueue::isSentEmpty() {
    return sent.empty();
}

unsigned long PackageQueue::getSize() {
    return waiting.size() + sent.size();
}

unsigned long PackageQueue::getWaitingSize() {
    return waiting.size();
}

void PackageQueue::pushWithoutSaving(Package *pkg) {
    waiting.push_back(pkg);
}
