#pragma once
#include<string>
#include<GL/glew.h>
#include "Black_Cat/Base/Component.h"

namespace BLK_Cat {
	struct Texture : public Component
	{
		Texture(const std::string& filename)
		{
			_filename = filename;
		}

		virtual ~Texture() {}


		GLuint _texture;
		std::string _filename;
	};
}
