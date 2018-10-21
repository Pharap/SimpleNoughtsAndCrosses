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

#include <cstdint>

enum class Buttons : std::uint8_t
{
	None = 0,
	Up = (1 << 0),
	Down = (1 << 1),
	Right = (1 << 2),
	Left = (1 << 3),
	A = (1 << 4),
	B = (1 << 5),
};

constexpr inline Buttons operator ~(Buttons value)
{
	return static_cast<Buttons>(~static_cast<std::uint8_t>(value));
}

constexpr inline Buttons operator |(Buttons left, Buttons right)
{
	return static_cast<Buttons>(static_cast<std::uint8_t>(left) | static_cast<std::uint8_t>(right));
}

constexpr inline Buttons operator &(Buttons left, Buttons right)
{
	return static_cast<Buttons>(static_cast<std::uint8_t>(left) & static_cast<std::uint8_t>(right));
}

constexpr inline Buttons operator ^(Buttons left, Buttons right)
{
	return static_cast<Buttons>(static_cast<std::uint8_t>(left) ^ static_cast<std::uint8_t>(right));
}

inline Buttons & operator |=(Buttons & left, Buttons right)
{
	left = (left | right);
	return left;
}

inline Buttons & operator &=(Buttons & left, Buttons right)
{
	left = (left & right);
	return left;
}

inline Buttons & operator ^=(Buttons & left, Buttons right)
{
	left = (left ^ right);
	return left;
}

inline void setButtons(Buttons & value, Buttons buttons)
{
	value |= buttons;
}

inline void clearButtons(Buttons & value, Buttons buttons)
{
	value &= ~buttons;
}

inline void setButtons(Buttons & value, Buttons buttons, bool bitValue)
{
	if(bitValue)
		value |= buttons;
	else
		value &= ~buttons;
}