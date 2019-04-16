#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>


std::tuple<std::vector<std::vector<int>>, int, int> read_file(const std::string fileName)
{
	std::ifstream infile(fileName);
	std::string line;
	int counter = 0;
	int machines;
	int parts;
	std::vector<std::vector<int>> prevData;
	int tmp_int;
	while (std::getline(infile, line))
	{
	    //std::cout << line << std::endl;
	    std::stringstream iss(line);
	    if (counter	== 0)
	    {
	    	iss >> machines >> parts;
	    }
	    else
	    {
	    	std::vector<int> tmp_vec;
	    	while(iss >> tmp_int)
	    	{
	    		tmp_vec.push_back(tmp_int);
	    	};
	    	tmp_vec.erase(tmp_vec.begin());
	    	prevData.push_back(tmp_vec);
	    }
	    counter++;
	}

	infile.close();

	std::vector<std::vector<int>> partsMatches;
	for (auto machine : prevData)
	{
		std::vector<int> tmp_vec(parts, 0);
		for (int el : machine)
			tmp_vec[el - 1] = 1;
		partsMatches.push_back(tmp_vec);
	}

	return {partsMatches, machines, parts};
}

void get_start_decision(std::vector<std::vector<int>>& partsMatches, int machines, int parts)
{
	std::pair< std::vector<int>, std::vector<int> > cluster;
	for (int i = 1; i <= machines; i++)
	{
		cluster.first.push_back(i);
	}
		for (int i = 1; i <= parts; i++)
	{
		cluster.second.push_back(i);
	}

	std::cout << cluster.first.size() << cluster.second.size() << " ";
}


int main()
{
	std::vector<std::vector<int>> partsMatches;
	int machines, parts;
	std::tie(partsMatches, machines, parts) =  read_file("30x50.txt");
	for (auto machine : partsMatches)
	{
		for (auto el : machine)
		{
			std::cout << el << " ";
		}
		std::cout << std::endl;
	}
	get_start_decision(partsMatches, machines, parts);

	return 1;
}