#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "ProblemPlecakowy.h"

using namespace std;

int main()
{
	vector<int> ItemsWeight;
	vector<int> SolutionsWeight;
	vector<int> SelectedSolutions;
	vector<string> Solutions;
	vector<string> NewGenOfSolutions;

	int MaxWeight = 2500;
	int BestWeight = 50000;
	int PreviousBestWeight = 0;
	int NumberOfIterationWithoutBetterSolution = 0;
	int NumberOfIterations = 0;

	string PreviousBestSolution;
	string BestSolution;

	srand(time(NULL));

	CreatingSolutionsCalcWeights(ItemsWeight, Solutions, SolutionsWeight);

	//Algorithm loops until it finds the best possible outcome (2500)
	//or until the best solution does not change after 10 iteration
	while (BestWeight != MaxWeight && NumberOfIterationWithoutBetterSolution != 10)
	{
		FindingBestSolutions(SolutionsWeight, MaxWeight, SelectedSolutions);
		CheckingForSolutionUnderMaxWeight(SelectedSolutions, SolutionsWeight);
		SolutionsCrossing(SelectedSolutions, NewGenOfSolutions, Solutions);
		Mutation(NewGenOfSolutions);

		ClearingVectors(Solutions, SelectedSolutions, SolutionsWeight, NewGenOfSolutions);
		WeightCount(Solutions, ItemsWeight, SolutionsWeight);
		FindingBestSolution(SolutionsWeight, BestWeight, MaxWeight, BestSolution, Solutions);

		NumberOfIterationUpdating(BestSolution, PreviousBestSolution, NumberOfIterationWithoutBetterSolution, PreviousBestWeight, BestWeight, NumberOfIterations);
	}

	OutcomePrinting(BestWeight, BestSolution, NumberOfIterations);

	return 0;
}

void OutcomePrinting(int BestWeight, std::string& BestSolution, int NumberOfIterations)
{
	cout << BestWeight << endl;
	cout << BestSolution << endl;
	cout << NumberOfIterations << endl;
}

void NumberOfIterationUpdating(string& BestSolution, string& PreviousBestSolution, 
	int& NumberOfIterationWithoutBetterSolution, int& PreviousBestWeight, int BestWeight, int& NumberOfIterations)
{
	if (BestSolution == PreviousBestSolution)
	{
		NumberOfIterationWithoutBetterSolution++;
	}
	else
	{
		NumberOfIterationWithoutBetterSolution = 0;
	}

	PreviousBestWeight = BestWeight;
	PreviousBestSolution = BestSolution;

	NumberOfIterations++;
}

void FindingBestSolution(vector<int>& SolutionsWeight, int& BestWeight, int MaxWeight, string& BestSolution, vector<string>& Solutions)
{
	//Finding the best solution 
	vector<int> SortedWeights = SolutionsWeight;
	sort(SortedWeights.begin(), SortedWeights.end(), greater<int>());
	for (int SortedWeightsIndex = 0; SortedWeightsIndex < SortedWeights.size(); SortedWeightsIndex++)
	{
		BestWeight = SortedWeights[SortedWeightsIndex];
		if (BestWeight <= MaxWeight)
		{
			for (int SolutionsWeightIndex = 0; SolutionsWeightIndex < SolutionsWeight.size(); SolutionsWeightIndex++)
			{
				if (BestWeight == SolutionsWeight[SolutionsWeightIndex])
				{
					BestSolution = Solutions[SolutionsWeightIndex];
					break;
				}
			}
			break;
		}
	}
}

void WeightCount(vector<string>& Solutions, vector<int>& ItemsWeight, vector<int>& SolutionsWeight)
{
	//Weight couting
	for (int SolutionIndex = 0; SolutionIndex < 20; SolutionIndex++)
	{
		int SolutionWeight = 0;
		for (int CharIndex = 0; CharIndex < 100; CharIndex++)
		{
			if (Solutions[SolutionIndex][CharIndex] == '1')
			{
				SolutionWeight += ItemsWeight[CharIndex];
			}
		}
		SolutionsWeight.push_back(SolutionWeight);
	}
}

void ClearingVectors(vector<string>& Solutions, vector<int>& SelectedSolutions, 
	vector<int>& SolutionsWeight, vector<string>& NewGenOfSolutions)
{
	Solutions.clear();
	SelectedSolutions.clear();
	SolutionsWeight.clear();
	Solutions = NewGenOfSolutions;
}

void Mutation(vector<string>& NewGenOfSolutions)
{
	//Mutation
	int PossibilityOfMutation = rand() % 100;

	if (PossibilityOfMutation == 0 || PossibilityOfMutation == 1 ||
		PossibilityOfMutation == 2 || PossibilityOfMutation == 3 || PossibilityOfMutation == 4)
	{
		int RandomIndexOfSolution = rand() % 20;
		int RandomIndexOfSolutionsChar = rand() % 100;

		if (NewGenOfSolutions[RandomIndexOfSolution][RandomIndexOfSolutionsChar] == '0')
		{
			NewGenOfSolutions[RandomIndexOfSolution][RandomIndexOfSolutionsChar] = '1';
		}
		else
		{
			NewGenOfSolutions[RandomIndexOfSolution][RandomIndexOfSolutionsChar] = '0';
		}
	}
}

