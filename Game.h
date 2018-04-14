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

#include "Grid.h"
#include "Point.h"

class Game
{
public:
	enum class Cell : unsigned char
	{
		None,
		Nought,
		Cross,
	};

	enum class Status : unsigned char
	{
		Unfinished,
		NoughtsWins,
		CrossesWins,
		Draw,
	};

public:
	using CellGrid = Grid<Cell, 3, 3>;
	using PointType = Point<unsigned int>;

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
	{
		if (Pokitto::Core::update())
		{
			this->update();
			this->draw();
		}
	}
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
		for(int i = 0; i < 3; ++i)
		{
			if(this->grid.getItem(line[i].x, line[i].y) != type)
			{
				success = false;
				break;
			}
		}

		if(success)
		{
			return std::make_pair(true, type);
		}
	}

	return std::make_pair(false, Cell::None);
}

bool Game::hasAnyEmptyCells(void) const
{
	for(int y = 0; y < 3; ++y)
	{
		for(int x = 0; x < 3; ++x)
		{
			if(this->grid.getItem(x, y) == Cell::None)
			{
				return true;
			}
		}
	}

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

	if(this->hasAnyEmptyCells())
	{
		return Status::Unfinished;
	}
	else
	{
		return Status::Draw;
	}
}

void Game::update(void)
{
	if(Pokitto::Buttons::held(BTN_LEFT, 1))
	{
		if(this->selector.x > 0)
			--this->selector.x;
	}

	if(Pokitto::Buttons::held(BTN_RIGHT, 1))
	{
		if(this->selector.x < 2)
			++this->selector.x;
	}

	if(Pokitto::Buttons::held(BTN_UP, 1))
	{
		if(this->selector.y > 0)
			--this->selector.y;
	}

	if(Pokitto::Buttons::held(BTN_DOWN, 1))
	{
		if(this->selector.y < 2)
			++this->selector.y;
	}

	if(Pokitto::Buttons::held(BTN_A, 1))
	{
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
	}

	if(Pokitto::Buttons::held(BTN_B, 10) && this->status != Status::Unfinished)
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

void Game::drawGrid(void)
{
	// Drawing parameters for easy modification
	const int xGap = 8;
	const int yGap = 8;
	const int cellWidth = 32;
	const int cellHeight = 32;

	//const int blackIndex = 0;
	const int whiteIndex = 1;

	const int boardWidth = (cellWidth * 3 + xGap * 2);
	const int boardHeight = (cellHeight * 3 + yGap * 2);

	const int xOffset = (Pokitto::Display::getWidth() - boardWidth) / 2;
	const int yOffset = (Pokitto::Display::getHeight() - boardHeight) / 2;

	const int leftLineX = xOffset + (cellWidth * 1) + (xGap * 0) + (xGap / 2);
	const int rightLineX = xOffset + (cellWidth * 2) + (xGap * 1) + (xGap / 2);

	const int topLineY = yOffset + (cellHeight * 1) + (yGap * 0) + (yGap / 2);
	const int bottomLineY = yOffset + (cellHeight * 2) + (yGap * 1) + (yGap / 2);

	// Draw lines
	Pokitto::Display::setColor(whiteIndex);

	Pokitto::Display::drawLine(leftLineX, yOffset - 4, leftLineX, yOffset + boardHeight + 8);
	Pokitto::Display::drawLine(rightLineX, yOffset - 4, rightLineX, yOffset + boardHeight + 8);

	Pokitto::Display::drawLine(xOffset - 4, topLineY, xOffset + boardWidth + 8, topLineY);
	Pokitto::Display::drawLine(xOffset - 4, bottomLineY, xOffset + boardWidth + 8, bottomLineY);


	// Draw grid
	for(int y = 0; y < 3; ++y)
	{
		for(int x = 0; x < 3; ++x)
		{
			const int cellX = xOffset + ((cellWidth + xGap) * x);
			const int cellY = yOffset + ((cellHeight + yGap) * y);

			// Draw symbol
			switch(grid.getItem(x, y))
			{
				case Cell::Nought:
				{
					const int halfCellWidth = cellWidth / 2;
					const int halfCellHeight = cellHeight / 2;
					const int cellCentreX = cellX + halfCellWidth;
					const int cellCentreY = cellY + halfCellHeight;
					const int radius = std::min(halfCellWidth, halfCellHeight) - 1;

					Pokitto::Display::drawCircle(cellCentreX, cellCentreY, radius);
					break;
				}
				case Cell::Cross:
				{
					const int left = cellX + 1;
					const int top = cellY + 1;
					const int right = cellX + cellWidth - 1;
					const int bottom = cellY + cellHeight - 1;

					Pokitto::Display::drawLine(left, top, right, bottom);
					Pokitto::Display::drawLine(right, top, left, bottom);
					break;
				}
				default:
					break;
			}

			// Draw selector
			if(x == this->selector.x && y == this->selector.y)
			{
				Pokitto::Display::drawRect(cellX, cellY, cellWidth, cellHeight);
			}
		}
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
