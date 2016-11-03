#pragma once
#include <string>

namespace tdogl
{
	class Bitmap
	{
	public:
		enum Format
		{
			Format_Grayscale = 1,
			Format_GrayscaleAlpha = 2,
			Format_RGB = 3,
			Format_RGBA = 4,
		};
		Bitmap(unsigned width ,
			   unsigned height,
			   Format   format,
			   const unsigned char* pixels = NULL);
		~Bitmap();
		Bitmap(const Bitmap& other);

		static Bitmap bitmapFromFile(std::string filePath);

		unsigned width () const;
		unsigned height() const;
		Format   format() const;
		unsigned char* pixelBuffer() const;

	private:
		unsigned _width ;
		unsigned _height;
		Format   _format;
		unsigned char* _pixels;

		void _set(unsigned width, unsigned height, Format format, const unsigned char* pixels);
	};
}