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

#define DISABLEAVRMIN
#include <Pokitto.h>

#include <algorithm>
#include <utility>
#include <cstddef>
#include <cstdint>

#include "Grid.h"
#include "Point.h"

class Game
{
public:
	enum class Cell : std::uint8_t
	{
		None,
		Nought,
		Cross,
	};

	enum class Status : std::uint8_t
	{
		Unfinished,
		NoughtsWins,
		CrossesWins,
		Draw,
	};

public:
	using CellGrid = Grid<Cell, 3, 3>;
	using PointType = Point<std::size_t>;

private:
	CellGrid grid;
	PointType selector;
	Cell currentTurn = Cell::Nought;
	Status status = Status::Unfinished;

public:
	void run(void);

private:
	void update(void);
	void draw(void);

	std::pair<bool, Cell> getWinner(void) const;
	bool hasAnyEmptyCells(void) const;
	Status calculateStatus(void) const;

	void drawGrid(void);
	void drawStatus(void);
};

void Game::run(void)
{
	Pokitto::Core::begin();

	while (Pokitto::Core::isRunning())
		if (Pokitto::Core::update())
		{
			this->update();
			this->draw();
		}
}

void Game::update(void)
{
	if(Pokitto::Buttons::held(BTN_LEFT, 1))
		if(this->selector.x > CellGrid::FirstX)
			--this->selector.x;

	if(Pokitto::Buttons::held(BTN_RIGHT, 1))
		if(this->selector.x < CellGrid::LastX)
			++this->selector.x;

	if(Pokitto::Buttons::held(BTN_UP, 1))
		if(this->selector.y > CellGrid::FirstY)
			--this->selector.y;

	if(Pokitto::Buttons::held(BTN_DOWN, 1))
		if(this->selector.y < CellGrid::LastY)
			++this->selector.y;

	if(Pokitto::Buttons::held(BTN_A, 1))
		if(this->grid.getItem(this->selector.x, this->selector.y) == Cell::None)
		{
			this->grid.getItem(this->selector.x, this->selector.y) = this->currentTurn;
			switch(this->currentTurn)
			{
				case Cell::Nought:
					this->currentTurn = Cell::Cross;
					break;
				case Cell::Cross:
					this->currentTurn = Cell::Nought;
					break;
				default: break;
			}

			this->status = this->calculateStatus();
		}

	if(Pokitto::Buttons::held(BTN_B, 10) && (this->status != Status::Unfinished))
	{
		this->grid.fill(Cell::None);
		this->status = Status::Unfinished;
	}
}

void Game::draw(void)
{
	drawGrid();
	drawStatus();
}

std::pair<bool, Game::Cell> Game::getWinner(void) const
{
	static const PointType winningSets[][3] =
	{
		// Vertical
		{ PointType(0, 0), PointType(0, 1), PointType(0, 2) },
		{ PointType(1, 0), PointType(1, 1), PointType(1, 2) },
		{ PointType(2, 0), PointType(2, 1), PointType(2, 2) },

		// Horizontal
		{ PointType(0, 0), PointType(1, 0), PointType(2, 0) },
		{ PointType(0, 1), PointType(1, 1), PointType(2, 1) },
		{ PointType(0, 2), PointType(1, 2), PointType(2, 2) },

		// Diagonal
		{ PointType(0, 0), PointType(1, 1), PointType(2, 2) },
		{ PointType(2, 0), PointType(1, 1), PointType(0, 2) },
	};

	for(auto const & line : winningSets)
	{
		const PointType start = line[0];
		const Cell type = this->grid.getItem(start.x, start.y);

		if(type == Cell::None)
			continue;

		bool success = true;
		for(std::size_t i = 0; i < 3; ++i)
			if(this->grid.getItem(line[i].x, line[i].y) != type)
			{
				success = false;
				break;
			}

		if(success)
			return std::make_pair(true, type);
	}

	return std::make_pair(false, Cell::None);
}

bool Game::hasAnyEmptyCells(void) const
{
	for(std::size_t y = 0; y < CellGrid::Height; ++y)
		for(std::size_t x = 0; x < CellGrid::Width; ++x)
			if(this->grid.getItem(x, y) == Cell::None)
				return true;

	return false;
}

Game::Status Game::calculateStatus(void) const
{
	const auto winner = getWinner();
	const bool success = winner.first;

	if(success)
	{
		const Cell type = winner.second;
		switch(type)
		{
			case Cell::Nought:
				return Status::NoughtsWins;
			case Cell::Cross:
				return Status::CrossesWins;
			default:
				return Status::Unfinished;
		}
	}

	return (this->hasAnyEmptyCells()) ? Status::Unfinished : Status::Draw;
}

