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

#include "../Buttons.h"
#include "../Controller.h"

class ButtonController : public Controller
{
protected:
	Buttons getCurrentButtons(void) override
	{
		Buttons buttons = Buttons::None;

		if(Pokitto::heldStates[BTN_A] != 0)
			buttons |= Buttons::A;

		if(Pokitto::heldStates[BTN_B] != 0)
			buttons |= Buttons::B;

		if(Pokitto::heldStates[BTN_UP] != 0)
			buttons |= Buttons::Up;

		if(Pokitto::heldStates[BTN_DOWN] != 0)
			buttons |= Buttons::Down;

		if(Pokitto::heldStates[BTN_LEFT] != 0)
			buttons |= Buttons::Left;

		if(Pokitto::heldStates[BTN_RIGHT] != 0)
			buttons |= Buttons::Right;

		return buttons;
	}
};