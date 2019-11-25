
#include "Session.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "json.hpp"

using json = nlohmann::json;

using namespace std;

Session::Session(const std::string& configFilePath)  {
	getJsonData(configFilePath);
}


json Session::getJsonData(const std::string& configFilePath) {

	string aaa;
	std::ifstream watchableFile(configFilePath, std::ifstream::binary);
	watchableFile >> aaa;

	cout << aaa;
}
