/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef TETRA_COLOR_H
#define TETRA_COLOR_H

struct Color {
	Color() : r(0), g(0), b(0), a(0) {}
	Color(FLOAT _r, FLOAT _g, FLOAT _b, FLOAT _a) : r(_r), g(_g), b(_b), a(_a) {}
	FLOAT r, g, b, a;

	FLOAT operator[] (const int& idx) const {
		return *(&r + idx);
	}

	FLOAT& operator[] (const int& idx) {
		return *(&r + idx);
	}

	operator const FLOAT*() const {
		return (&r);
	}
};

#endif