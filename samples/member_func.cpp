#include "../slotty.h"

#include <string>
#include <functional>
#include <iostream>
#include <memory>

class Button
{
public:
	typedef slotty::event<int, int> event_type;

	Button(const event_type& evt)
	{
		this->slot_ =
			std::unique_ptr<typename event_type::slot_type>(
				evt.connect(
					std::bind(
						&Button::on_resized,
						this,
						std::placeholders::_1,
						std::placeholders::_2
					)
				)
			);
	}

private:
	void on_resized(int new_width, int new_height)
	{
		std::cout << "New size: " << new_width << ", " << new_height << std::endl;
	}

private:
	std::unique_ptr<event_type::slot_type> slot_;
};

int main(int, char**)
{
	Button::event_type evt;
	Button* foo = new Button(evt);

	evt.raise(500, 500);

	delete foo;

	std::cin.get();

	return 0;
}