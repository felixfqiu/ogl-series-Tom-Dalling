#pragma once

#include <gl/glew.h>
#include "Bitmap.h"

namespace tdogl
{
	class Texture
	{
	public:
		Texture(const Bitmap& bitmap,
				GLuint minMagFiler = GL_LINEAR,
				GLuint wrapMode = GL_CLAMP_TO_EDGE);
		~Texture();

		GLuint object() const;

	private:
		GLuint _object;
	};
};
