#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iomanip>


float count_score(std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters, std::vector< std::vector<int> >& partsMatches)
{
	int n1Clust = 0;
	int n0Clust = 0;
	int n1All = 0;

	for (auto cluster : clusters)
	{
		for (auto machine : cluster.first)
		{
			for (auto part : cluster.second)
			{
				if (partsMatches[machine - 1][part - 1] == 1)
					n1Clust++;
				else
					n0Clust++;
			}
		}
	}

	
	for (int i = 0; i < partsMatches.size(); i++)
	{
		for (auto part : partsMatches[i])
		{
			if (partsMatches[i][part] == 1)
				n1All++;
		}
	}

	double res = double(n1Clust) / double(n1All + n0Clust);

	return res;
}

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
		machines_seq.push_back(i);

	for (int i = 1; i <= parts; i++)
		parts_seq.push_back(i);

	do
	{
		std::cout << "New decision!" << std::endl;
		int n_clusters = 4;
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

bool save_output(const std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters, 
				 const std::string& filename, const int& nMachines, const int& nParts)
{
	std::fstream file;
    file.open(filename, std::fstream::out);
    std::vector<int> machines(nMachines + 1, 0);
    std::vector<int> parts(nParts + 1, 0);
    int nClusters = clusters.size();

	for (int i = 0; i < nClusters; i++)
	{
		for (auto machine : clusters[i].first)
			machines[machine] = i + 1;
		for (auto part : clusters[i].second)
			parts[part] = i + 1;
	}

	machines.erase(machines.begin());
	parts.erase(parts.begin());

    for (auto el : machines)
    	file << el << " ";

    file << std::endl;

    for (auto el : parts)
    	file << el << " ";

    file.close();
    return true;
}

int main()
{
	srand(1);
	std::string fileName = "20x20.txt";
	std::vector<std::vector<int>> partsMatches;
	int machines, parts;
	std::tie(partsMatches, machines, parts) =  read_file(fileName);
	auto clusters = get_start_decision(partsMatches, machines, parts);
	float result = count_score(clusters, partsMatches);
	std::cout << result;
	save_output(clusters, fileName.replace(fileName.size() - 3, fileName.size(), "sol"), machines, parts);
	return 1;
}