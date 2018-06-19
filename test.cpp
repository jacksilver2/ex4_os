#include <map>
#include <iostream>
#include <cassert>
#include <vector>


void mapTest();

int removeByName(std::vector<std::string> *vec, std::string name)
{
	int i;
	for (i = 0; i < vec->size(); ++i)
	{
		if ((*vec)[i] == name)
		{
//			std::cout << "found it at index " << i << std::endl;
			(*vec).erase((*vec).begin() + i);
			return i;
		}
	}
	return -1; //not found
}

void vecTest()
{

	std::vector<std::string> tester;
	tester.push_back("a");
	tester.push_back("b");
	tester.push_back("c");
	tester.push_back("d");
	for (int i = 0; i < tester.size(); ++i)
	{
		std::cout << tester[i] << " ";
	}
	std::cout << std::endl;

	int ret = removeByName(&tester, "d");

	for (int i = 0; i < tester.size(); ++i)
	{
		std::cout << tester[i] << " ";
	}
}

std::string

int smain(int argc, char **argv)
{
//	mapTest();
//	vecTest();
//	return 0;
}

void mapTest()
{
	std::map<std::string, int> m;
	m["b"] = 23;
	m["c"] = 42;
	m["z"] = 12;
	m["a"] = 56;
	// check if key is present
//	if (m.find("world") != m.end())
//	{
//		std::cout << "map contains key world!\n";
//		std::cout << m["world"] << std::endl;
//	}
//	 retrieve
//	std::cout << m["hello"] << '\n';
//	std::map<std::string, int>::iterator i = m.find("hello");
//	assert(i != m.end());
//	std::cout << "Key: " << i->first << " Value: " << i->second << '\n';
//
//	std::map<std::string, int>::const_iterator it;
//	for (it = m.cbegin(); it != m.cend(); ++it)
//	{
//		std::cout << "Key: " << it->first << " Value: " << it->second << '\n';
//	}
}


