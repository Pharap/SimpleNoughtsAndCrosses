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

#include <cstddef>

template< typename Type, std::size_t WidthValue, std::size_t HeightValue >
class Grid
{
public:
	using ValueType = Type;	
	using SizeType = std::size_t;

public:
	constexpr static const SizeType Width = WidthValue;
	constexpr static const SizeType Height = HeightValue;
	constexpr static const SizeType Count = Width * Height;

	constexpr static const SizeType FirstX = 0;
	constexpr static const SizeType FirstY = 0;
	constexpr static const SizeType LastX = Width - 1;
	constexpr static const SizeType LastY = Height - 1;

private:
	ValueType items[Count];

	inline SizeType flattenIndex(SizeType x, SizeType y) const
	{
		return (Width * y) + x;
	}

public:
	constexpr SizeType getCount(void) const;
	constexpr SizeType getWidth(void) const;
	constexpr SizeType getHeight(void) const;

	ValueType & getItem(SizeType x, SizeType y);
	const ValueType & getItem(SizeType x, SizeType y) const;

	void fill(const ValueType & value); // O(n)
	void clear(void); // O(n)
};

template< typename Type, std::uint8_t Width, std::uint8_t Height >
constexpr std::uint16_t Grid<Type, Width, Height>::getCount(void) const
{
	return Grid<Type, Width, Height>::Count;
}

template< typename Type, std::uint8_t Width, std::uint8_t Height >
constexpr std::uint8_t Grid<Type, Width, Height>::getWidth(void) const
{
	return Grid<Type, Width, Height>::Width;
}

template< typename Type, std::uint8_t Width, std::uint8_t Height >
constexpr std::uint8_t Grid<Type, Width, Height>::getHeight(void) const
{
	return Grid<Type, Width, Height>::Height;
}

template< typename Type, std::uint8_t Width, std::uint8_t Height >
typename Grid<Type, Width, Height>::ValueType & Grid<Type, Width, Height>::getItem(SizeType x, SizeType y)
{
	return this->items[flattenIndex(x, y)];
}

template< typename Type, std::uint8_t Width, std::uint8_t Height >
const typename Grid<Type, Width, Height>::ValueType & Grid<Type, Width, Height>::getItem(SizeType x, SizeType y) const
{
	return this->items[flattenIndex(x, y)];
}

template< typename Type, std::uint8_t Width, std::uint8_t Height >
void Grid<Type, Width, Height>::fill(const ValueType & value)
{
	for(SizeType i = 0; i < Count; ++i)
		items[i] = value;
}

template< typename Type, std::uint8_t Width, std::uint8_t Height >
void Grid<Type, Width, Height>::clear(void) // O(n)
{
	for (SizeType = 0; i < Count; ++i)
		(&this->items[i])->~ValueType();
}
