#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <list>
#include <vector>
#include <random>
#include <valarray>

#include "Object.h"

enum class Types {
    PRINTER,
    MACHINE,
    COFFEE
};

class Task {
private:
    std::list<Types> types;
    int position = 0;
    const Object* next;
    std::vector<const Object*> used;
public:
    explicit Task(const std::list<Types> &types) : types(types) {}

    void start() {
        this->position = 0;
        this->next = nullptr;
        this->used.clear();
    }

    bool pickRandom(const std::unordered_map<Types, std::vector<Object>>& objects) {

        // If finished
        if (this->position >= types.size()) {
            return true;
        }

        auto typeIt = types.begin();
        std::advance(typeIt, position);

        const auto it = objects.find(*typeIt);
        if (it == objects.end() || it->second.empty()) {
            throw std::runtime_error("No objects found for the specified type.");
        }

        const std::vector<Object>& objectList = it->second;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dis(0, objectList.size() - 1);

        size_t randomIndex = dis(gen);

        for (const Object* object : this->used) {
            if (object == &objectList[randomIndex]) {
                pickRandom(objects);
                return false;
            }
        }

        this->next = &objectList[randomIndex];
        this->used.emplace_back(this->next);
        this->position++;

        return false;
    }

    const Object* getNextObject() {
        return this->next;
    }
};

#endif //TASK_H
