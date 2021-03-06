/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

namespace JsonReader {
	std::vector<Point> ParsePoints(const json & j, const std::string & first)
	{
		std::vector<Point> points;
		if (j.find(first) != j.end()) {
			int size = int(j[first].size());
			for (int i = 0; i < size; ++i) {
				Point p;
				p.x = j[first][i]["x"];
				p.y = j[first][i]["y"];
				points.push_back(p);
			}
		}
		return points;
	}

	std::vector<Point> ParsePoints(const json & j, const std::string & first, const std::string & second)
	{
		std::vector<Point> points;
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				int size = int(j[first][second].size());
				for (int i = 0; i < size; ++i) {
					Point p;
					p.x = j[first][second][i]["x"];
					p.y = j[first][second][i]["y"];
					points.push_back(p);
				}
			}
		}
		return points;
	}

	float ParseFloat(const json& j, const std::string& first)
	{
		if (j.find(first) != j.end())
				return j[first];

		return 0.0f;
	}

	float ParseFloat(const json& j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return 0.0f;
	}

	float ParseFloat(const json& j, const std::string& first, const std::string& second, const std::string& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return 0.0f;
	}

	double ParseDouble(const json& j, const std::string& first)
	{
		if (j.find(first) != j.end()) return j[first];
		return 0.0;
	}

	double ParseDouble(const json& j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return 0.0;
	}

	double ParseDouble(const json& j, const std::string& first, const std::string& second, const std::string& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return 0.0;
	}

	int ParseInt(const json& j, const std::string& first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return 0;
	}

	int ParseInt(const json& j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return 0;
	}

	int ParseInt(const json& j, const std::string& first, const std::string& second, const std::string& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return 0;
	}

	std::string ParseStringUnsafe(const json & j, const std::string & first)
	{
		return j[first];
	}

	std::string ParseString(const json& j, const std::string& first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return "";
	}

	std::string ParseString(const json& j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return "";
	}

	std::string ParseString(const json& j, const std::string& first, const std::string& second, const std::string& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return "";
	}

	bool ParseBool(const json& j, const std::string& first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return false;
	}

	bool ParseBool(const json& j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return false;
	}

	bool ParseBool(const json& j, const std::string& first, const std::string& second, const std::string& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return false;
	}

	std::vector<std::string> ParseStringList(const json& j, const std::string& first)
	{
		if (j.find(first) != j.end())
			return j[first];
		return std::vector<std::string>();
	}

	std::vector<std::string> ParseStringList(const json& j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end())
				return j[first][second];
		}
		return std::vector<std::string>();
	}

	std::vector<std::string> ParseStringList(const json& j, const std::string& first, const std::string& second, const std::string& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return j[first][second][third];
			}
		}
		return std::vector<std::string>();
	}

	Vector2D ParseVector2D(const json& j, const std::string& first)
	{
		if (j.find(first) != j.end()) {
			return Vector2D(
				ParseFloat(j[first], "x"),
				ParseFloat(j[first], "y"),
				ParseFloat(j[first], "w")
			);
		}
		return Vector2D();
	}

	Vector2D ParseVector2D(const json& j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				return Vector2D(
					ParseFloat(j[first][second], "x"),
					ParseFloat(j[first][second], "y"),
					ParseFloat(j[first][second], "w")
				);
			}
		}
		return Vector2D();
	}

	Vector3D ParseVector3D(const json& j, const std::string& first)
	{
		if (j.find(first) != j.end()) {
			return Vector3D(
				ParseFloat(j[first], "x"),
				ParseFloat(j[first], "y"),
				ParseFloat(j[first], "z"),
				ParseFloat(j[first], "w")
			);
		}
		return Vector3D();
	}

	Vector3D ParseVector3D(const json& j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				return Vector3D(
					ParseFloat(j[first][second], "x"),
					ParseFloat(j[first][second], "y"),
					ParseFloat(j[first][second], "z"),
					ParseFloat(j[first][second], "w")
				);
			}
		}
		return Vector3D();
	}

	Color ParseColor(const json& j, const std::string& first)
	{
		if (j.find(first) != j.end()) {
			return Color(
				ValueExists(j, first) ? j[first]["r"] : 1.0f,
				ValueExists(j, first) ? j[first]["g"] : 1.0f,
				ValueExists(j, first) ? j[first]["b"] : 1.0f,
				ValueExists(j, first) ? j[first]["a"] : 1.0f
			);
		}
		return Color(1, 1, 1, 1);
	}

	Color ParseColor(const json& j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				return Color(
					ValueExists(j, first, second) ? j[first][second]["r"] : 1.0f,
					ValueExists(j, first, second) ? j[first][second]["g"] : 1.0f,
					ValueExists(j, first, second) ? j[first][second]["b"] : 1.0f,
					ValueExists(j, first, second) ? j[first][second]["a"] : 1.0f
				);
			}
		}
		return Color(1, 1, 1, 1);
	}

	CompressedColor ParseCompColor(const json& j, const std::string& first)
	{
		if (j.find(first) != j.end()) {
			return CompressedColor(
				ValueExists(j, first) ? j[first]["r"] : 255,
				ValueExists(j, first) ? j[first]["g"] : 255,
				ValueExists(j, first) ? j[first]["b"] : 255,
				ValueExists(j, first) ? j[first]["a"] : 255
			);
		}
		return CompressedColor(255, 255, 255, 255);
	}

	CompressedColor ParseCompColor(const json & j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				return CompressedColor(
					ValueExists(j, first, second) ? j[first][second]["r"] : 255,
					ValueExists(j, first, second) ? j[first][second]["g"] : 255,
					ValueExists(j, first, second) ? j[first][second]["b"] : 255,
					ValueExists(j, first, second) ? j[first][second]["a"] : 255
				);
			}
		}
		return CompressedColor(255, 255, 255, 255);
	}

	bool ValueExists(const json& j, const std::string& first)
	{
		return j.find(first) != j.end();
	}

	bool ValueExists(const json& j, const std::string& first, const std::string& second)
	{
		if (j.find(first) != j.end()) {
			return j[first].find(second) != j[first].end();
		}
		return false;
	}

	bool ValueExists(const json& j, const std::string& first, const std::string& second, const std::string& third)
	{
		if (j.find(first) != j.end()) {
			if (j[first].find(second) != j[first].end()) {
				if (j[first][second].find(third) != j[first][second].end())
					return true;
			}
		}
		return false;
	}

	json OpenJsonFile(const std::string& fileName)
	{
		try {
			std::ifstream i(fileName);
			json j;
			i >> j;
			i.close();
			return j;
		}
		catch (json::parse_error &e) {
			std::cout << e.what() << " in" + fileName << std::endl;
			return 0;
		}
	}

	bool KeyIs(json::iterator it, const std::string& key)
	{
		return it.key().compare(key) == 0;
	}

	void SerializeInterpolationItem(InterpolationItem & i, const json & j, const std::string & first)
	{
		i.points = ParsePoints(j, first, "points");
		i.amplitude = ParseFloat(j, first, "amplitude");
	}

}

