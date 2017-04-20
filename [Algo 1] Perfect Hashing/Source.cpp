#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

class student;
class group {
public:
	std::string name;
	std::vector<student*> groupList;

	group(std::string a) {
		name = a;
	}
};

class student {
public:
	std::string name;
	std::string homeCountry;
	std::string homeCity;
	double rating;
	group* specialization;

	student(std::string name, std::string country, std::string city, double rating, group* s) {
		this->name = name;
		homeCountry = country;
		homeCity = city;
		this->rating = rating;
		specialization = s;
	}

	void show() const {
		std::cout << "Student Name: " << name << "\nStudent Country: " << homeCountry << "\nStudent City: " << homeCity << std::endl;
	}
};

long long encode(std::string name) {
	long long code = 0;
	for (int i = 0; i<name.size(); i++) {
		name[i] = char(tolower(name[i]));
		code += ((long long)(name[i])*(i + 1));
	}
	return code;
}

long long hash(long long a, long long b, long long p, long long m, std::string name) {
	long long result = ((a*encode(name) + b) % p) % m;
	return result;
}

int main() {
	std::ifstream in("base.txt");
	std::string temp, tempCountry, tempCity, tempSpec, tempR;
	double tempRating;
	int firstLevelIndex;

	std::vector<student*> hashTable[10];
	std::vector<group*> Groups(10);
	std::vector<student*> Students(100);
	std::vector<int> counter(10, 0);

	for (int i = 0; i < 10; i++) {
		getline(in, temp);
		Groups[i] = new group(temp);
	}

	getline(in, temp);

	for (int i = 0; i < 100; i++) {
		getline(in, temp);
		getline(in, tempR);
		getline(in, tempCity);
		getline(in, tempCountry);
		getline(in, tempSpec);
		tempRating = atof(tempR.c_str());
		Students[i] = new student(temp, tempCountry, tempCity, tempRating, new group(tempSpec));
		for (int j = 0; j < 10; j++) {
			if (Groups[j]->name == temp) {
				Groups[j]->groupList.push_back(Students[i]);
			}
		}
		firstLevelIndex = hash(3, 42, 2999, 10, Students[i]->name);
		counter[firstLevelIndex]++;
		getline(in, temp);
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < counter[i] * counter[i]; j++) {
			hashTable[i].push_back(nullptr);
		}
	}

	for (int i = 0; i<Students.size(); i++) {
		firstLevelIndex = hash(3, 42, 2999, 10, Students[i]->name);
		long long secondLevelIndex = hash(135664, 1786127, 2999, counter[firstLevelIndex] * counter[firstLevelIndex], Students[i]->name);
		hashTable[firstLevelIndex][secondLevelIndex] = Students[i];
	}

	do {
		getline(std::cin, temp);
		firstLevelIndex = hash(3, 42, 2999, 10, temp);

		if (!counter[firstLevelIndex]) {
			std::cout << "Can't find such student." << std::endl;
			continue;
		}

		long long secondLevelIndex = hash(135664, 1786127, 2999, counter[firstLevelIndex] * counter[firstLevelIndex], temp);

		if (hashTable[firstLevelIndex][secondLevelIndex] != nullptr) {
			std::cout << hashTable[firstLevelIndex][secondLevelIndex]->specialization->name << std::endl;
			std::cout << hashTable[firstLevelIndex][secondLevelIndex]->rating << std::endl;
		}
		else {
			std::cout << "Can't find such student." << std::endl;
		}
	} while (temp != "EXIT");

	system("pause");
	return 0;
}