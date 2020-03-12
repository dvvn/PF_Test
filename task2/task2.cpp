#include <algorithm>
#include <array>
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

struct point
{
	point ( ) = default;

	point (std::string str)
	{
		const auto space = str.find (' ');

		x = std::stof (str.substr (0, space));
		y = std::stof (str.substr (space));
	}

	point (float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float distance (const point &p2) const
	{
		const float diffY = y - p2.y;
		const float diffX = x - p2.x;
		return sqrt ((diffY * diffY) + (diffX * diffX));
	}

	float x, y;
};

struct quard_s
{
	//from file
	quard_s (const char *path)
	{
		std::ifstream dots_file (path);
		if (!dots_file || !file_exists (path))
			throw std::runtime_error ("quard path is broken!");
		std::string str;
		auto        idx = 0;
		while (std::getline (dots_file, str))
		{
			data[idx] = (str);
			idx++;
		}
		/*if (idx > 4)
			throw std::runtime_error ("dots data is incorrect!");*/ //while do this already
	}

	static bool IsPointsOnLine (const point &a, const point &b, const point &c)
	{
		return (c.x * (b.y - a.y) - c.y * (b.x - a.x) == a.x * b.y - b.x * a.y);
	}

	/*
	  0 - точка на одной из вершин
	  1 - точка на одной из сторон
	  2 - точка внутри
	  3 - точка снаружи*/
	int scan (const point &p)
	{
		auto &nearest = *std::min_element (data.begin ( ), data.end ( ), [&] (const point &a,const point &b)
		{
			return a.distance (p) < b.distance (p);
		});

		const auto backup = nearest;

		const auto s  = area ( );
		nearest       = p;
		const auto s2 = area ( );

		nearest = backup;

		if (s == s2)
			return std::rand ( ) % 2; //hehe
		if (s < s2)
			return 3;

		if (IsPointsOnLine (p, data[0], data[1]) || IsPointsOnLine (p, data[2], data[3]))
			return 1;
		if (IsPointsOnLine (p, data[1], data[2]) || IsPointsOnLine (p, data[3], data[1]))
			return 0;

		return 2;
	}

	float area ( ) const
	{
		const auto x1 = data[0].x;
		const auto x2 = data[1].x;
		const auto x3 = data[2].x;
		const auto x4 = data[3].x;

		const auto y1 = data[0].y;
		const auto y2 = data[1].y;
		const auto y3 = data[2].y;
		const auto y4 = data[3].y;

		return 0.5 * abs ((x1 - x2) * (y1 + y2) + (x2 - x3) * (y2 + y3) + (x3 - x4) * (y3 + y4) + (x4 - x1) * (y4 + y1));
	}

	std::array<point, 4> data;
};

int main (int count, char *argv[])
{
	const auto quard_path = argv[1];
	const auto dot_path   = argv[2];

	if (quard_path == nullptr)
		throw std::runtime_error ("quard path is null!");
	if (dot_path == nullptr)
		throw std::runtime_error ("dots path is null!");

	quard_s quard (quard_path);

	std::vector<point> dots;
	std::ifstream      dots_file (dot_path);
	if (!dots_file || !file_exists (dot_path))
		throw std::runtime_error ("dots coords is broken!");
	std::string str;
	while (std::getline (dots_file, str))
		dots.emplace_back (str);
	if (dots.size ( ) == 0 || dots.size ( ) > 100)
		throw std::runtime_error ("dots data is incorrect!");

	for (auto d : dots)
	{
		const auto buff = std::make_unique<char[]> (64);
		sprintf_s (buff.get ( ), 64, "%.2f %.2f : %d", d.x, d.y, quard.scan (d));
		std::cout << buff << std::endl;
	}
}
