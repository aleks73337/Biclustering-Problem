#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iomanip>
#include <functional>

void print_debug(std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters)
{
	std::cout << std::endl;
	std::cout << "===============DEBUG==================";
	std::cout << std::endl;
	for (auto cluster : clusters)
	{
		std::cout << "Machines: ";
		for (auto el : cluster.first)
			std::cout << el << " ";

		std::cout << std::endl << " Parts: ";

		for (auto el : cluster.second)
			std::cout << el << " ";

		std::cout << std::endl;
	}
}

float count_score(std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters, std::vector< std::vector<int> >& partsMatches)
{
	int n1Clust = 0;
	int n0Clust = 0;
	int n1All = 0;

	for (auto& cluster : clusters)
	{
		for (auto& machine : cluster.first)
		{
			for (auto& part : cluster.second)
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
		for (auto& part : partsMatches[i])
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
		if (counter == 0)
		{
			iss >> machines >> parts;
		}
		else
		{
			std::vector<int> tmp_vec;
			while (iss >> tmp_int)
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
	for (auto& machine : prevData)
	{
		std::vector<int> tmp_vec(parts, 0);
		for (int el : machine)
			tmp_vec[el - 1] = 1;
		partsMatches.push_back(tmp_vec);
	}

	return { partsMatches, machines, parts };
}

bool is_valid(std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters, int& machines, int& parts)
{
	std::vector<int> machinesInClusters(machines + 1, 0);
	int machinesCounter = 0;
	std::vector<int> partsInClusters(parts + 1, 0);
	int partsCounter = 0;
	for (auto& cluster : clusters)
	{
		if (cluster.first.size() == 0 || cluster.second.size() == 0)
			return false;
		for (int el : cluster.first)
		{
			if (machinesInClusters[el] == 0)
				machinesInClusters[el] = 1;
			else
			{
				std::cout << std::endl << "Machines dublicating" << std::endl;
				return false;
			}
			machinesCounter++;
		}

		for (int el : cluster.second)
		{
			if (partsInClusters[el] == 0)
				partsInClusters[el] = 1;
			else
			{
				std::cout << std::endl << "Parts dublicating" << std::endl;
				return false;
			}
			partsCounter++;
		}
	}

	if (partsCounter != parts || machinesCounter != machines)
	{
		std::cout << std::endl << "Parts or machines lost" << std::endl;
		return false;
	}

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
		int n_clusters = 5;
		for (int i = 0; i < n_clusters; i++)
		{
			std::pair< std::vector<int>, std::vector<int> > cluster;
			clusters.push_back(cluster);
		}

		for (int i = 0; i < machines; i++)
			clusters[rand() % n_clusters].first.push_back(machines_seq[i]);
		for (int i = 0; i < parts; i++)
			clusters[rand() % n_clusters].second.push_back(parts_seq[i]);

		print_debug(clusters);
	} while (!is_valid(clusters, machines, parts));

	return clusters;
}

void devide(std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters, int clToChange, int non_usable1)
{
	//create 2 new vectors with machines
	int nChange = clToChange;
	auto& toSplit = clusters[nChange];

	if (toSplit.first.size() < 2 || toSplit.second.size() < 2)
		return;
	int nMachines = toSplit.first.size();
	int nLMachines = rand() % nMachines;
	int nRMachines = nMachines - nLMachines;
	std::vector<int> lMachines(nLMachines, 0),
		rMachines(nRMachines, 0);
	std::copy(toSplit.first.begin(), toSplit.first.begin() + nLMachines, lMachines.begin());
	std::copy(toSplit.first.begin() + nLMachines, toSplit.first.end(), rMachines.begin());

	int nParts = toSplit.second.size();
	int nLParts = rand() % nParts;
	int nRParts = nParts - nLParts;
	std::vector<int> lParts(nLParts, 0),
		rParts(nRParts, 0);
	std::copy(toSplit.second.begin(), toSplit.second.begin() + nLParts, lParts.begin());
	std::copy(toSplit.second.begin() + nLParts, toSplit.second.end(), rParts.begin());
	clusters.erase(clusters.begin() + nChange);

	std::pair< std::vector<int>, std::vector<int> > lPair(lMachines, lParts);
	std::pair< std::vector<int>, std::vector<int> > rPair(rMachines, rParts);
	clusters.push_back(lPair);
	clusters.push_back(rPair);

}

void move_machine(std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters, int nFrom, int nTo, int nMachine)
{
	if (nFrom > clusters.size() || nTo > clusters.size())
		return;
	if (clusters[nFrom].first.size() < nMachine || clusters[nFrom].first.size() < 2)
		return;

	auto& mFrom = clusters[nFrom].first;
	auto& mTo = clusters[nTo].first;

	int machine = mFrom[nMachine];
	mFrom.erase(mFrom.begin() + nMachine);

	mTo.push_back(machine);

}

void move_part(std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters, int nFrom, int nTo, int nPart)
{
	if (nFrom > clusters.size() || nTo > clusters.size())
		return;
	if (clusters[nFrom].second.size() < nPart || clusters[nFrom].second.size() < 2)
		return;

	auto& pFrom = clusters[nFrom].second;
	auto& pTo = clusters[nTo].second;

	int part = pFrom[nPart];
	pFrom.erase(pFrom.begin() + nPart);

	pTo.push_back(part);

}

void combine(std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters, int nLCluster, int nRCluster)
{
	if (nLCluster == nRCluster)
		return;
	auto& lMachines = clusters[nLCluster].first;
	auto& lParts = clusters[nLCluster].second;

	auto& rMachines = clusters[nRCluster].first;
	auto& rParts = clusters[nRCluster].second;

	lMachines.insert(lMachines.end(), rMachines.begin(), rMachines.end());
	lParts.insert(lParts.end(), rParts.begin(), rParts.end());

	clusters.erase(clusters.begin() + nRCluster);
}

std::vector< std::pair< std::vector<int>, std::vector<int> > >
local_search(std::vector< std::pair< std::vector<int>, std::vector<int> > > clusters,
	std::vector< std::vector<int> >& partsMatches, bool doDevide)
{
	std::vector< std::pair< std::vector<int>, std::vector<int> > > bestSolution = clusters;
	float bestScore = count_score(clusters, partsMatches);

	if (doDevide)
	{
		for (int i = 0; i < clusters.size(); i++)
		{
			auto tmpSolution = clusters;
			devide(clusters, i, 0);
			float newScore = count_score(tmpSolution, partsMatches);
			if (newScore > bestScore)
			{
				bestScore = newScore;
				bestSolution = tmpSolution;
			}
		}
	}
	else
	{
		for (int i = 0; i < clusters.size(); i++)
		{
			for (int j = 0; j < clusters.size(); j++)
			{
				auto tmpSolution = clusters;
				combine(tmpSolution, i, j);
				float newScore = count_score(tmpSolution, partsMatches);
				if (newScore > bestScore)
				{
					bestScore = newScore;
					bestSolution = tmpSolution;
				}
			}
		}
	}

	return bestSolution;
}

bool save_output(const std::vector< std::pair< std::vector<int>, std::vector<int> > >& clusters,
	const std::string& filename, const int& nMachines, const int& nParts)
{
	std::fstream file;
	file.open(filename, std::fstream::out);

	if (!file.is_open())
		return false;

	std::vector<int> machines(nMachines + 1, 0);
	std::vector<int> parts(nParts + 1, 0);
	int nClusters = clusters.size();

	for (int i = 0; i < nClusters; i++)
	{
		for (auto& machine : clusters[i].first)
			machines[machine] = i + 1;
		for (auto& part : clusters[i].second)
			parts[part] = i + 1;
	}

	machines.erase(machines.begin());
	parts.erase(parts.begin());

	for (auto& el : machines)
		file << el << " ";

	file << std::endl;

	for (auto& el : parts)
		file << el << " ";

	file.close();
	return true;
}

int main()
{
	srand(time(NULL));
	std::string fileName = "20x20.txt";
	std::vector<std::vector<int>> partsMatches;
	int machines, parts;
	std::tie(partsMatches, machines, parts) = read_file(fileName);
	auto clusters = get_start_decision(partsMatches, machines, parts);

	std::vector< std::pair<int, int> > N_k(20);

	float bestScore = count_score(clusters, partsMatches);

	for (int i = 0; i < 2000; i++)
	{
		auto tmpClusters = clusters;
		for (int i = 1; i < 3; i++)
		{
			tmpClusters = clusters;
			int nClusters = tmpClusters.size();
			if (i % 2)
				devide(tmpClusters, rand() % (nClusters - 1), 0);
			else
				combine(tmpClusters, rand() % (nClusters - 1), rand() % (nClusters - 1));

			float result = 0;
			float new_result = 0.2;
			bool doDevide = true;

			do
			{
				if (new_result <= result)
					doDevide = false;
				result = count_score(tmpClusters, partsMatches);
				tmpClusters = local_search(tmpClusters, partsMatches, doDevide);
				new_result = count_score(tmpClusters, partsMatches);
			} while (new_result > result || doDevide);
		};

		float newBestScore = count_score(tmpClusters, partsMatches);

		if (bestScore < newBestScore)
		{
			std::cout << std::endl << "Old: " << bestScore << " New: " << newBestScore << " " << is_valid(tmpClusters, machines, parts) << std::endl;
			clusters = tmpClusters;
			bestScore = newBestScore;
		}
	}

	std::cout << "New score: " << bestScore << std::endl;
	save_output(clusters, fileName.replace(fileName.size() - 3, fileName.size(), "sol"), machines, parts);
	std::cin.get();
	return 1;
}
