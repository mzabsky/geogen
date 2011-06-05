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

using namespace System;

namespace GeoGen{
    namespace Net{
        [Serializable]
        public ref class SyntaxErrorException sealed: System::Exception{
        public:	
            SyntaxErrorException()
                : System::Exception("Squirrel compiler could not compile inserted script, please see recent messages returned by the MessageThrown event for details.", nullptr){};
        };

        [Serializable]
        public ref class ArgsUnreadableException sealed: System::Exception{
        public:		
            ArgsUnreadableException()
                : System::Exception("Map argument definitions could not be loaded, the GetInfo function is either missing in the script, or it causes errors (such as undefined symbols or argument mismatches). Please see recent messages returned by the MessageThrown event for details.", nullptr){};
        };

        [Serializable]
        public ref class ArgsNotLoadedException sealed: System::Exception{
        public:		
            ArgsNotLoadedException()
                : System::Exception("LoadArgs method must be called before the Generate function call.", nullptr){};
        };

        [Serializable]
        public ref class GenerationFailedException sealed: System::Exception{
        public:		
            GenerationFailedException()
                : System::Exception("Map generation failed. The Generate function is either missing in the script, or it causes errors (such as undefined symbols or argument mismatches), or the generator ran out of memory. Please see recent messages returned by the MessageThrown event for details.", nullptr){};
        };

        [Serializable]
        public ref class OneInstanceAllowedException sealed: System::Exception{
        public:		
            OneInstanceAllowedException()
                : System::Exception("Only one Generator instance can exist in a program at one time. Free the current instance or use the GetInstance method to refer to it from any place in the program.", nullptr){};
        };

        [Serializable]
        public ref class InternalErrorException sealed: System::Exception{
        public:	
            InternalErrorException(System::Exception^ innerException)
                : System::Exception("GeoGen native library has unexpectedly crashed, please see InnerException object for more details.", innerException){};
        };

        [Serializable]
        public ref class ExceptionInCallbackException sealed: System::Exception{
        public:	
            ExceptionInCallbackException(System::Exception^ innerException)
                : System::Exception("An unhandled exception has been thrown by assigned event handler, please see InnerException object for more details.", innerException){};
        };

        [Serializable]
        public ref class ArgumentMismatchException sealed: System::Exception{
        public:	
            ArgumentMismatchException()
                : System::Exception("Argument list provided by Generator does not match internal argument list. Argument count and types mist not change between LoadArgs and Generate method calls.", nullptr){};
        };

        [Serializable]
        public ref class InvalidStatusException sealed: System::Exception{
        public:	
            InvalidStatusException()
                : System::Exception("The generator is incorect status for calling this method. Make sure the method is called in correct context (the core methods must be called in correct order, generator methods and properties must not be referenced from within of Generator event handlers).", nullptr){};
        };
    }
}