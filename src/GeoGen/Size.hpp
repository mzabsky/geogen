/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <climits>

#include "Serializable.hpp"

namespace geogen {
	/// Size of a 1D region.
	typedef unsigned Size1D;

	/// Maximum 1D size.
	const Size1D SIZE1D_MAX = UINT_MAX;

	/// Size of a 2D region with width and height.
	class Size2D : public Serializable
	{
	private:
		Size1D width, height;
	public:
		/// Default constructor.
		Size2D() : width(0), height(0) {}

		/// Constructor.
		/// @param width The width.
		/// @param height The height.
		Size2D(Size1D width, Size1D height) : width(width), height(height) {}

		inline Size2D operator+(const Size2D& other) const
		{
			return Size2D(this->width + other.GetWidth(), this->height + other.GetHeight());
		}

		Size2D& operator+=(const Size2D& other)
		{
			*this = *this + other;
			return *this;
		}

		inline Size2D operator-(const Size2D& other) const
		{
			return Size2D(this->width - other.GetWidth(), this->height - other.GetHeight());
		}

		Size2D& operator-=(const Size2D& other)
		{
			*this = *this - other;
			return *this;
		}

		inline Size2D operator*(const double other) const
		{
			return Size2D((Size1D)(this->width * other), (Size1D)(this->height * other));
		}

		Size2D& operator*=(const double other)
		{
			*this = *this * other;
			return *this;
		}

		/// Gets the width.
		/// @return The width.
		inline Size1D GetWidth() const { return this->width; }

		/// Gets the height.
		/// @return The height.
		inline Size1D GetHeight() const { return this->height; }

		/// Gets total number of pixels in the region.
		/// @return The total length.
		inline unsigned GetTotalLength() const { return this->width * this->height; }

		void Serialize(IOStream& stream) const
		{
			stream << GG_STR("[") << this->width << GG_STR(", ") << this->height << GG_STR("]");
		}
	};

	/// Maximum 2D size.
	const Size2D SIZE2D_MAX = Size2D(SIZE1D_MAX, SIZE1D_MAX);
}