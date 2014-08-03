#pragma once

#include "../String.hpp"

namespace geogen
{
	namespace random
	{		
		inline unsigned Hash(unsigned x)
		{
			x = ((x >> 16) ^ x) * 4256233;
			x = ((x >> 16) ^ x) * 4256249;
			x = ((x >> 16) ^ x);
			return x;
		}

		inline unsigned Hash(unsigned a, unsigned b)
		{
			return Hash(((a * 1028385129 + b * 945191568 + 2015177) /** 141650963*/) ^ 1028385129);
		}

		inline unsigned Hash(unsigned a, unsigned b, unsigned c)
		{
			return Hash(Hash(a, b), c);
		}

		inline unsigned Hash(unsigned a, unsigned b, unsigned c, unsigned d)
		{
			return Hash(Hash(Hash(a, b), c), d);
		}

		inline unsigned Hash(unsigned a, unsigned b, unsigned c, unsigned d, unsigned e)
		{
			return Hash(Hash(Hash(Hash(a, b), c), d), e);
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