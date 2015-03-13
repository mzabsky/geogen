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

#include <vector>

#include "../Configuration.hpp"
#include "../runtime/CompiledScript.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class TypeDefinition;
	}

	namespace compiler 
	{		
		/// A script compiler based on ANTLR 3.
		class Compiler
		{
			const Configuration configuration;
		private:
			Compiler(const Compiler&); // Not copyable
		public:

			/// Default constructor.
			/// @param configuration (Optional) Configuration. Default is used if not specified.
			Compiler(Configuration configuration = Configuration()); 

			/// Compiles script string into a runtime::CompiledString.
			/// @param code The code.
			/// @return A compiled script.
			runtime::CompiledScript* CompileScript(String const& code) const;

			/// Gets the configuration of the compiler.
			/// @return The configuration.
			inline Configuration const& GetConfiguration() const { return this->configuration; }
		};
	}
}