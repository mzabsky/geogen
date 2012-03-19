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

#include "../include/geogen.h"

namespace GeoGen{
    namespace Net{
        private ref class StringUtil abstract sealed{ // "private abstract sealed" = "internal static"
        internal:
            static GGen_String ManagedToUnmanagedString(System::String^ managedString){
                GGen_Char* unmanagedString = new GGen_Char[managedString->Length + 1];

                for(int i = 0; i < managedString->Length; i++){
                    unmanagedString[i] = managedString[i];
                }

                unmanagedString[managedString->Length] = GGen_Const_String('\0');

                GGen_String str(unmanagedString);

                delete [] unmanagedString;

                return str;
            }

            static System::String^ UnmanagedToManagedString(const GGen_String& unmanagedString){
                System::Text::StringBuilder^ managedString = gcnew System::Text::StringBuilder(unmanagedString.length());

                for(unsigned i = 0; i < unmanagedString.length(); i++){
                    managedString->Append(unmanagedString[i]);
                }

                return managedString->ToString();
            }
        };
    }
}