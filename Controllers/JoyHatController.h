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

#include <mbed.h>

template<typename JoyHatSettings> class JoyHatController;

struct CommonJoyHatSettings
{
	constexpr static PinName buttonAPin = EXT15;
	constexpr static PinName buttonBPin = EXT14;
	constexpr static PinName axisXPin = EXT1;
	constexpr static PinName axisYPin = EXT2;

	constexpr static float axisXRight = 0.4f;
	constexpr static float axisXLeft = 0.6f;
	constexpr static float axisYUp = 0.4f;
	constexpr static float axisYDown = 0.6f;
};

using CommonJoyHatController = JoyHatController<CommonJoyHatSettings>;

template<typename JoyHatSettings>
class JoyHatController : public Controller
{
public:
	using Settings = JoyHatSettings;
	constexpr static PinName buttonAPin = Settings::buttonAPin;
	constexpr static PinName buttonBPin = Settings::buttonBPin;
	constexpr static PinName axisXPin = Settings::axisXPin;
	constexpr static PinName axisYPin = Settings::axisYPin;

	constexpr static float axisXRight = Settings::axisXRight;
	constexpr static float axisXLeft = Settings::axisXLeft;
	constexpr static float axisYUp = Settings::axisYUp;
	constexpr static float axisYDown = Settings::axisYDown;

private:
	mbed::DigitalIn buttonA { buttonAPin };
	mbed::DigitalIn buttonB { buttonBPin };
	mbed::AnalogIn axisX { axisXPin };
	mbed::AnalogIn axisY { axisYPin };

protected:
	Buttons getCurrentButtons(void) override
	{
		Buttons buttons = Buttons::None;
		
		if(this->buttonA.read() != 0)
			buttons |= Buttons::A;

		if(this->buttonB.read() != 0)
			buttons |= Buttons::B;

		const auto x = this->axisX.read();

		if(x < axisXRight)
			buttons |= Buttons::Right;

		if(x > axisXLeft)
			buttons |= Buttons::Left;

		const auto y = this->axisY.read();

		if(y < axisYUp)
			buttons |= Buttons::Up;

		if(y > axisYDown)
			buttons |= Buttons::Down;

		return buttons;
	}
};