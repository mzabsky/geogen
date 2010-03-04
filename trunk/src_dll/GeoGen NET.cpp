using namespace System;

#ifdef _DEBUG
#pragma comment(lib,"../bin/GeoGenD.lib")
#else
#pragma comment(lib,"../bin/GeoGen.lib")
#endif


#include "../include/geogen.h"

void MessageHandler(const GGen_String& message, GGen_Message_Level level, int line, int column);
void ProgressHandler(int current_progress, int max_progress);
void ReturnHandler(const GGen_String& name, const short* map, int width, int height);

public ref class GGenNet{
public:
	enum class MessageLevel{
		Message = 0,
		Notice = 1,
		Warning = 2,
		Error = 3
	};

	enum class ScriptArgType{
		Bool,
		Int,
		Enum
	};

	ref class HeightData{
	private:
		unsigned short width;
		unsigned short height;
		unsigned length;

		array<short>^ data;
	public:
		property unsigned short Width{
			unsigned short get(){
				return width;
			}
		}

		property unsigned short Height{
			unsigned short get(){
				return height;
			}
		}

		property unsigned Length{
			unsigned get(){
				return length;
			}
		}

		property array<short>^ Data{
			array<short>^ get(){
				return data;
			}
		}

		HeightData(unsigned short width, unsigned short height, array<short>^ data){
			this->width = width;
			this->height = height;
			this->length = width * height;

			this->data = array<short>(data);
		}

	internal:
		HeightData(unsigned short width, unsigned short height, const short* data){
			this->width = width;
			this->height = height;
			this->length = width * height;
			
			this->data = gcnew array<short>(this->length);

			for(unsigned i = 0; i < Length; i++){
				this->data[i] = data[i];
			}
		}
	};

	ref class ScriptArg{
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
		
		property String^ Name{
			String^ get(){
				return name;
			}
		}

		property String^ Label{
			String^ get(){
				return label;
			}
		}

		property String^ Description{
			String^ get(){
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

		property array<System::String^>^ Options{
			array<System::String^>^ get(){
				return options;
			}
		}

		property System::String^ SelectedOption{
			System::String^ get(){
				return this->type == ScriptArgType::Enum ? Options[Value] : "";
			}
		}
	internal:
		ScriptArg(GGen_ScriptArg* unmanagedArg){
			switch(unmanagedArg->type){
				case GGEN_BOOL: this->type = ScriptArgType::Bool; break;
				case GGEN_INT: this->type = ScriptArgType::Int; break;
				case GGEN_ENUM: this->type = ScriptArgType::Enum; break;
			}

			name = GGenNet::UnmanagedToManagedString(unmanagedArg->name);
			label = GGenNet::UnmanagedToManagedString(unmanagedArg->label);
			description = GGenNet::UnmanagedToManagedString(unmanagedArg->description);

			default = unmanagedArg->default_value;
			maximum = unmanagedArg->max_value;
			minimum = unmanagedArg->min_value;
			value = unmanagedArg->default_value;

			options = gcnew array<System::String^>(unmanagedArg->options.size());

			for(unsigned i = 0; i < unmanagedArg->options.size(); i++){
				options[i] = GGenNet::UnmanagedToManagedString(unmanagedArg->options[i]);
			}
		}
	};

	ref class Exception: System::Exception{
		/*Exception(System::String^ message){
			((System::Exception^) this)
		};*/
	};

	ref class SyntaxErrorException: Exception{
		/*SyntaxErrorException()
			: base("Squirrel compiler could not compile inserted script, please see recent messages returned by the MessageThrown event for details."){}
		*/
	};

	ref class ArgsUnreadableException: Exception{
		/*ArgsUnreadableException(){
			this->Message = "Map argument definitions could not be loaded, the GetInfo function is either missing in the script, or it causes errors (such as undefined symbols or argument mismatches). Please see recent messages returned by the MessageThrown event for details.";
		}*/
	};

	ref class ArgsNotLoadedException: Exception{
		/*ArgsNotLoadedException(){
			this->Message = "LoadArgs must be called before the Generate function call.";
		}*/
	};

	ref class GenerationFailedException: Exception{
		/*GenerationFailedException(){
			this->Message = "Map generation failed. The Generate function is either missing in the script, or it causes errors (such as undefined symbols or argument mismatches), or the generator ran out of memory. Please see recent messages returned by the MessageThrown event for details.";
		}*/
	};

	ref class OneInstanceAllowedException: Exception{
		/*OneInstanceAllowedException(){
			this->Message = "Only one GGenNet instance can exist in a program at one time. Free the current instance or use the GetInstance method to refer to it from any place in the program.";
		}*/
	};

	ref class InternalErrorException: Exception{
		/*InternalErrorException(System::Exception^ inner){
			this->InnerException = inner;
			this->Message = "GeoGen native library has unexpectedly crashed, please see InnerException object for details.";
		}*/
	};

	ref class MessageEventArgs: System::EventArgs{
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

	ref class ProgressEventArgs: System::EventArgs{
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

	ref class MapReturnedEventArgs: System::EventArgs{
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

	delegate void MessageEventHandler(System::Object^ sender, MessageEventArgs^ e);
	delegate void ProgressEventHandler(System::Object^ sender, ProgressEventArgs^ e);
	delegate void MapReturnedEventHandler(System::Object^ sender, MapReturnedEventArgs^ e);

	event MessageEventHandler^ MessageThrown;
	event ProgressEventHandler^ ProgressChanged;
	event MapReturnedEventHandler^ MapReturned;

private:
	static GGenNet^ instance = nullptr;
	GGen* ggen;

	array<ScriptArg^>^ args;

public:
	property array<ScriptArg^>^ Args{
		array<ScriptArg^>^ get(){
			return this->args;
		}
	}

	property int MaximumMapWidth{
		int get(){
			return ggen->GetMaxWidth();
		}

		void set(int value){
			ggen->SetMaxWidth(value);
		}
	}

	property int MaximumMapHeight{
		int get(){
			return ggen->GetMaxHeight();
		}

		void set(int value){
			ggen->SetMaxHeight(value);
		}
	}

	property int MaximumMapCount{
		int get(){
			return ggen->GetMaxMapCount();
		}

		void set(int value){
			ggen->SetMaxMapCount(value);
		}
	}

	GGenNet(){
		if(GGenNet::instance != nullptr) throw gcnew OneInstanceAllowedException();

		GGenNet::instance = this;

		try{
			this->ggen = new GGen_Squirrel;
			
			if(this->ggen == NULL) throw gcnew System::AccessViolationException("Could not create GGen class object.");
		
			this->ggen->SetMessageCallback(&::MessageHandler);
			this->ggen->SetProgressCallback(&::ProgressHandler);
			this->ggen->SetReturnCallback(&::ReturnHandler);
		}
		catch(System::Exception^){
			throw gcnew InternalErrorException;
		}

		this->MessageThrown += gcnew MessageEventHandler(this, &GGenNet::DefaultMessageHandler);

		this->args = nullptr;
	}

	~GGenNet(){
		delete this->ggen;

		this->ggen = NULL;
		
		this->instance = nullptr;
	}

	static GGenNet^ GetInstance(){
		return instance;
	}

	void SetScript(System::String^ script){
		bool result;
			
		try{
			result = ggen->SetScript(
				this->ManagedToUnmanagedString(script)
			);
		}
		catch(System::Exception^){
			throw gcnew InternalErrorException;
		}
		
		if(!result){
			throw gcnew SyntaxErrorException();
		}

		this->args = nullptr;
	}

	System::String^ GetInfo(System::String^ label){
		GGen_String unmanagedInfo;
		
		try{
			unmanagedInfo = ggen->GetInfo(ManagedToUnmanagedString(label));
		}
		catch(System::Exception^){
			throw gcnew InternalErrorException;
		}

		return UnmanagedToManagedString(unmanagedInfo);
	}

	int GetInfoInt(System::String^ label){
		return ggen->GetInfoInt(ManagedToUnmanagedString(label));
	}

	array<ScriptArg^>^ LoadArgs(){
		std::vector<GGen_ScriptArg>* unmanagedArgs;
		
		try{
			unmanagedArgs = ggen->LoadArgs();
		}
		catch(System::Exception^){
			throw gcnew InternalErrorException;
		}

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
		if(this->args == nullptr) {
			throw gcnew ArgsNotLoadedException;
			return nullptr;
		}
		
		short* pureData;
		
		try{
			pureData = ggen->Generate();
		}
		catch(System::Exception^){
			throw gcnew InternalErrorException;
		}

		if(pureData == NULL) {
			throw gcnew GenerationFailedException;
			return nullptr;
		}

		return gcnew HeightData(ggen->output_width, ggen->output_height, pureData);
	}

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


	void MessageHandler(const GGen_String& message, GGen_Message_Level level, int line, int column){
		this->MessageThrown(this, %MessageEventArgs(GGenNet::UnmanagedToManagedString(message), (GGenNet::MessageLevel) level, line, column));
	}

	void ProgressHandler(int currentProgress, int maxProgress){
		this->ProgressChanged(this, %ProgressEventArgs(currentProgress, maxProgress));
	}

	void ReturnHandler(const GGen_String& name, const short* map, int width, int height){
		this->MapReturned(this, %MapReturnedEventArgs(UnmanagedToManagedString(name), %HeightData(width, height, map)));
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

void MessageHandler(const GGen_String& message, GGen_Message_Level level, int line, int column){
	GGenNet::GetInstance()->MessageHandler(message, level, line, column);
};

void ProgressHandler(int current_progress, int max_progress){
	GGenNet::GetInstance()->ProgressHandler(current_progress, max_progress);
}

void ReturnHandler(const GGen_String& name, const short* map, int width, int height){
	GGenNet::GetInstance()->ReturnHandler(name, map, width, height);
}

