#pragma once

namespace geogen {
	typedef unsigned Size1D;

	class Size2D{
	private:
		int width, height;
	public:
		Size2D() : width(0), height(0) {}
		Size2D(int width, int height) : width(width), height(height) {}

		inline int GetWidth() const { return this->width; }
		inline int GetHeight() const { return this->height; }
	};
}