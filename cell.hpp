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
#ifndef CELL_HPP
#define CELL_HPP

// system
#include <climits>
#include <memory>
#include <vector>


class World;

class Cell {

public:
	/// Cell Id, makes the cell unique.
	typedef uint64_t ID;

	/// Struct for the cell's position.
	struct Position {
		int x;
		int y;
		Position(int x,int y) :x(x),y(y) {};
	};

private:
	/// Cell ID
	ID id;

	/// Cell location
	Position pos;

	/// The world the cell exists in
	World * world;

	/// Returns all postions that are not already occupied by any neighbour cells.
	std::vector<Position> findFreePos(const std::vector<Cell*>& neighbours);

public:
	Cell(World * world, int x, int y);
	~Cell();

	/// Update cell state
	void Update();

	/// Return cell id
	ID getID() const { return id; }

	// Return cell postion
	Position getPosition() const { return pos;}
};


#endif //CELL_HPP
