/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef TETRA_COLOR_H
#define TETRA_COLOR_H

typedef unsigned char UByte;

struct CompressedColor {
	UByte r, g, b, a;

	CompressedColor() : r(0), g(0), b(0), a(0) {}
	CompressedColor(UByte _r, UByte _g, UByte _b, UByte _a) : r(_r), g(_g), b(_b), a(_a) {}

	UByte operator[] (const int& idx) const {
		return *(&r + idx);
	}

	UByte& operator[] (const int& idx) {
		return *(&r + idx);
	}

	operator const UByte*() const {
		return &(r);
	}
};

static CompressedColor Lerp(const CompressedColor& colorA, const CompressedColor& colorB, float t)
{
	if (t > 1)
		t = 1;
	else if (t < 0)
		t = 0;
	float tMinus = 1.f - t;

	return CompressedColor(
		int(colorA.r * tMinus + t * colorB.r),
		int(colorA.g * tMinus + t * colorB.g),
		int(colorA.b * tMinus + t * colorB.b),
		int(colorA.a * tMinus + t * colorB.a)
	);
}



struct Color {
	FLOAT r, g, b, a;

	Color() : r(0), g(0), b(0), a(0) {}
	Color(FLOAT _r, FLOAT _g, FLOAT _b, FLOAT _a) : r(_r), g(_g), b(_b), a(_a) {}
	Color(const CompressedColor& _color) :
		r(FLOAT(_color.r) / 255.f),
		g(FLOAT(_color.g) / 255.f),
		b(FLOAT(_color.b) / 255.f),
		a(FLOAT(_color.a) / 255.f)
	{}

	FLOAT operator[] (const int& idx) const {
		return *(&r + idx);
	}

	FLOAT& operator[] (const int& idx) {
		return *(&r + idx);
	}

	operator const FLOAT*() const {
		return &(r);
	}
};

static Color Lerp(const Color& colorA, const Color& colorB, float t)
{
	if (t > 1)
		t = 1;
	else if (t < 0)
		t = 0;
	float tMinus = 1.f - t;

	return Color(
		colorA.r * tMinus + t * colorB.r,
		colorA.g * tMinus + t * colorB.g,
		colorA.b * tMinus + t * colorB.b,
		colorA.a * tMinus + t * colorB.a
	);
}


#endif