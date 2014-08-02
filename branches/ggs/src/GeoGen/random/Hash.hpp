#pragma once

#include "../String.hpp"

namespace geogen
{
	namespace random
	{		
		inline unsigned Hash(unsigned x)
		{
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = ((x >> 16) ^ x);
			return x;
		}

		inline unsigned Hash(unsigned a, unsigned b)
		{
			return (a * 122949829 + b) * 141650963;
		}

		inline unsigned Hash(unsigned a, unsigned b, unsigned c)
		{
			return ((a * 122949829 + b) * 141650963 + c) * 160481219;
		}

		inline unsigned Hash(unsigned a, unsigned b, unsigned c, unsigned d)
		{
			return (((a * 122949829 + b) * 141650963 + c) * 160481219 + d) * 179424691;
		}

		inline unsigned Hash(unsigned a, unsigned b, unsigned c, unsigned d, unsigned e)
		{
			return ((((a * 122949829 + b) * 141650963 + c) * 160481219 + d) * 179424691 + e) * 198491329;
		}

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