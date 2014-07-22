#pragma once

namespace geogen {
	typedef unsigned Size1D;

	class Size2D{
	private:
		unsigned width, height;
	public:
		Size2D() : width(0), height(0) {}
		Size2D(unsigned width, unsigned height) : width(width), height(height) {}

		inline Size2D Size2D::operator+(const Size2D& other) const
		{
			return Size2D(this->width + other.GetWidth(), this->width + other.GetHeight());
		}

		Size2D& Size2D::operator+=(const Size2D& other)
		{
			*this = *this + other;
			return *this;
		}

		inline Size2D Size2D::operator-(const Size2D& other) const
		{
			return Size2D(this->width - other.GetWidth(), this->width - other.GetHeight());
		}

		Size2D& Size2D::operator-=(const Size2D& other)
		{
			*this = *this - other;
			return *this;
		}

		inline unsigned GetWidth() const { return this->width; }
		inline unsigned GetHeight() const { return this->height; }
		inline unsigned GetTotalLength() const { return this->width * this->height; }
	};
}