
#include "Texture.h"

using namespace tdogl;

static GLenum TextureFormatForBitmapFormat(Bitmap::Format format)
{
	switch (format)
	{
	case Bitmap::Format_Grayscale: return GL_LUMINANCE;
	case Bitmap::Format_GrayscaleAlpha: return GL_LUMINANCE_ALPHA;
	case Bitmap::Format_RGB: return GL_RGB;
	case Bitmap::Format_RGBA: return GL_RGBA;
	default: throw std::runtime_error("Unrecognised Bitmap::Format");
	}
}

GLuint Texture::object() const
{
	return _object;
}

//
// TODO: texture creation
//
Texture::Texture(const Bitmap& bitmap, GLuint minMagFiler, GLuint wrapMode)
{
	/*/ // 11/3/2016 
	void GenTextures(sizei n, uint *textures);

	void BindTexture(enum target, uint texture);
	target: TEXTURE_{1D, 2D}{_ARRAY},
	TEXTURE_{3D, RECTANGLE, BUFFER},
	TEXTURE_CUBE_MAP{_ARRAY},
	TEXTURE_2D_MULTISAMPLE{_ARRAY}
	
	void DeleteTextures(sizei n, const uint *textures);
	boolean IsTexture(uint texture);

	void TexImage2D(enum target,
					int level,
					int internalformat,
					sizei width,
					sizei height,
					int border,
					enum format,
					enum type,
					const void *data);
	target: PROXY_TEXTURE_CUBE_MAP,
	POSITIVE_{X, Y, Z}, NEGATIVE_{X, Y, Z}
	internalformat, format, type: see TexImage3D

	void TexParameter{i f}(enum target,	enum pname, T param);
	target: see BindTexture

	void TexParameter{i f}v(enum target, enum pname, const T *params);
	target: see BindTexture, plus
	TEXTURE_{BORDER_COLOR, SWIZZLE_RGBA}

	void TexParameterI{i ui}v(enum target, enum pname, const T *params);
	target: see BindTexture, plus
	TEXTURE_{BORDER_COLOR, SWIZZLE_RGBA}
	pname: DEPTH_STENCIL_TEXTURE_MODE or
	TEXTURE_xwhere x may be one of
	WRAP_{S, T, R}, BORDER_COLOR, {MIN, MAG}_FILTER,
	LOD_BIAS,{MIN, MAX}_LOD, {BASE, MAX}_LEVEL,
	SWIZZLE_{R, G, B, A, RGBA}, COMPARE_{MODE, FUNC}
	[Table 8.16]
	//*/
	glGenTextures(1, &_object);
	glBindTexture(GL_TEXTURE_2D, _object);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFiler);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFiler);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 TextureFormatForBitmapFormat(bitmap.format()),
				 (GLsizei)bitmap.width(),
				 (GLsizei)bitmap.height(),
				 0,
				 TextureFormatForBitmapFormat(bitmap.format()),
				 GL_UNSIGNED_BYTE,
				 bitmap.pixelBuffer());

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &_object);
}
