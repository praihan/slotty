Slotty
===

The official **git** repository for **Slotty**,

A **very tiny**, **header-only** framework for **event-driven** models in **C++11**, with **zero dependencies**.

***

###Table of Contents

* [Slotty](#slotty)
    * [Usage](#usage)
    * [Installation](#installation)
    * [License](#license)
    * [Authors](#authors)

***

### Usage

Sample usages can all be found in the [`samples`](samples) directory.

A small and simple example:

```c++
#include <slotty.h> // get slotty
#include <iostream> // for cout and endl

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
```

Or a more practical use:

```c++
#include <slotty.h> // get slotty

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
```

You could even use lambda functions:

```c++
#include <slotty.h> // get slotty
#include <iostream> // for cout and endl

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
```

***

### Installation

Copy the file `slotty.h` to your includes directory and you are good to go! This is a **header-only** library, **no linking required**. Just make sure your compiler is **C++11 compliant** *(especially supporting variadic templates)*.

***

### License

This is project is licensed under the Apache License 2.0. You can find a copy of the terms in the [`LICENSE`](LICENSE) file or on the [Apache website](http://www.apache.org/licenses/LICENSE-2.0.html).

***

### Authors

* Pranjal Raihan

Check out the [`AUTHORS`](AUTHORS) for the most recently updated list.