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
#include <iostream>
#include <sstream>

// local
#include "logging.hpp"
#include "cell.hpp"
#include "world.hpp"


World::World(uint16_t width, uint16_t height)
       : width(width),height(height) {

	logger = Logger::get("world");
	logger->debug("Create World");

	cellIDCounter=0;
}


World::~World() {


	logger->debug("Destroy World");
}


void World::Add(std::unique_ptr<Cell> && cell) {

	auto pos = cell->getPosition();
	
	if( pos.x < width && pos.x > -width
	  && pos.y < height && pos.y > -width){

		logger->debug("Add cell");
		addQueue.push(std::move(cell));
	}
}


void World::Remove(Cell::ID cellid) {

	logger->debug("Remove cell");
	removeQueue.push(cellid);
}

std::vector<Cell *> World::findCells(Cell::ID id,int minX, int minY, int maxX, int maxY) {

	std::vector<Cell *> list;

	for(auto & item : cellList) {

		if(item->getID() == id){
			continue;
		}

		Cell::Position pos = item->getPosition();

		if(pos.x >= minX && pos.x <= maxX
		   && pos.y >= minY && pos.y <= maxY ) {

			list.push_back(item.get());
		}
	}

	return list;
}

void World::UpdateList() {

	std::queue<std::unique_ptr<Cell>> localAddQueue;

	// Lock
	{
		localAddQueue.swap(addQueue);
	}

	std::queue<Cell::ID> localRemoveQueue;

	// Lock
	{
		
		localRemoveQueue.swap(removeQueue);
	}

	while(!localRemoveQueue.empty()) {

		Cell::ID id = localRemoveQueue.front();
		localRemoveQueue.pop();

		std::queue<std::vector<std::unique_ptr<Cell>>::iterator> iterQueue;

		for(auto it=cellList.begin(); it != cellList.end(); ++it){

			if( (*it)->getID() == id ) {
				iterQueue.push(it);
			}
		}

		while(!iterQueue.empty()) {
			cellList.erase(std::move(iterQueue.front()));
			iterQueue.pop();
		}
	}

	while(!localAddQueue.empty()) {

		cellList.push_back(std::move(localAddQueue.front()));
		localAddQueue.pop();
	}

	
	std::vector<std::pair<std::unique_ptr<Cell>,char>> localPopulationList;
	{
		localPopulationList.swap(populationList);
	}

	for(auto & item : localPopulationList) {

		if(item.second == 3){

			cellList.push_back(std::move(item.first));
		}
	}

}

void World::Update() {

	UpdateList();

	for(auto & item : cellList) {

		item->Update();
	}
}

void World::Populate(int x, int y){

	if( x >= width || x <= -width || y >= height || y <= -height){

		return;
	}

	for(auto & item : populationList) {

		Cell * cell = item.first.get();

		Cell::Position pos = cell->getPosition();

		if(pos.x == x && pos.y == y) {

			item.second+=1;
			return;
		}
	}

	populationList.push_back(std::pair<std::unique_ptr<Cell>,int>(std::unique_ptr<Cell>(new Cell(this,x,y)),1));

}

void World::Status() {

	std::stringstream sstream;

	sstream << "Count: " << cellList.size();

	logger->info(sstream.str());

}

void World::Display() {

	std::string * screen = new std::string[height*2+1];

	for(int y=0; y<height*2+1; ++y){

		screen[y]=std::string(width*2+1,' ');
		screen[y][0]='|';
		screen[y][width*2]='|';
	}

	screen[0]=std::string(width*2+1,'-');
	screen[height*2]=std::string(width*2+1,'-');

	for(auto & item : cellList) {

		auto pos = item->getPosition();

		screen[-pos.y+height][pos.x+width]='x';
	}

	system("clear");

	for(int y=0; y<height*2+1; ++y){

		std::cout << screen[y] << std::endl;
	}
}

json11::Json World::Store() {

	json11::Json::array cells;

	for(auto & item : cellList) {

		auto pos = item->getPosition();

		json11::Json::object cell;

		cell["x"]=pos.x;
		cell["y"]=pos.y;

		cells.push_back(std::move(cell));
	}

	return cells;
}
