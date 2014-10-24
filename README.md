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

using namespace std;
using namespace slotty;

void listener(int arg)
{
    cout << "ARG RECEIVED: " << arg << endl;
}

int main(int, char**)
{
    // create the actual event object
    event<int> evt;
    
    // connect 'listener' to the event and get the slot
    slot<int>* slot = evt.connect(&listener);

    // raise the event, 'listener' is called
    evt.raise(0);

    // the slot is being deleted, so the listener is no longer valid
    delete slot;

    // nothing happens
    evt.raise(1);

    return 0;
}
```

Or a more practical use:

```c++
#include <slotty.h> // get slotty

#include <string> // string
#include <functional> // bind
#include <iostream> // cout and endl
#include <memory> // unique_ptr

using namespace std;
using namespace std::placeholders;
using namespace slotty;

// some class that represents a button on a window
class Button
{
private:
    // an automatic pointer, delete's the contents during destruction
    // when this is deleted, the listener is also invalidated
    unique_ptr<event_type::slot_type> slot_;
public:
    // give it a smaller name
    typedef event<int, int> event_type;

    // this constructor is just for convenience of this sample
    Button(const event_type& evt)
    {
        // assign the slot
        this->slot_ =
            std::unique_ptr<typename event_type::slot_type>(
                evt.connect(
                    bind(
                        &Button::on_resized, // the member method
                        this, // the instance to be bound
                        _1, // the first param
                        _2  // the second param
                    )
                )
            );
    }

private:
    // the listener/callback
    void on_resized(int new_width, int new_height)
    {
        cout << "New size: " << new_width << ", " << new_height << endl;
    }
};

int main(int, char**)
{
    // create the event from the typedef
    Button::event_type evt;
    
    // create a new button with the event
    // the listener is already registered
    Button* foo = new Button(evt);

    // raises the event, the 'on_resized' in 'foo' is called
    evt.raise(500, 500);

    // when 'foo' is deleted, the unique_ptr is also deleted 
    // invalidating the slot. i.e. the slot's lifetime is equal 
    // to the lifetime of 'foo'
    delete foo;

    return 0;
}
```

You could even use lambda functions:

```c++
#include <slotty.h> // get slotty
#include <iostream> // for cout and endl

using namespace std;
using namespace slotty;

int main(int, char**)
{
    event<const char*, const char*> some_evt;

    slot<const char*, const char*>* slot =
        some_evt.connect([](const char* a, const char* b) {
            cout << a << " " << b << endl;;
        });

    some_evt.raise("Hello", "World");

    delete slot;

    some_evt.raise("Never", "Printed");

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