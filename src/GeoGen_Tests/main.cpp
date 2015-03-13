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

#include <GeoGen/GeoGen.hpp>

#include "BasicOperatorsTests.hpp"
#include "FlowControlTests.hpp"
#include "VariablesTests.hpp"
#include "CommentTests.hpp"
#include "FunctionTests.hpp"
#include "MetadataTests.hpp"
#include "ArrayTests.hpp"
#include "CompositeTests.hpp"
#include "MathTests.hpp"
#include "MessageTests.hpp"
#include "StringTests.hpp"
#include "EnumTests.hpp"
#include "CoordinateTests.hpp"
#include "RendererTests.hpp"
#include "MetadataTests.hpp"
#include "RandomTests.hpp"

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
	RUN_FIXTURE(MathTests);
	RUN_FIXTURE(MessageTests);
	RUN_FIXTURE(StringTests);
	RUN_FIXTURE(EnumTests);
	RUN_FIXTURE(CoordinateTests);
	RUN_FIXTURE(RendererTests);
	RUN_FIXTURE(RandomTests);

	cout << "================================================================" << endl << "Finished! " << numberOfFailures << " tests failed, " << numberOfPassed << " tests passed.";

	return numberOfFailures > 0 ? 1 : 0;
}