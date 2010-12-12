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

#include <math.h>

using namespace System;

#ifdef _DEBUG
#pragma comment(lib,"../bin/GeoGenD.lib")
#else
#pragma comment(lib,"../bin/GeoGen.lib")
#endif


#include "../include/geogen.h"

namespace GeoGen{
	namespace Net{
		// Native callback handlers
		void MessageHandler(const GGen_String& message, GGen_Message_Level level, int line, int column);
		void ProgressHandler(int current_progress, int max_progress);
		void ReturnHandler(const GGen_String& name, const short* map, int width, int height);

		// String conversion helpers
		GGen_String ManagedToUnmanagedString(System::String^ managedString);
		System::String^ UnmanagedToManagedString(const GGen_String& unmanagedString);

		public enum class MessageLevel{
			Message = 0,
			Notice = 1,
			Warning = 2,
			Error = 3
		};

		public enum class ScriptArgType{
			Bool,
			Int,
			Enum
		};

		public enum class StatusMode{
			NoScript,
			ScriptLoaded,
			ReadyToGenerate,
			LoadingMapInfo,
			Generating
		};

		public ref class HeightData{
		protected:
			int width;
			int height;
			System::Int64 length;

			short* data;
		public:
			property int Width{
				int get(){
					return width;
				}
			}

			property int Height{
				int get(){
					return height;
				}
			}

			property System::Int64 Length{
				System::Int64 get(){
					return length;
				}
			}

			property short default[int]{
				short get(int index){
					if(index < this->length && index >= 0) return data[index];
					else throw gcnew IndexOutOfRangeException;
				}

				void set(int index, short value){
					if(index < this->length && index >= 0) data[index] = value;
					else throw gcnew IndexOutOfRangeException;
				}
			}

			property short default[int, int]{
				short get(int x, int y){
					if(x < this->width && y < this->height && x >= 0 && y >= 0) return data[x + this->width * y];
					else throw gcnew IndexOutOfRangeException;
				}

				void set(int x, int y, short value){
					if(x < this->width && y < this->height && x >= 0 && y >= 0) data[x + this->width * y] = value;
					else throw gcnew IndexOutOfRangeException;
				}
			}

			property System::IntPtr DataPtr{
				System::IntPtr get(){
					return (System::IntPtr) data;
				}
			}

			HeightData(int width, int height){
				if(width < 3 || height < 3) throw gcnew ArgumentException("Both height and with must be >= 2.");
				if(width < GGen::GetInstance()->GetMaxWidth() || height < GGen::GetInstance()->GetMaxHeight()) throw gcnew ArgumentException("Both height and with must be >= 2.");

				this->width = width;
				this->height = height;
				this->length = width * height;
			
				this->data = new short[(unsigned int) this->length];

				for(unsigned i = 0; i < this->length; i++){
					this->data[i] = 0;
				}
			}

			HeightData(const HeightData% victim){
				this->width = victim.width;
				this->height = victim.height;
				this->length = this->width * this->height;

				this->data = new short[(unsigned int) this->length];

				memcpy(this->data, victim.data, (unsigned int) this->length * sizeof(short));
			}

			~HeightData(){
				delete [] this->data;
			}

		internal:
			HeightData(unsigned short width, unsigned short height, const short* data){
				this->width = width;
				this->height = height;
				this->length = width * height;
			
				this->data = new short[(unsigned int) this->length];
			
				try{
					memcpy(this->data, data, (unsigned int) this->length * sizeof(short));
				}
				catch(System::Exception^){
					throw gcnew OutOfMemoryException;
				}
			}
		};

		public ref class ScriptArg{
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
			protected: 
				void set(ScriptArgType value){
					type = value;
				}
			}

			property String^ Name{
				String^ get(){
					return name;
				}
			protected: 
				void set(String^ value){
					name = value;
				}
			}

			property String^ Label{
				String^ get(){
					return label;
				}
			protected: 
				void set(String^ value){
					label = value;
				}
			}

			property String^ Description{
				String^ get(){
					return description;
				}
			protected: 
				void set(String^ value){
					description = value;
				}
			}

