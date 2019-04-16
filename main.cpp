#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>


std::vector<std::vector<int>> read_file(const std::string fileName)
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
		std::vector<int> tmp_vec(20, 0);
		for (int el : machine)
			tmp_vec[el - 1] = 1;
		partsMatches.push_back(tmp_vec);
	}

	return partsMatches;
}


int main()
{

	std::vector<std::vector<int>> partsMatches = read_file("20x20.txt");

	return 1;
}