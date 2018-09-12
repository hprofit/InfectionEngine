/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef  FACE_H
#define FACE_H 

struct Face {
	DWORD index[3];
	Face(DWORD v1, DWORD v2, DWORD v3) {
		index[0] = v1; index[1] = v2; index[2] = v3;
	}
	Face(const aiFace& face) {
		index[0] = face.mIndices[0];
		index[1] = face.mIndices[1];
		index[2] = face.mIndices[2];
	}
	DWORD operator[](int i) const { return index[i]; }
	DWORD& operator[](int i) { return index[i]; }
	operator DWORD*() { return &(index[0]); }
};

#endif // ! FACE_H
