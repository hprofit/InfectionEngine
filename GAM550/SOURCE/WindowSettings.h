/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#ifndef INFECT_WINDOW_SETTINGS_H
#define INFECT_WINDOW_SETTINGS_H

struct WindowSettings {
	unsigned int Width, Height;
	float AspectRatio;
	bool FullScreen;
	std::string WindowTitle;
	bool VSync;
};

#endif