			property unsigned Default{
				unsigned get(){
					return default;
				}
			protected: 
				void set(unsigned value){
					default = value;
				}
			}

			property unsigned Maximum{
				unsigned get(){
					return maximum;
				}
			protected: 
				void set(unsigned value){
					maximum = value;
				}
			}

			property unsigned Minimum{
				unsigned get(){
					return minimum;
				}
			protected: 
				void set(unsigned value){
					minimum = value;
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

			property array<System::String^>^ Options{
				array<System::String^>^ get(){
					return options;
				}
			protected: 
				void set(array<System::String^>^ value){
					options = value;
				}
			}

			property System::String^ SelectedOption{
				System::String^ get(){
					return this->type == ScriptArgType::Enum ? Options[Value] : "";
				}
			}

			ScriptArg(){}
		internal:			

			ScriptArg(GGen_ScriptArg* unmanagedArg){
				switch(unmanagedArg->type){
				case GGEN_BOOL: this->type = ScriptArgType::Bool; break;
				case GGEN_INT: this->type = ScriptArgType::Int; break;
				case GGEN_ENUM: this->type = ScriptArgType::Enum; break;
				}

				name = UnmanagedToManagedString(unmanagedArg->name);
				label = UnmanagedToManagedString(unmanagedArg->label);
				description = UnmanagedToManagedString(unmanagedArg->description);

				default = unmanagedArg->default_value;
					maximum = unmanagedArg->max_value;
					minimum = unmanagedArg->min_value;
					value = unmanagedArg->default_value;

					options = gcnew array<System::String^>(unmanagedArg->options.size());

					for(unsigned i = 0; i < unmanagedArg->options.size(); i++){
						options[i] = UnmanagedToManagedString(unmanagedArg->options[i]);
					}
			}
		};

		public ref class Exception: System::Exception{
		public:	
			Exception(System::String^ message, System::Exception^ innerException)
				: System::Exception(message, innerException){};
		};

		public ref class SyntaxErrorException: Exception{
		public:	
			SyntaxErrorException()
				: Exception("Squirrel compiler could not compile inserted script, please see recent messages returned by the MessageThrown event for details.", nullptr){};
		};

		public ref class ArgsUnreadableException: Exception{
		public:		
			ArgsUnreadableException()
				: Exception("Map argument definitions could not be loaded, the GetInfo function is either missing in the script, or it causes errors (such as undefined symbols or argument mismatches). Please see recent messages returned by the MessageThrown event for details.", nullptr){};
		};

		public ref class ArgsNotLoadedException: Exception{
		public:		
			ArgsNotLoadedException()
				: Exception("LoadArgs method must be called before the Generate function call.", nullptr){};
		};

		public ref class GenerationFailedException: Exception{
		public:		
			GenerationFailedException()
				: Exception("Map generation failed. The Generate function is either missing in the script, or it causes errors (such as undefined symbols or argument mismatches), or the generator ran out of memory. Please see recent messages returned by the MessageThrown event for details.", nullptr){};
		};

		public ref class OneInstanceAllowedException: Exception{
		public:		
			OneInstanceAllowedException()
				: Exception("Only one GGenNet instance can exist in a program at one time. Free the current instance or use the GetInstance method to refer to it from any place in the program.", nullptr){};
		};

		public ref class InternalErrorException: Exception{
		public:	
			InternalErrorException(System::Exception^ innerException)
				: Exception("GeoGen native library has unexpectedly crashed, please see InnerException object for more details.", innerException){};
		};

		public ref class ExceptionInCallbackException: Exception{
		public:	
			ExceptionInCallbackException(System::Exception^ innerException)
				: Exception("An unhandled exception has been thrown by assigned event handler, please see InnerException object for more details.", innerException){};
		};

		public ref class ArgumentMismatchException: Exception{
		public:	
			ArgumentMismatchException()
				: Exception("Argument list provided by GGenNet does not match internal argument list. Argument count and types mist not change between LoadArgs and Generate method calls.", nullptr){};
		};

		public ref class InvalidStatusException: Exception{
		public:	
			InvalidStatusException()
				: Exception("The generator is incorect status for calling this method. Make sure the method is called in correct context (the core methods must be called in correct order, generator methods and properties must not be referenced from within of GGenNet event handlers).", nullptr){};
		};

		public ref class MessageEventArgs: System::EventArgs{
		private:
			System::String^ message;
			MessageLevel level;
			int line;
			int column;
		public:
			property System::String^ Message{
				System::String^ get(){
					return message;
				}
			}

			property MessageLevel Level{
				MessageLevel get(){
					return level;
				}
			}

			property int Line{
				int get(){
					return line;
				}
			}

			property int Column{
				int get(){
					return column;
				}
			}

			MessageEventArgs(System::String^ message, MessageLevel level, int line, int column):message(message),level(level),line(line),column(column){}
		};

		public ref class ProgressEventArgs: System::EventArgs{
		private:
			int currentProgress;
			int maximumProgress;
		public:
			property int CurrentProgress{
				int get(){
					return currentProgress;
				}
			}

			property int MaximumProgress{
				int get(){
					return maximumProgress;
				}
			}

			ProgressEventArgs(int currentProgress, int maximumProgress):currentProgress(currentProgress),maximumProgress(maximumProgress){}
		};

		public ref class MapReturnedEventArgs: System::EventArgs{
		private:
			System::String^ label;
			HeightData^ heightMap;
		public:
			property System::String^ Label{
				System::String^ get(){
					return label;
				}
			}

			property HeightData^ HeightMap{
				HeightData^ get(){
					return heightMap;
				}
			}

			MapReturnedEventArgs(System::String^ label, HeightData^ heightMap): label(label), heightMap(heightMap){}
		};

		public ref class Generator{
		public:
			delegate void MessageEventHandler(System::Object^ sender, MessageEventArgs^ e);
			delegate void ProgressEventHandler(System::Object^ sender, ProgressEventArgs^ e);
			delegate void MapReturnedEventHandler(System::Object^ sender, MapReturnedEventArgs^ e);

			event MessageEventHandler^ MessageThrown;
			event ProgressEventHandler^ ProgressChanged;
			event MapReturnedEventHandler^ MapReturned;

		private:
			static Generator^ instance = nullptr;
			GGen* ggen;

			array<ScriptArg^>^ args;

			bool disposed;

			unsigned seed;

		public:
			property array<ScriptArg^>^ Args{
				array<ScriptArg^>^ get(){
					return this->args;
				}
			}

			property StatusMode Status{
				StatusMode get(){
					switch(ggen->GetStatus()){
						case GGEN_NO_SCRIPT: return StatusMode::NoScript;
						case GGEN_SCRIPT_LOADED: return StatusMode::ScriptLoaded;
						case GGEN_READY_TO_GENERATE: return StatusMode::ReadyToGenerate;
						case GGEN_LOADING_MAP_INFO: return StatusMode::LoadingMapInfo;
						case GGEN_GENERATING: return StatusMode::Generating;
					}
			
					return StatusMode::NoScript;
				}
			}

			property int MaximumMapWidth{
				int get(){
					return ggen->GetMaxWidth();
				}

				void set(int value){
					if(this->ggen->GetStatus() == GGEN_LOADING_MAP_INFO || this->ggen->GetStatus() == GGEN_GENERATING){
						throw gcnew InvalidStatusException();
					}
			
					ggen->SetMaxWidth(value);
				}
			}

			property int MaximumMapHeight{
				int get(){
					return ggen->GetMaxHeight();
				}

				void set(int value){
					if(this->ggen->GetStatus() == GGEN_LOADING_MAP_INFO || this->ggen->GetStatus() == GGEN_GENERATING){
						throw gcnew InvalidStatusException();
					}
			
					ggen->SetMaxHeight(value);
				}
			}

			property int MaximumMapCount{
				int get(){
					return ggen->GetMaxMapCount();
				}

				void set(int value){
					if(this->ggen->GetStatus() == GGEN_LOADING_MAP_INFO || this->ggen->GetStatus() == GGEN_GENERATING){
						throw gcnew InvalidStatusException();
					}
			
					ggen->SetMaxMapCount(value);
				}
			}

			property unsigned Seed{
				unsigned get(){
					return this->seed;
				}

				void set(unsigned seed){
					if(this->ggen->GetStatus() == GGEN_LOADING_MAP_INFO || this->ggen->GetStatus() == GGEN_GENERATING){
						throw gcnew InvalidStatusException();
					}

					this->ggen->SetSeed(seed);
					this->seed = seed;
				}
			}

			Generator(){
				if(Generator::instance != nullptr) throw gcnew OneInstanceAllowedException();

				this->disposed = false;

				Generator::instance = this;

				try{
					this->ggen = new GGen_Squirrel;

					if(this->ggen == NULL) throw gcnew System::AccessViolationException("Could not create GGen class object.");
		
					this->ggen->SetMessageCallback(&GeoGen::Net::MessageHandler);
					this->ggen->SetProgressCallback(&GeoGen::Net::ProgressHandler);
					this->ggen->SetReturnCallback(&GeoGen::Net::ReturnHandler);
				}
				catch(System::Exception^ e){
					throw gcnew InternalErrorException(e);
				}

				this->MessageThrown += gcnew MessageEventHandler(this, &Generator::DefaultMessageHandler);

				this->args = nullptr;
			}

			~Generator(){
				if(!disposed){
					delete this->ggen;

					this->ggen = NULL;
			
					this->instance = nullptr;

					disposed = true;
				}
			}

			static Generator^ GetInstance(){
				return instance;
			}

			void SetScript(System::String^ script){
				if(this->ggen->GetStatus() == GGEN_LOADING_MAP_INFO || this->ggen->GetStatus() == GGEN_GENERATING){
					throw gcnew InvalidStatusException();
				}

				bool result;
			
				try{
					result = ggen->SetScript(
						ManagedToUnmanagedString(script)
					);
				}
				catch(ExceptionInCallbackException^ e){
					throw e;
				}
				catch(System::Exception^ e){
					throw gcnew InternalErrorException(e);
				}
		
				if(!result){
					throw gcnew SyntaxErrorException();
				}

				this->args = nullptr;
			}

			System::String^ GetInfo(System::String^ label){
				if(this->ggen->GetStatus() != GGEN_SCRIPT_LOADED && this->ggen->GetStatus() != GGEN_READY_TO_GENERATE){
					throw gcnew InvalidStatusException();
				}

				GGen_String unmanagedInfo;
		
				try{
					unmanagedInfo = ggen->GetInfo(ManagedToUnmanagedString(label));
				}
				catch(ExceptionInCallbackException^ e){
					throw e;
				}
				catch(System::Exception^ e){
					throw gcnew InternalErrorException(e);
				}

				return UnmanagedToManagedString(unmanagedInfo);
			}

			int GetInfoInt(System::String^ label){
				if(this->ggen->GetStatus() != GGEN_SCRIPT_LOADED && this->ggen->GetStatus() != GGEN_READY_TO_GENERATE){
					throw gcnew InvalidStatusException();
				}

				return ggen->GetInfoInt(ManagedToUnmanagedString(label));
			}

			array<ScriptArg^>^ LoadArgs(){
				if(this->ggen->GetStatus() != GGEN_SCRIPT_LOADED){
					throw gcnew InvalidStatusException();
				}
		
				std::vector<GGen_ScriptArg>* unmanagedArgs;
		
				try{
					unmanagedArgs = ggen->LoadArgs();
				}
				catch(ExceptionInCallbackException^ e){
					throw e;
				}
				catch(System::Exception^ e){
					throw gcnew InternalErrorException(e);
				};

				if(unmanagedArgs == NULL){
					throw gcnew ArgsUnreadableException;
					return nullptr;
				}

				this->args = gcnew array<ScriptArg^>(unmanagedArgs->size());

				for(unsigned i = 0; i < unmanagedArgs->size(); i++){
					this->args[i] = gcnew ScriptArg(&(*unmanagedArgs)[i]);
				}

				return this->args;
			}

			HeightData^ Generate(){
				if(this->ggen->GetStatus() != GGEN_READY_TO_GENERATE){
					throw gcnew InvalidStatusException();
				}

				if(this->args == nullptr) {
					throw gcnew ArgsNotLoadedException;
					return nullptr;
				}

				if(this->args->Length != ggen->args.size()){
					throw gcnew ArgumentMismatchException();
				}

				for(int i = 0; i < this->args->Length; i++){
					if(
						(this->args[i]->Type == ScriptArgType::Bool && ggen->args[i].type != GGEN_BOOL) ||
						(this->args[i]->Type == ScriptArgType::Int && ggen->args[i].type != GGEN_INT) ||
						(this->args[i]->Type == ScriptArgType::Enum && ggen->args[i].type != GGEN_ENUM)
					){
						throw gcnew ArgumentMismatchException();
					}

					ggen->args[i].value = this->args[i]->Value;
				}
		
				short* pureData;
		
				try{
					pureData = ggen->Generate();
				}
				catch(ExceptionInCallbackException^ e){
					throw e;
				}
				catch(System::Exception^ e){
					throw gcnew InternalErrorException(e);
				}

				if(pureData == NULL) {
					throw gcnew GenerationFailedException;
					return nullptr;
				}

				HeightData^ heightData = gcnew HeightData(ggen->output_width, ggen->output_height, pureData);

				GGen_DeleteNativeArrayPtr(pureData);

				return heightData;
			}

		internal:
			void MessageHandler(const GGen_String& message, GGen_Message_Level level, int line, int column){
				try{
					this->MessageThrown(this, %MessageEventArgs(UnmanagedToManagedString(message), (MessageLevel) level, line, column));
				}
				catch(System::Exception^ e){
					throw gcnew ExceptionInCallbackException(e);
				}
			}

			void ProgressHandler(int currentProgress, int maxProgress){
				try{
					this->ProgressChanged(this, %ProgressEventArgs(currentProgress, maxProgress));
				}
				catch(System::Exception^ e){
					throw gcnew ExceptionInCallbackException(e);
				}
			}

			void ReturnHandler(const GGen_String& name, const short* map, int width, int height){
				try{
					this->MapReturned(this, %MapReturnedEventArgs(UnmanagedToManagedString(name), gcnew HeightData(width, height, map)));
				}
				catch(System::Exception^ e){
					throw gcnew ExceptionInCallbackException(e);
				}
			}

		public:
			void DefaultMessageHandler(System::Object^ sender, MessageEventArgs^ e){
				System::String^ level;

				switch(e->Level){
					case MessageLevel::Error: level = "Error"; break;
					case MessageLevel::Warning: level = "Warning"; break;
					case MessageLevel::Notice: level = "Notice"; break;
					case MessageLevel::Message: level = "Message"; break;
				}

				Console::WriteLine("GGen " + level + ": " + e->Message + (e->Line != -1 ? " on line " + e->Line : ""));
			}
		};

		// Native callback handlers
		void MessageHandler(const GGen_String& message, GGen_Message_Level level, int line, int column){
			Generator::GetInstance()->MessageHandler(message, level, line, column);
		};

		void ProgressHandler(int current_progress, int max_progress){
			Generator::GetInstance()->ProgressHandler(current_progress, max_progress);
		}

		void ReturnHandler(const GGen_String& name, const short* map, int width, int height){
			Generator::GetInstance()->ReturnHandler(name, map, width, height);
		}

		// string conversion helpers
		GGen_String ManagedToUnmanagedString(System::String^ managedString){
			GGen_Char* unmanagedString = new GGen_Char[managedString->Length + 1];

			for(int i = 0; i < managedString->Length; i++){
				unmanagedString[i] = managedString[i];
			}

			unmanagedString[managedString->Length] = GGen_Const_String('\0');

			GGen_String str(unmanagedString);

			delete [] unmanagedString;

			return str;
		}

		System::String^ UnmanagedToManagedString(const GGen_String& unmanagedString){
			System::Text::StringBuilder^ managedString = gcnew System::Text::StringBuilder(unmanagedString.length());

			for(unsigned i = 0; i < unmanagedString.length(); i++){
				managedString->Append(unmanagedString[i]);
			}

			return managedString->ToString();
		}
	}
}
