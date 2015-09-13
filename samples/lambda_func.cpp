#include "../slotty.h"

#include <iostream>

template<class T>
struct identity{ typedef T type; };

int main(int, char**)
{
    slotty::event<const char*, const char*> some_evt;

    {
        identity<decltype(some_evt)>::type::slot_type slot;

        some_evt.connect([](const char* a, const char* b) {
            std::cout << a << " " << b << std::endl;
        }, slot);

        some_evt.raise("Hello", "World");
    }
    // slot deleted

    some_evt.raise("Never", "Printed");

    std::cin.get();

    return 0;
}