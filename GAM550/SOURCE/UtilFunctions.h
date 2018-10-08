/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef INFACT_UTIL_FUNCS_H
#define INFACT_UTIL_FUNCS_H

std::string CreateErrorMessage(ID3DBlob* pErrors);

void CreateErrorDialog(ID3DBlob* pErrors, const std::string& msg);

#endif