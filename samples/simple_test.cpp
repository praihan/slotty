#include "../slotty.h"

#include <iostream>

void listener(int arg)
{
    static bool fail = false;
    if (fail)
    {
        throw std::exception("FAILED");
    }
    fail = true;
    std::cout << "ARGS RECEIVED: " << arg << std::endl;
}

int main(int, char**)
{
    using event_type = slotty::event<int>;

    event_type evt;

    {
        event_type::slot_type slot;
        evt.connect(&listener, slot);
        evt.raise(0);
    }

    evt.raise(1);

    std::cin.get();

    return 0;
}