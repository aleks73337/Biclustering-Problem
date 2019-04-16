#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>


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

bool is_valid(std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters, int& machines, int& parts)
{
	std::vector<int> machinesInClusters(machines + 1, 0);
	int machinesCounter = 0;
	std::vector<int> partsInClusters(parts + 1, 0);
	int partsCounter = 0;
	for (auto cluster : clusters)
	{
		if (cluster.first.size() == 0 || cluster.second.size() == 0)
			return false;
		for (int el : cluster.first)
		{
			if (machinesInClusters[el] == 0)
				machinesInClusters[el] = 1;
			else
				return false;
			machinesCounter++;
		}

		for (int el : cluster.second)
		{
			if (partsInClusters[el] == 0)
				partsInClusters[el] = 1;
			else
				return false;
			partsCounter++;
		}
	}

	if(partsCounter != parts || machinesCounter != machines)
		return false;

	return true;
}

std::vector< std::pair< std::vector<int>, std::vector<int> > > get_start_decision(std::vector<std::vector<int>>& partsMatches, int machines, int parts)
{
	std::vector< std::pair< std::vector<int>, std::vector<int> > > clusters;
	std::vector<int> machines_seq, parts_seq;
	for (int i = 1; i <= machines; i++)
	{
		machines_seq.push_back(i);
	}
	for (int i = 1; i <= parts; i++)
	{
		parts_seq.push_back(i);
	}
	do
	{
		std::cout << "New decision!" << std::endl;
		int n_clusters = 8;
		for (int i = 0; i < n_clusters; i++)
		{
			std::pair< std::vector<int>, std::vector<int> > cluster;
			clusters.push_back(cluster);
		}

		for (int i = 0; i < machines; i++)
			clusters[rand() % n_clusters].first.push_back(machines_seq[i]);
		for (int i = 0; i < parts; i++)
			clusters[rand() % n_clusters].second.push_back(parts_seq[i]);

		for (auto cluster : clusters)
		{
			std::cout << " Next cluster, machines: ";
			for (auto el : cluster.first)
				std::cout << el << " ";
			std::cout << std::endl << " Parts: ";
			for (auto el : cluster.second)
				std::cout << el << " ";
			std::cout << std::endl;
		}
	} while(!is_valid(clusters, machines, parts));
	
	return clusters;
}


int main()
{
	srand(1);
	std::vector<std::vector<int>> partsMatches;
	int machines, parts;
	std::tie(partsMatches, machines, parts) =  read_file("30x50.txt");
	auto clusters = get_start_decision(partsMatches, machines, parts);

	return 1;
}