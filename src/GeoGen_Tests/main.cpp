#include <iostream>

#include "../GeoGen/GeoGen.hpp"
#include "BasicRuntimeTests.hpp"

using namespace std;

int main()
{
	int numberOfFailures = 0;
	int numberOfPassed = 0;

	BasicRuntimeTests().Run(numberOfFailures, numberOfPassed);

	cout << "================================================================" << endl << "Finished! " << numberOfFailures << " tests failed, " << numberOfPassed << " tests passed.";

	return numberOfFailures > 0 ? 1 : 0;
}