/* Start Header -------------------------------------------------------
Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: JsonReader.h
Purpose: A set of functions for opening and reading a JSON file.
Language: C++
Platform: Windows 8.1 / 10.0.15063.0  or  GCC 7.1.0
Project: CS529_holdenprofit_
Author: Holden Profit, holden.profit, 42833
Creation date: 10/29/17
- End Header --------------------------------------------------------*/

#pragma once

#ifndef JSON_READER_H
#define JSON_READER_H

namespace JsonReader {
	std::vector<Point> ParsePoints(const json& j, const std::string&);
	std::vector<Point> ParsePoints(const json& j, const std::string&, const std::string&);

	float ParseFloat(const json& j, const std::string&);
	float ParseFloat(const json& j, const std::string&, const std::string&);
	float ParseFloat(const json& j, const std::string&, const std::string&, const std::string&);

	double ParseDouble(const json& j, const std::string&);
	double ParseDouble(const json& j, const std::string&, const std::string&);
	double ParseDouble(const json& j, const std::string&, const std::string&, const std::string&);

	int ParseInt(const json& j, const std::string&);
	int ParseInt(const json& j, const std::string&, const std::string&);
	int ParseInt(const json& j, const std::string&, const std::string&, const std::string&);

	std::string ParseStringUnsafe(const json& j, const std::string&);
	std::string ParseString(const json& j, const std::string&);
	std::string ParseString(const json& j, const std::string&, const std::string&);
	std::string ParseString(const json& j, const std::string&, const std::string&, const std::string&);

	bool ParseBool(const json& j, const std::string&);
	bool ParseBool(const json& j, const std::string&, const std::string&);
	bool ParseBool(const json& j, const std::string&, const std::string&, const std::string&);

	std::vector<std::string> ParseStringList(const json& j, const std::string&);
	std::vector<std::string> ParseStringList(const json& j, const std::string&, const std::string&);
	std::vector<std::string> ParseStringList(const json& j, const std::string&, const std::string&, const std::string&);

	Vector2D ParseVector2D(const json& j, const std::string&);
	Vector2D ParseVector2D(const json& j, const std::string&, const std::string&);
	
	Vector3D ParseVector3D(const json& j, const std::string&);
	Vector3D ParseVector3D(const json& j, const std::string&, const std::string&);

	Vector3D ParseColor(const json& j, const std::string&);
	Vector3D ParseColor(const json& j, const std::string&, const std::string&);

	bool ValueExists(const json& j, const std::string&);
	bool ValueExists(const json& j, const std::string&, const std::string&);
	bool ValueExists(const json& j, const std::string&, const std::string&, const std::string&);
	
	json OpenJsonFile(const std::string& fileName);

	bool KeyIs(json::iterator it, const std::string& key);

	void SerializeInterpolationItem(InterpolationItem& i, const json& j, const std::string&);
}

#endif