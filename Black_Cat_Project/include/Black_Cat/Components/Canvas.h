#pragma once

#include "Display.h"
#include "Black_Cat/Base/Component.h"

namespace BLK_Cat {
	struct Canvas : public Component
	{

		Canvas(int width, int height, std::string& tittle)
		{
			_width = width;
			_height = height;
			_tittle = tittle;
			_display = nullptr;
		}

		virtual ~Canvas() {}

		int _width;
		int _height;
		std::string _tittle;
		Display* _display;
	};
}