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

#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>

#include <yaml-cpp/yaml.h>
#include <cxxopts.hpp>

#include "logging.hpp"
#include "cell.hpp"
#include "world.hpp"


int main(int argc, char ** argv){

	
	/////////////////////////////////
	////////// Commandline //////////
	/////////////////////////////////

	cxxopts::Options options("GameOfLife");

	options.add_options("required")
	        ("config","Config file", cxxopts::value<std::string>());

	options.add_options()
	        ("h,help","Show help", cxxopts::value<bool>());

	auto result = options.parse(argc,argv);

	if(result["help"].count() > 0){

		std::cerr << options.help(std::vector<std::string>()) << std::endl;
		return 1;
	}

	if(result["config"].count() < 1){

		throw cxxopts::option_required_exception("config");
	}

	std::string configpath = result["config"].as<std::string>();


	/////////////////////////////////
	//////////// Config /////////////
	/////////////////////////////////

	spdlog::set_level(spdlog::level::info);

	auto logger = Logger::get("console");

	logger->info("Start");

	YAML::Node config = YAML::LoadFile(configpath);


	/////////////////////////////////
	/////////// World cfg ///////////
	/////////////////////////////////

	YAML::Node worldcfg = config["World"];

	size_t maxgens = worldcfg["maxgens"].as<size_t>(1000);

	World world(worldcfg["width"].as<uint16_t>(),worldcfg["height"].as<uint16_t>());

	/////////////////////////////////
	/////////// Cells cfg ///////////
	/////////////////////////////////

	YAML::Node cells = config["Cells"];

	for(std::size_t i=0; i<cells.size(); ++i){

		int x = cells[i]["x"].as<int>(0);
		int y = cells[i]["y"].as<int>(0);

		world.Add(std::unique_ptr<Cell>(new Cell(&world,x,y)));
	}

	/////////////////////////////////
	////////// Databse cfg //////////
	/////////////////////////////////

	std::ofstream dbfile(worldcfg["dbfile"].as<std::string>("db.json"),std::ios_base::app);

	dbfile.exceptions(std::ios_base::badbit);
	
	/////////////////////////////////
	/////////// Simulation //////////
	/////////////////////////////////

	world.UpdateList();

	for(int i=0; i<maxgens && world.Count() > 0; ++i){

		world.Update();
		//world.Display();
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		dbfile << world.Store().dump() << std::endl;
		dbfile.flush();
	}

	{
		std::stringstream sstream;
		sstream << "Total cell count: " << world.TotalCells();
		logger->info(sstream.str());
	}

	logger->info("Stop");

	return 0;
}
