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
	slotty::event<int> evt;

	slotty::slot<int>* slot = evt.connect(&listener);

	evt.raise(0);

	delete slot;

	evt.raise(1);

	std::cin.get();

	return 0;
}