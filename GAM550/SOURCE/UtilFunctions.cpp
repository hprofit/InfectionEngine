#include "Stdafx.h"

std::string CreateErrorMessage(ID3DBlob * pErrors)
{
	return std::string((const char *)(pErrors->GetBufferPointer()), pErrors->GetBufferSize());
}

void CreateErrorDialog(ID3DBlob * pErrors, const std::string& msg)
{
	if (pErrors) {
		std::string errorListString = std::string(msg + CreateErrorMessage(pErrors));
		MessageBox(nullptr, errorListString.c_str(), "Error", MB_OK);
	}
	else {
		MessageBox(nullptr, msg.c_str(), "Error", MB_OK);
	}
}
