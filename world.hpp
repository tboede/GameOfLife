/*
Copyright (c) 2019 Tim Böde

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <queue>
#include <memory>

#include <climits>


#include "json11.hpp"
#include "logging.hpp"

class Cell;

class World {

private:
	/// List of all existing cells.
	std::vector<std::unique_ptr<Cell>> cellList;

	/// World width
	uint16_t width;

	/// World height
	uint16_t height;

	/// Queue for cells to be added.
	std::queue<std::unique_ptr<Cell>> addQueue;

	/// Queue for cells to be removed.
	std::queue<Cell::ID> removeQueue;

	/// List for neighbour cells to be created.
	std::vector<std::pair<std::unique_ptr<Cell>,char>> populationList;

	/// Instance logger
	std::shared_ptr<spdlog::logger> logger;

	uint64_t cellIDCounter;

public:
	/**
	Creates a new World.	

	\param width World width
	\param height World height
	*/
	World(uint16_t width, uint16_t height);
	~World();

	/// Adds a new cell
	void Add(std::unique_ptr<Cell> && cell);

	/// Removes a cell
	void Remove(Cell::ID cellid);

	/// Flush all queues.
	void UpdateList();

	/// Next generation
	void Update();

	/// Prints current world status
	void Status();

	/// Display current world state on screen
	void Display();

	/// Creates a json array as snapshot of the current generation
	json11::Json Store();

	/** 
 	Increase population counter of the cell at the given location.
 	If the counter is exactly 3 at the nex Update(), a new cell will be created at the population location.
 	*/ 
	void Populate(int x,int y);

	/**
 	Returns all cells (except the calling cell) within the search area.

	\param id id of the calling cell
	\param minX,minY lower left corner of the search area
	\param maxX,maxY upper right corner of the search area

	*/ 
	std::vector<Cell *> findCells(Cell::ID id,int minX, int minY, int maxX, int maxY);

	/// Number of living cells
	std::size_t Count() const { return cellList.size();}

	uint64_t newID() { return (++cellIDCounter);}

	uint64_t TotalCells() const { return cellIDCounter;}
};


#endif //WORLD_HPP
