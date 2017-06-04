#include "reference.h"
#include <sstream>

void conveny(std::string name, const int& age)
{
	std::cout << "name = " << name.c_str() << std::endl;
	std::cout << "age = " << age << std::endl;

/************************************************************************/
	{
		for(int i = 0; i < 10; i++)
		{
			if (i%2 == 1)
			{
				std::cout << i << std::endl;
			}
		}
	}
/************************************************************************/
	/************************************************************************/
	{
		std::string str = "circulation";
		for(int i = 0; i < 10; ++i)
		{
			// int to string 
			std::stringstream ss;
			std::string temp_s;
			ss<<i;
			ss>>temp_s;
			std::string temp_str = str + temp_s;
			std::cout << temp_str << std::endl;
		}
	}
	/************************************************************************/
}