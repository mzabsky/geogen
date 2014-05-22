#pragma once

namespace geogen {
	class Point{
	private:
		int x, y;
	public:
		Point() : x(0), y(0) {}
		Point(int x, int y) : x(x), y(y) {}

		inline int GetX() const { return this->x; }
		inline int GetY() const { return this->y; }
	};
}