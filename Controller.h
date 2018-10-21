#pragma once

/*
   Copyright (C) 2018 Pharap (@Pharap)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "Buttons.h"

// This is the sort of thing I'd like to see make its way into the Pokitto library

class Controller
{
private:
	Buttons previous;
	Buttons current;

protected:
	virtual Buttons getCurrentButtons(void) = 0;

public:
	bool isPressed(Buttons buttons) const
	{
		return ((this->current & buttons) != Buttons::None);
	}

	bool isReleased(Buttons buttons) const
	{
		return ((this->current & buttons) == Buttons::None);
	}

	bool justPressed(Buttons buttons) const
	{
		return ((this->current & buttons) != Buttons::None) && ((this->previous & buttons) == Buttons::None);
	}

	bool justReleased(Buttons buttons) const
	{
		return ((this->current & buttons) == Buttons::None) && ((this->previous & buttons) != Buttons::None);
	}

	void update(void)
	{
		this->previous = this->current;
		this->current = this->getCurrentButtons();
	}
};