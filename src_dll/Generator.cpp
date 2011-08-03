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

#include "ScriptArg.h"
#include "HeightData.h"
#include "StringUtil.h"
#include "Exception.h"
#include "EventArgs.h"

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

		public enum class StatusMode{
			NoScript,
			ScriptLoaded,
			ReadyToGenerate,
			LoadingMapInfo,
			Generating
		};

		public ref class Generator sealed {
		public:
			event EventHandler<MessageEventArgs^>^ MessageThrown;
			event EventHandler<ProgressEventArgs^>^ ProgressChanged;
			event EventHandler<MapReturnedEventArgs^>^ MapReturned;

		private:
			static Generator^ instance = nullptr;
			GGen* ggen;

			array<ScriptArg^>^ args;

			bool disposed;

			unsigned seed;

		public:
			property System::Collections::Generic::IEnumerable<ScriptArg^>^ Args{
				System::Collections::Generic::IEnumerable<ScriptArg^>^ get(){
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

			property int MaximumMapSize{
				int get(){
					return ggen->GetMaxMapSize();
				}

				void set(int value){
					if(this->ggen->GetStatus() == GGEN_LOADING_MAP_INFO || this->ggen->GetStatus() == GGEN_GENERATING){
						throw gcnew InvalidStatusException();
					}
			
					ggen->SetMaxMapSize(value);
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

        internal:
            static property Generator^ Instance{
                Generator^ get(){
                    return Generator::instance;
                }
            }
        public:
			Generator(){
				if(Generator::instance != nullptr) throw gcnew OneInstanceAllowedException();

				this->disposed = false;

				Generator::instance = this;

				try{
					this->ggen = new GGen_Squirrel;

					this->ggen->SetMessageCallback(&GeoGen::Net::MessageHandler);
					this->ggen->SetProgressCallback(&GeoGen::Net::ProgressHandler);
					this->ggen->SetReturnCallback(&GeoGen::Net::ReturnHandler);
				}
				catch(System::Exception^ e){
					throw gcnew InternalErrorException(e);
				}

				this->MessageThrown += gcnew EventHandler<MessageEventArgs^>(this, &Generator::DefaultMessageHandler);

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

			void SetScript(System::String^ script){
				if(this->ggen->GetStatus() == GGEN_LOADING_MAP_INFO || this->ggen->GetStatus() == GGEN_GENERATING){
					throw gcnew InvalidStatusException();
				}

				bool result;
			
				try{
					result = ggen->SetScript(
						StringUtil::ManagedToUnmanagedString(script)
					);
				}
				catch(ExceptionInCallbackException^){
					throw;
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
					unmanagedInfo = ggen->GetInfo(StringUtil::ManagedToUnmanagedString(label));
				}
				catch(ExceptionInCallbackException^){
					throw;
				}
				catch(System::Exception^ e){
					throw gcnew InternalErrorException(e);
				}

				return StringUtil::UnmanagedToManagedString(unmanagedInfo);
			}

			int GetInfoInt(System::String^ label){
				if(this->ggen->GetStatus() != GGEN_SCRIPT_LOADED && this->ggen->GetStatus() != GGEN_READY_TO_GENERATE){
					throw gcnew InvalidStatusException();
				}

				return ggen->GetInfoInt(StringUtil::ManagedToUnmanagedString(label));
			}

			array<ScriptArg^>^ LoadArgs(){
				if(this->ggen->GetStatus() != GGEN_SCRIPT_LOADED){
					throw gcnew InvalidStatusException();
				}
		
				std::vector<GGen_ScriptArg>* unmanagedArgs;
		
				try{
					unmanagedArgs = ggen->LoadArgs();
				}
				catch(ExceptionInCallbackException^){
					throw;
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
					this->args[i] = ScriptArg::CreateFromNative(&(*unmanagedArgs)[i]);
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
				catch(ExceptionInCallbackException^){
					throw;
				}
				catch(System::Exception^ e){
					throw gcnew InternalErrorException(e);
				}

				if(pureData == NULL) {
					throw gcnew GenerationFailedException;
					return nullptr;
				}

				HeightData^ heightData = HeightData::CreateFromNative(ggen->output_width, ggen->output_height, pureData);

				GGen_DeleteNativeArrayPtr(pureData);

				return heightData;
			}

            void Reset(){
                this->ggen->Reset();
            }

		internal:
			void MessageHandler(const GGen_String& message, GGen_Message_Level level, int line, int column){
				try{
					this->MessageThrown(this, %MessageEventArgs(StringUtil::UnmanagedToManagedString(message), (MessageLevel) level, line, column));
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
					this->MapReturned(this, %MapReturnedEventArgs(StringUtil::UnmanagedToManagedString(name), HeightData::CreateFromNative(width, height, map)));
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
			Generator::Instance->MessageHandler(message, level, line, column);
		};

		void ProgressHandler(int current_progress, int max_progress){
			Generator::Instance->ProgressHandler(current_progress, max_progress);
		}

		void ReturnHandler(const GGen_String& name, const short* map, int width, int height){
			Generator::Instance->ReturnHandler(name, map, width, height);
		}
	}
}
