#include "../slotty.h"

#include <string>
#include <iostream>

class Button
{
public:
    Button()
    {
        resize().connect([=](int x, int y){ on_resized(x, y); }, resize_evt_pair.slot);
    }

    const slotty::event<int, int>& resize() const
    {
        return resize_evt_pair.event;
    }

    void resize(int x, int y)
    {
        resize_evt_pair.event.raise(x, y);
    }

private:
    slotty::event_slot_pair<int, int> resize_evt_pair;

    void on_resized(int new_width, int new_height)
    {
        std::cout << "New size: " << new_width << ", " << new_height << std::endl;
    }
};

int main(int, char**)
{
    {
        Button btn;
        btn.resize(500, 500);
    }

    std::cin.get();

    return 0;
}