#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

static bool file_exists (const char *name)
{
	if (name == nullptr)
		return false;

	struct stat buffer;
	return (stat (name, &buffer) == 0);
}

float calc_percentile (std::vector<short> vec, double target)
{
	std::sort (vec.begin ( ), vec.end ( ));
	const auto size = vec.size ( );
	const auto n    = (size - 1) * target + 1;
	//const auto n = (size + 1) * target;
	if (n == 1.0)
		return vec[0];
	if (n == size)
		return vec[size - 1];
	const auto k = static_cast<int> (n);
	const auto d = n - k;
	return vec[k - 1] + d * (vec[k] - vec[k - 1]);
}

float calc_median (std::vector<short> vec)
{
	std::sort (vec.begin ( ), vec.end ( ));

	const auto size = vec.size ( );
	if (size % 2 == 0)
	{
		const auto s = size / 2.f;
		return (vec[s - 1] + vec[s]) / 2.f;
	}
	else
	{
		return vec[size / 2];
	}
}

float calc_average (const std::vector<short> &vec)
{
	auto sum = 0.f;

	for (auto v : vec)
		sum += v;

	return sum / vec.size ( );
}

int main (int count, char *argv[])
{
	const auto path = argv[1];

	if (!file_exists (path))
	{
		throw (std::runtime_error ("path is incorrect!"));
	}

	std::ifstream file (path);
	if (!file)
		throw (std::runtime_error ("file is broken!"));

	std::vector<short> vec;
	std::string        str;
	while (std::getline (file, str))
	{
		const auto temp = std::stoi (str);
		if (temp < std::numeric_limits<short>::min ( ) || temp > std::numeric_limits<short>::max ( ))
			throw (std::runtime_error ("file is broken!"));

		if (vec.size ( ) == 1000)
			throw (std::runtime_error ("file is too big!"));

		vec.emplace_back (temp);
	}

	for (auto val : {calc_percentile (vec, 0.9), calc_median (vec), static_cast<float> (*std::max_element (vec.begin ( ), vec.end ( ))), static_cast<float> (*std::min_element (vec.begin ( ), vec.end ( ))), calc_average (vec)})
	{
		auto buff = std::make_unique<char[]> (64);
		sprintf_s (buff.get ( ), 64, "%.2f", val);

		std::cout << buff << std::endl;
	}
}