void Game::drawGrid(void)
{
	// Drawing parameters for easy modification
	constexpr std::int16_t xGap = 8;
	constexpr std::int16_t yGap = 8;
	constexpr std::int16_t cellWidth = 32;
	constexpr std::int16_t cellHeight = 32;

	//const int blackIndex = 0;
	constexpr std::uint8_t whiteIndex = 1;

	constexpr std::int16_t boardWidth = (cellWidth * CellGrid::Width) + (xGap * 2);
	constexpr std::int16_t boardHeight = (cellHeight * CellGrid::Height) + (yGap * 2);

	const std::int16_t xOffset = (Pokitto::Display::getWidth() - boardWidth) / 2;
	const std::int16_t yOffset = (Pokitto::Display::getHeight() - boardHeight) / 2;

	const std::int16_t leftLineX = xOffset + (cellWidth * 1) + (xGap * 0) + (xGap / 2);
	const std::int16_t rightLineX = xOffset + (cellWidth * 2) + (xGap * 1) + (xGap / 2);

	const std::int16_t topLineY = yOffset + (cellHeight * 1) + (yGap * 0) + (yGap / 2);
	const std::int16_t bottomLineY = yOffset + (cellHeight * 2) + (yGap * 1) + (yGap / 2);

	// Draw lines
	Pokitto::Display::setColor(whiteIndex);

	Pokitto::Display::drawLine(leftLineX, yOffset - 4, leftLineX, yOffset + boardHeight + 8);
	Pokitto::Display::drawLine(rightLineX, yOffset - 4, rightLineX, yOffset + boardHeight + 8);

	Pokitto::Display::drawLine(xOffset - 4, topLineY, xOffset + boardWidth + 8, topLineY);
	Pokitto::Display::drawLine(xOffset - 4, bottomLineY, xOffset + boardWidth + 8, bottomLineY);


	// Draw grid
	for(std::size_t y = 0; y < CellGrid::Height; ++y)
		for(std::size_t x = 0; x < CellGrid::Width; ++x)
		{
			const std::int16_t cellX = xOffset + ((cellWidth + xGap) * x);
			const std::int16_t cellY = yOffset + ((cellHeight + yGap) * y);

			// Draw symbol
			switch(grid.getItem(x, y))
			{
				case Cell::Nought:
				{
					constexpr std::int16_t halfCellWidth = cellWidth / 2;
					constexpr std::int16_t halfCellHeight = cellHeight / 2;
					const std::int16_t cellCentreX = cellX + halfCellWidth;
					const std::int16_t cellCentreY = cellY + halfCellHeight;
					const std::int16_t radius = std::min(halfCellWidth, halfCellHeight) - 1;

					Pokitto::Display::drawCircle(cellCentreX, cellCentreY, radius);
					break;
				}
				case Cell::Cross:
				{
					const std::int16_t left = cellX + 1;
					const std::int16_t top = cellY + 1;
					const std::int16_t right = cellX + (cellWidth - 1);
					const std::int16_t bottom = cellY + (cellHeight - 1);

					Pokitto::Display::drawLine(left, top, right, bottom);
					Pokitto::Display::drawLine(right, top, left, bottom);
					break;
				}
				default:
					break;
			}

			// Draw selector
			if((x == this->selector.x) && (y == this->selector.y))
				Pokitto::Display::drawRect(cellX, cellY, cellWidth, cellHeight);
		}
}

void Game::drawStatus(void)
{
	Pokitto::Display::setCursor(16, 8);
	switch(this->status)
	{
		case Status::Unfinished:
		{
			switch(this->currentTurn)
			{
				case Cell::Nought:
				{
					Pokitto::Display::print("Player: Noughts");
					break;
				}
				case Cell::Cross:
				{
					Pokitto::Display::print("Player: Crosses");
					break;
				}
				case Cell::None:
				{
					Pokitto::Display::print("Error!?");
					break;
				}
			}
			break;
		}
		case Status::NoughtsWins:
		{
			Pokitto::Display::print("Noughts Wins!");
			break;
		}
		case Status::CrossesWins:
		{
			Pokitto::Display::print("Crosses Wins!");
			break;
		}
		case Status::Draw:
		{
			Pokitto::Display::print("Draw!");
			break;
		}
	}
}
