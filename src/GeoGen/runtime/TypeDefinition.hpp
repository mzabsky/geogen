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

#include "SymbolDefinitionTable.hpp"

#include "VariableDefinition.hpp" // Necessary to properly compile the owning map.
#include "FunctionDefinition.hpp" // Necessary to properly compile the owning map.

namespace geogen 
{
	namespace runtime
	{
		class ManagedObject;
		class StaticObject;
		class VariableDefinition;
		class FunctionDefinition;
		class VirtualMachine;

		/// Base class for definitions of script classes.
		class TypeDefinition : public Serializable
		{
		private:
			String name;

			SymbolDefinitionTable<VariableDefinition> variableDefinitions;
			SymbolDefinitionTable<FunctionDefinition> functionDefinitions;
			SymbolDefinitionTable<VariableDefinition> staticVariableDefinitions;
			SymbolDefinitionTable<FunctionDefinition> staticFunctionDefinitions;
		protected:

			/// Constructor.
			/// @param name The class name.
			TypeDefinition(String const& name) : name(name) {}
		public:			
			/// Virtual destructor.
			virtual ~TypeDefinition() {};

			/// Gets the class name.
			/// @return The name.
			inline String const& GetName() const { return this->name; };			

			/// Gets variable definitions.
			/// @return The variable definitions.
			inline SymbolDefinitionTable<VariableDefinition>& GetVariableDefinitions() { return this->variableDefinitions; };

			/// Gets variable definitions.
			/// @return The variable definitions.
			inline SymbolDefinitionTable<VariableDefinition> const& GetVariableDefinitions() const { return this->variableDefinitions; };

			/// Gets function definitions.
			/// @return The function definitions.
			inline SymbolDefinitionTable<FunctionDefinition>& GetFunctionDefinitions() { return this->functionDefinitions; };

			/// Gets function definitions.
			/// @return The function definitions.
			inline SymbolDefinitionTable<FunctionDefinition> const& GetFunctionDefinitions() const { return this->functionDefinitions; };

			/// Gets static variable definitions.
			/// @return The static variable definitions.
			inline SymbolDefinitionTable<VariableDefinition>& GetStaticVariableDefinitions() { return this->staticVariableDefinitions; };

			/// Gets static variable definitions.
			/// @return The static variable definitions.
			inline SymbolDefinitionTable<VariableDefinition> const& GetStaticVariableDefinitions() const { return this->staticVariableDefinitions; };

			/// Gets static function definitions.
			/// @return The static function definitions.
			inline SymbolDefinitionTable<FunctionDefinition>& GetStaticFunctionDefinitions() { return this->staticFunctionDefinitions; };

			/// Gets static function definitions.
			/// @return The static function definitions.
			inline SymbolDefinitionTable<FunctionDefinition> const& GetStaticFunctionDefinitions() const { return this->staticFunctionDefinitions; };

			/// Creates and registers a static object representing the type during script's execution.
			/// @param vm The virtual machine.
			/// @return The created object.
			StaticObject* CreateStaticObject(VirtualMachine* vm) const;

			/// Initializes the type. Called during script execution startup.
			/// @param vm The virtual machine.
			virtual void Initialize(VirtualMachine* vm) const;

			/// Determines whether the type is an enum type.
			/// @return true if the type is an enum type, false otherwise.
			virtual bool IsEnumType() const { return false; };

			/// Determines whether instance A is less than B.
			/// @param a The instance A.
			/// @param b The instance B.
			/// @return true if A is less than B, false otherwise.
			virtual bool InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const;
			
			/// Determines whether instance A equals to B.
			/// @param a The instance A.
			/// @param b The instance B.
			/// @return true if A equals to B, false otherwise.
			virtual bool InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const;

			/// Assignment operation.
			/// @deprecated This operation is deprecated, all assignments just pass.
			virtual ManagedObject* Copy(VirtualMachine* vm, ManagedObject* a) const = 0;

			/// Checks if this type is convertible from another type.
			/// @param vm The virtual machine.
			/// @param anotherTypeDefinition The other type.
			/// @return true if convertible from the other type, false if not.
			virtual bool IsConvertibleFrom(VirtualMachine* vm, TypeDefinition const* anotherTypeDefinition) const;

			/// Converts an managed object of its type to this type.
			/// @param vm The virtual machine.
			/// @param object The object to be converted. Must be convertible from its type to this type.
			/// @return null The converted object. If the two types are the same, the input object will be
			/// returned without any changes.
			virtual ManagedObject* Convert(VirtualMachine* vm, ManagedObject* object) const;

			virtual void Serialize(IOStream& stream) const;
		};
	}
}