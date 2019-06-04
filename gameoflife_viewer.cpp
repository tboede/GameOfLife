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
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

// 3rd party
#include "json11.hpp"
#include <cxxopts.hpp>

// local
#include "logging.hpp"

#define BOTTOM_CHAR '-'
#define SIDE_CHAR '|'

void display(int width,int height,const json11::Json& entry) {

	int limitx = width*2+3;
	int limity = height*2+3;

	std::string * lines = new std::string[limity];

	for(int y=0; y<limity; ++y){

		lines[y] = std::string(limitx,' ');
		lines[y][0] = SIDE_CHAR;
		lines[y][limitx-1] = SIDE_CHAR;
	}

	lines[0] = std::string(limitx,BOTTOM_CHAR);
	lines[limity-1] = std::string(limitx,BOTTOM_CHAR);


	for(const auto & item : entry.array_items()) {

		int x = item["x"].int_value()+width+1;
		int y = -(item["y"].int_value())+height+1;

		if( x >= 0 && x < limitx && y > 0 && y < limity){
			lines[y][x]='x';
		}
	}

	for(int y=0; y<limity; ++y){
		std::cout << lines[y] << std::endl;
	}


	delete[] lines;
}


int main(int argc, char ** argv) {

	/////////////////////////////////
	////////// Commandline //////////
	/////////////////////////////////

	cxxopts::Options options("GameOfLifeViewer");

	options.add_options("required")
	        ("database","Database file", cxxopts::value<std::string>());

	options.add_options()
	        ("h,help","Show help", cxxopts::value<bool>());

	auto result = options.parse(argc,argv);

	if(result["help"].count() > 0){

		std::cerr << options.help(std::vector<std::string>()) << std::endl;
		return 1;
	}

	if(result["database"].count() < 1){

		throw cxxopts::option_required_exception("database");
	}

	std::string dbpath = result["database"].as<std::string>();


	/////////////////////////////////
	//////////// Config /////////////
	/////////////////////////////////

	auto logger = Logger::get("viewer");

	logger->info("Start");

	std::ifstream db(dbpath);
	
	db.exceptions(std::ios_base::badbit);


	/////////////////////////////////
	/////////// Main loop ///////////
	/////////////////////////////////


	std::string buffer;

	while(std::getline(db,buffer)) {
	
		std::string err;

		json11::Json entry = json11::Json::parse(buffer,err);

		if(err.size() > 0){

			std::cerr << err << std::endl;
			return 1;
		}

		system("clear");
		display(20,20,entry);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	logger->info("Stop");

	return 0;
}

