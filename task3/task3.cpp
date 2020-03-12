#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

static bool file_exists (const char *name)
{
	/*if (name == nullptr)
		return false;*/

	struct stat buffer;
	return stat (name, &buffer) == 0;
}

int main (int count, char *argv[])
{
	const auto path = argv[1];

	if (path == nullptr)
	{
		throw std::runtime_error ("path is null!");
	}

	std::vector<std::string> files;
	for (auto i = 1; i <= 5; i++)
	{
		std::string str = "";
		if (std::string (path).back ( ) != '\\')
			str += "\\";
		str += "Cash" + i;

		files.emplace_back (path + str + ".txt");
	}

	std::map<int, float> intervals;
	for (auto i = 1; i <= 16; i++)
	{
		intervals.emplace (i, 0.f);
	}

	for (auto f : files)
	{
		if (!file_exists (f.c_str ( )))
			throw std::runtime_error ("file " + f + " not found!");

		std::ifstream file (f);
		if (!file)
			throw std::runtime_error ("file " + f + " is broken!");

		std::string str;
		auto        line = 1;
		while (std::getline (file, str))
		{
			const auto num = std::stof (str);
			intervals[line] += num;
			line++;
		}
	}

	using type = decltype(intervals);
	auto max = *std::max_element (intervals.begin ( ), intervals.end ( ), [] (const type::value_type &p1, const type::value_type &p2)
	{
		return p1.second < p2.second;
	});

	auto buff = std::make_unique<char[]> (128);
	sprintf_s (buff.get ( ), 128, "Наибольшее число посетителей было в интервале № %d (%.2f)", max.first, max.second);

	std::cout << buff << std::endl;
}
