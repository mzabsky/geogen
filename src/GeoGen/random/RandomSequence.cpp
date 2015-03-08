#include "RandomSequence.hpp"

using namespace std;
using namespace geogen;
using namespace random;
using namespace mtrand;

RandomSequence::RandomSequence(RandomSeed seed) : mtRand(seed)
{
}