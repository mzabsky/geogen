#include <iostream>

#include "../GeoGen/GeoGen.hpp"

#include "BasicOperatorsTests.hpp"
#include "FlowControlTests.hpp"
#include "VariablesTests.hpp"

using namespace std;

int main()
{
	int numberOfFailures = 0;
	int numberOfPassed = 0;

	RUN_FIXTURE(BasicOperatorsTests);
	RUN_FIXTURE(FlowControlTests);
	RUN_FIXTURE(VariablesTests);

	cout << "================================================================" << endl << "Finished! " << numberOfFailures << " tests failed, " << numberOfPassed << " tests passed.";

	return numberOfFailures > 0 ? 1 : 0;
}