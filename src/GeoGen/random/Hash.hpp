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

#include "../String.hpp"

namespace geogen
{
	namespace random
	{		
		/// Calculates a hash of @a x. 
		/// @param x The x.
		/// @return The hash.
		inline unsigned Hash(unsigned x)
		{
			x = ((x >> 16) ^ x) * 4256233;
			x = ((x >> 16) ^ x) * 4256249;
			x = ((x >> 16) ^ x);
			return x;
		}

		/// Calculates a hash of @a a and @a b. 
		/// @param a The a.
		/// @param b The b.
		/// @return The hash.
		inline unsigned Hash(unsigned a, unsigned b)
		{
			return Hash(((a * 1028385129 + b * 945191568 + 2015177) /** 141650963*/) ^ 1028385129);
		}

		/// Calculates a hash of @a a, @a b and @a c. 
		/// @param a The a.
		/// @param b The b.
		/// @param c The c.
		/// @return The hash.
		inline unsigned Hash(unsigned a, unsigned b, unsigned c)
		{
			return Hash(Hash(a, b), c);
		}

		/// Calculates a hash of @a a, @a b, @a c and @a d. 
		/// @param a The a.
		/// @param b The b.
		/// @param c The c.
		/// @param d The d.
		/// @return The hash.
		inline unsigned Hash(unsigned a, unsigned b, unsigned c, unsigned d)
		{
			return Hash(Hash(Hash(a, b), c), d);
		}

		/// Calculates a hash of @a a, @a b, @a c, @a d and @a e. 
		/// @param a The a.
		/// @param b The b.
		/// @param c The c.
		/// @param d The d.
		/// @param e The e.
		/// @return The hash.
		inline unsigned Hash(unsigned a, unsigned b, unsigned c, unsigned d, unsigned e)
		{
			return Hash(Hash(Hash(Hash(a, b), c), d), e);
		}

		/// Calculates a hash of @a str. 
		/// @param str The str.
		/// @return The hash.
		inline unsigned Hash(String str)
		{
			// djb2 hash algorithm
			Char const* cStr = str.c_str();

			unsigned long hash = 5381;
			int c;

			while (c = *cStr++)
				hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

			return hash;
		}
	}
}