#pragma once

#include <map>
#include <algorithm>

#include "../Point.hpp"
#include "../Direction.hpp"
#include "../Rectangle.hpp"

namespace geogen
{
	namespace genlib
	{
		/// Linear transformation matrix used for HeightMap::Transform.
		class TransformationMatrix
		{
		public:

			/// Element [1, 1].
			double A11;
			
			/// Element [1, 2].
			double A12;
			
			/// Element [2, 1].
			double A21; 
			
			/// Element [2, 2].
			double A22;

			/// Gets X coordinate of a point transformed by this matrix. Preserves floating point precision.
			/// @param point The point.
			/// @return The transformed X coordinate.
			inline double GetTransformedX(Point point) const
			{
				return (point.GetX() * this->A11) + (point.GetY() * this->A12);
			}

			/// Gets Y coordinate of a point transformed by this matrix. Preserves floating point precision.
			/// @param point The point.
			/// @return The transformed Y coordinate.
			inline double GetTransformedY(Point point) const
			{
				return (point.GetX() * this->A21) + (point.GetY() * this->A22);
			}

			/// Transforms the point with this matrix. Discards fractional part of the transformed coordinates.
			/// @param point The point to transform.
			/// @return The transformed point.
			inline Point TransformPoint(Point point) const
			{
				return Point(
					Coordinate(this->GetTransformedX(point)),
					Coordinate(this->GetTransformedY(point)));
			}

			/// Transforms the rectangle with this matrix. Discards fractional part of the transformed coordinates.
			/// @param point The rectangle to transform.
			/// @return Bounding rectangle of the transformed rectangle.
			inline Rectangle TransformRectangle(Rectangle rectangle) const
			{
				double transformedTopLeftX = this->GetTransformedX(rectangle.GetPosition());
				double transformedTopLeftY = this->GetTransformedY(rectangle.GetPosition());
				double transformedTopRightX = this->GetTransformedX(Point(rectangle.GetEndingPoint().GetX(), rectangle.GetPosition().GetY()));
				double transformedTopRightY = this->GetTransformedY(Point(rectangle.GetEndingPoint().GetX(), rectangle.GetPosition().GetY()));
				double transformedBottomLeftX = this->GetTransformedX(Point(rectangle.GetPosition().GetX(), rectangle.GetEndingPoint().GetY()));
				double transformedBottomLeftY = this->GetTransformedY(Point(rectangle.GetPosition().GetX(), rectangle.GetEndingPoint().GetY()));
				double transformedBottomRightX = this->GetTransformedX(rectangle.GetEndingPoint());
				double transformedBottomRightY = this->GetTransformedY(rectangle.GetEndingPoint());

				Coordinate left = (Coordinate)RoundAway((std::min)(transformedTopLeftX, (std::min)(transformedTopRightX, (std::min)(transformedBottomLeftX, transformedBottomRightX))));
				Coordinate right = (Coordinate)RoundAway((std::max)(transformedTopLeftX, (std::max)(transformedTopRightX, (std::max)(transformedBottomLeftX, transformedBottomRightX))));
				Coordinate top = (Coordinate)RoundAway((std::min)(transformedTopLeftY, (std::min)(transformedTopRightY, (std::min)(transformedBottomLeftY, transformedBottomRightY))));
				Coordinate bottom = (Coordinate)RoundAway((std::max)(transformedTopLeftY, (std::max)(transformedTopRightY, (std::max)(transformedBottomLeftY, transformedBottomRightY))));

				return Rectangle(
					Point(left, top),
					Size2D(right - left + 1, bottom - top + 1));
			}

			/// Determines whether the matrix is invertible (has ).
			/// @return true if invertible, false if not.
			/// @see http://en.wikipedia.org/wiki/Invertible_matrix
			bool IsInvertible() const
			{
				return this->A11 * this->A22 - this->A12 * this->A21 != 0;
			}

			/// Creates a matrix that is an inverse of another matrix.
			/// @param m The matrix to invert.
			/// @return The inverted matrix.
			/// @see http://en.wikipedia.org/wiki/Inverse_element#Matrices
			static TransformationMatrix Inverse(TransformationMatrix m)
			{
				TransformationMatrix inverse;
				inverse.A11 = m.A22 / (-(m.A12 * m.A21) + m.A11 * m.A22);
				inverse.A12 = -(m.A12 / (-(m.A12 * m.A21) + m.A11 * m.A22));
				inverse.A21 = -(m.A21 / (-(m.A12 * m.A21) + m.A11 * m.A22));
				inverse.A22 = m.A11 / (-(m.A12 * m.A21) + m.A11 * m.A22);

				return inverse;
			}
		};
	}
}