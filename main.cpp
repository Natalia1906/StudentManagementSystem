/**
 * @file main.cpp
 * @brief Точка входа в программу Student Management System.
 */

#include "models.h"

int main() {
    auto alice = std::make_shared<Student>("alice","123",std::vector<int>{4,5,3});
    auto bob   = std::make_shared<Student>("bob","123",std::vector<int>{5,4,4});

    auto tina  = std::make_shared<Teacher>("tina","teach",
                    std::vector<std::shared_ptr<Student>>{alice,bob});
    auto paul  = std::make_shared<Parent>("paul","parent",alice);

    System sys;
    sys.add(alice); sys.add(bob); sys.add(tina); sys.add(paul);
    sys.run();
    return 0;
}