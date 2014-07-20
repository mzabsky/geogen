#pragma once

namespace geogen {
	typedef unsigned Size1D;

	class Size2D{
	private:
		unsigned width, height;
	public:
		Size2D() : width(0), height(0) {}
		Size2D(unsigned width, unsigned height) : width(width), height(height) {}

		inline unsigned GetWidth() const { return this->width; }
		inline unsigned GetHeight() const { return this->height; }
		inline unsigned GetTotalLength() const { return this->width * this->height; }
	};
}