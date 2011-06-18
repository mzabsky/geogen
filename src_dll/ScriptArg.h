/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

#pragma once

#include <string>

#include "StringUtil.h"

namespace GeoGen{
    namespace Net{
        public enum class ScriptArgType{
            Bool,
            Int,
            Enum
        };

        public ref class ScriptArg sealed {
        private:
            ScriptArgType type;
            System::String^ name, ^label, ^description;
            unsigned default, minimum, maximum;
            unsigned value;
            array<System::String^>^ options;
        public:
            property ScriptArgType Type{
                ScriptArgType get(){
                    return type;
                }
            }

            property System::String^ Name{
                System::String^ get(){
                    return name;
                }
            }

            property System::String^ Label{
                System::String^ get(){
                    return label;
                }
            }

            property System::String^ Description{
                System::String^ get(){
                    return description;
                }
            }

            property unsigned Default{
                unsigned get(){
                    return default;
                }
            }

            property unsigned Maximum{
                unsigned get(){
                    return maximum;
                }
            }

            property unsigned Minimum{
                unsigned get(){
                    return minimum;
                }
            }

            property unsigned Value{
                unsigned get(){
                    return value;
                }

                void set(unsigned value){
                    if((type == ScriptArgType::Enum && value >= (unsigned) options->Length) || value < minimum) {
                        this->value = 0; 
                        return;	
                    }
                    else if(value > maximum){
                        this->value = maximum;
                        return;
                    }

                    this->value = value;
                }
            }

            property System::Collections::Generic::IEnumerable<System::String^>^ Options{
                System::Collections::Generic::IEnumerable<System::String^>^ get(){
                    return options;
                }
            }

            property System::String^ SelectedOption{
                System::String^ get(){
                    return this->type == ScriptArgType::Enum ? options[Value] : "";
                }
            }

            ScriptArg(){}
        internal:			
            static ScriptArg^ CreateFromNative(GGen_ScriptArg* unmanagedArg){
                ScriptArg^ newObject = gcnew ScriptArg();
                
                switch(unmanagedArg->type){
                case GGEN_BOOL: 
                    newObject->type = ScriptArgType::Bool; 
                    break;
                case GGEN_INT: 
                    newObject->type = ScriptArgType::Int; 
                    break;
                case GGEN_ENUM: 
                    newObject->type = ScriptArgType::Enum; 
                    break;
                default:
                    throw gcnew System::InvalidOperationException("Invalid ScriptArg type.");
                }

                newObject->name = StringUtil::UnmanagedToManagedString(unmanagedArg->name);
                newObject->label = StringUtil::UnmanagedToManagedString(unmanagedArg->label);
                newObject->description = StringUtil::UnmanagedToManagedString(unmanagedArg->description);

                newObject->default = unmanagedArg->default_value;
                newObject->maximum = unmanagedArg->max_value;
                newObject->minimum = unmanagedArg->min_value;
                newObject->value = unmanagedArg->default_value;

                newObject->options = gcnew array<System::String^>(unmanagedArg->options.size());

                for(unsigned i = 0; i < unmanagedArg->options.size(); i++){
                    newObject->options[i] = StringUtil::UnmanagedToManagedString(unmanagedArg->options[i]);
                }

                return newObject;
            }
        };
    }
}