void SolutionsCrossing(vector<int>& SelectedSolutions, vector<string>& NewGenOfSolutions, vector<string>& Solutions)
{
	//Solution crossing
	//Adding best solutions to new vector NewGenOfSolutions
	for (int SelectedSolutionIndex = 0; SelectedSolutionIndex < SelectedSolutions.size(); SelectedSolutionIndex++)
	{
		NewGenOfSolutions.push_back(Solutions[SelectedSolutions[SelectedSolutionIndex]]);
	}

	vector<int> UsedIndexVector;

	while (UsedIndexVector.size() < 10)
	{
		//Two random numbers between 0 and 9 to draw random indexes
		int index1 = rand() % 10;
		int index2 = rand() % 10;

		//Checking if generated indexes have already been used
		auto it1 = find(UsedIndexVector.begin(), UsedIndexVector.end(), index1);
		auto it2 = find(UsedIndexVector.begin(), UsedIndexVector.end(), index2);

		//If not, adding used indexes and new solutions that were created by solution crossing
		if (it1 == UsedIndexVector.end() && it2 == UsedIndexVector.end() && index1 != index2)
		{
			UsedIndexVector.push_back(index1);
			UsedIndexVector.push_back(index2);

			string Solution1Part1, Solution1Part2, Solution2Part1, Solution2Part2;

			int index = rand() % 100;

			Solution1Part1 = NewGenOfSolutions[index1].substr(0, index);
			Solution1Part2 = NewGenOfSolutions[index1].substr(index);
			Solution2Part1 = NewGenOfSolutions[index2].substr(0, index);
			Solution2Part2 = NewGenOfSolutions[index2].substr(index);

			string NewSolution1, NewSolution2;

			NewSolution1 = Solution1Part1 + Solution2Part2;
			NewSolution2 = Solution2Part1 + Solution1Part2;

			NewGenOfSolutions.push_back(NewSolution1);
			NewGenOfSolutions.push_back(NewSolution2);
		}
	}
}

void CheckingForSolutionUnderMaxWeight(vector<int>& SelectedSolutions, vector<int>& SolutionsWeight)
{
	//Checking if there is at least one solution that weights <= 2500
	for (int i = 0; i < SelectedSolutions.size(); i++)
	{
		int CurrentSolutionWeight = SolutionsWeight[SelectedSolutions[i]];

		if (CurrentSolutionWeight <= 2500)
		{
			break;
		}

		//If not, removing last index of SelectedSolution and adding the best solution that is <= 2500
		else if (i == 9)
		{
			SelectedSolutions.pop_back();
			vector<int> SortedWeights = SolutionsWeight;
			sort(SortedWeights.begin(), SortedWeights.end(), greater<int>());

			for (int Weight : SortedWeights)
			{
				if (Weight <= 2500)
				{
					for (int WeightIndex = 0; WeightIndex < SolutionsWeight.size(); WeightIndex++)
					{
						if (SolutionsWeight[WeightIndex] == Weight)
						{
							SelectedSolutions.push_back(WeightIndex);
							break;
						}
					}
					break;
				}
			}
		}
	}
}

void FindingBestSolutions(vector<int>& SolutionsWeight, int MaxWeight, vector<int>& SelectedSolutions)
{
	//Finding the best solution indexes in vector Solutions
	int Fitness = 0;
	int BestFitness = 0;
	int BestFitnessIndex = 0;

	for (int NumberOfSolutions = 0; NumberOfSolutions < 10; NumberOfSolutions++)
	{
		BestFitness = 10000; //Random high number in order to reset it

		for (int WeightIndex = 0; WeightIndex < SolutionsWeight.size(); WeightIndex++)
		{
			Fitness = abs(SolutionsWeight[WeightIndex] - MaxWeight);

			//Checking of current weight index is already in vector SelectedSolutions
			auto FindIndexInSelectedSolutions = find(SelectedSolutions.begin(), SelectedSolutions.end(), WeightIndex);

			if (Fitness < BestFitness && FindIndexInSelectedSolutions == SelectedSolutions.end())
			{
				BestFitness = Fitness;
				BestFitnessIndex = WeightIndex;
			}
		}

		//Adding solution index to vector SelectedSolutions
		if (BestFitnessIndex != -1) {
			SelectedSolutions.push_back(BestFitnessIndex);
		}
	}
}

void CreatingSolutionsCalcWeights(vector<int>& ItemsWeight, vector<string>& Solutions, vector<int>& SolutionsWeight)
{
	//Random weights added to vector ItemsWeight
	for (int WeightIndex = 0; WeightIndex < 100; WeightIndex++)
	{
		//Random weights in the range of 10 to 90
		ItemsWeight.push_back(10 + (rand() % 81));
	}

	//Random solutions added to vector Solutions && weight couting
	for (int SolutionIndex = 0; SolutionIndex < 20; SolutionIndex++)
	{
		string Solution;
		int SolutionWeight = 0;

		for (int SolutionItemIndex = 0; SolutionItemIndex < 100; SolutionItemIndex++)
		{
			int Number = rand() % 2;
			Solution += to_string(Number);

			if (Number == 1)
			{
				SolutionWeight += ItemsWeight[SolutionItemIndex];
			}
		}

		Solutions.push_back(Solution);
		SolutionsWeight.push_back(SolutionWeight);
	}
}
