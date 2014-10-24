#include "../slotty.h"

#include <iostream>

int main(int, char**)
{
	slotty::event<const char*, const char*> some_evt;

	slotty::slot<const char*, const char*>* slot =
		some_evt.connect([](const char* a, const char* b) {
			std::cout << a << " " << b << std::endl;;
		});

	some_evt.raise("Hello", "World");

	delete slot;

	some_evt.raise("Never", "Printed");

	std::cin.get();

	return 0;
}