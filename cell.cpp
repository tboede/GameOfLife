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

// system
#include <vector>


// local
#include "logging.hpp"
#include "cell.hpp"
#include "world.hpp"


Cell::Cell(World * world, int x, int y) : 
      world(world),pos(x,y) {

	id = world->newID();
}


Cell::~Cell() {

}

void Cell::Update() {

	std::vector<Cell*> list= world->findCells(id,pos.x-1,pos.y-1,pos.x+1,pos.y+1);

	int counter = list.size();

	if(counter > 3){

		world->Remove(id);

	} else if (counter < 2 ){

		world->Remove(id);
	}

	std::vector<Cell::Position> freepos = findFreePos(list);

	for(Cell::Position & pos : freepos) {

		world->Populate(pos.x,pos.y);
	}

}

std::vector<Cell::Position> Cell::findFreePos(const std::vector<Cell *>& neighbours) {

	std::vector<Cell::Position> freepos;

	for(int y=-1; y<=1; y++){
		for(int x=-1; x<=1; x++){

			if(x == 0 && y == 0){
				continue;
			}

			bool isfree=true;

			for(auto item : neighbours) {
				auto opos = item->getPosition();

				if(opos.x == pos.x+x && opos.y == pos.y+y){
					isfree=false;
					break;
				}
			}

			if(isfree){
				freepos.push_back(Position(pos.x+x,pos.y+y));
			}
		}
	}

	return freepos;
}

