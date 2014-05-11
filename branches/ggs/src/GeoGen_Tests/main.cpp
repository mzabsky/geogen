#include <iostream>

#include "../GeoGen/GeoGen.hpp"

#include "BasicOperatorsTests.hpp"
#include "FlowControlTests.hpp"
#include "VariablesTests.hpp"
#include "CommentTests.hpp"
#include "FunctionTests.hpp"
#include "MetadataTests.hpp"
#include "ArrayTests.hpp"
#include "CompositeTests.hpp"

using namespace std;

int main()
{
	int numberOfFailures = 0;
	int numberOfPassed = 0;

	RUN_FIXTURE(BasicOperatorsTests);
	RUN_FIXTURE(FlowControlTests);
	RUN_FIXTURE(VariablesTests);
	RUN_FIXTURE(CommentTests);
	RUN_FIXTURE(FunctionTests);
	RUN_FIXTURE(MetadataTests);
	RUN_FIXTURE(ArrayTests);
	RUN_FIXTURE(CompositeTests);

	cout << "================================================================" << endl << "Finished! " << numberOfFailures << " tests failed, " << numberOfPassed << " tests passed.";

	return numberOfFailures > 0 ? 1 : 0;
}