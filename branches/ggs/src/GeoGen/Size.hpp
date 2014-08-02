#pragma once

namespace geogen {
	typedef unsigned Size1D;

	const Size1D SIZE1D_MAX = UINT_MAX;

	class Size2D{
	private:
		Size1D width, height;
	public:
		Size2D() : width(0), height(0) {}
		Size2D(Size1D width, Size1D height) : width(width), height(height) {}

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

		inline Size2D Size2D::operator*(const double other) const
		{
			return Size2D((Size1D)(this->width * other), (Size1D)(this->width * other));
		}

		Size2D& Size2D::operator*=(const double other)
		{
			*this = *this * other;
			return *this;
		}

		inline Size1D GetWidth() const { return this->width; }
		inline Size1D GetHeight() const { return this->height; }
		inline unsigned long long GetTotalLength() const { return this->width * this->height; }
	};

	const Size2D SIZE2D_MAX = Size2D(SIZE1D_MAX, SIZE1D_MAX);
}