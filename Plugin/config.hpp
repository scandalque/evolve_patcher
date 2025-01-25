#pragma once
#include <iostream>
#include <map>

class c_settings
{
private:
	using config_t = std::map<std::string, bool>;

public:
	std::string path;
	std::string evolve_processing = "__EvolveClient.asi";

	config_t data = {};

	static c_settings* get();

	void load();
	void save();

	c_settings();
	~c_settings();
};