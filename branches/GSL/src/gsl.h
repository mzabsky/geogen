#include <vector>
#include <map>
#include <stack>

#include <windows.h>

#include "ggen_support.h"
#include "ggen.h"
#include "ggen_data_2d.h"
#include "ggen_amplitudes.h"

namespace GeoGen{
	namespace GSL {
		class Type;

		class Type {
			String name;
			static map<String,Type*> typesByName;

			Type(String name){
				this->name = name;				
				Type::typesByName.insert(pair<String, Type*>(this->GetName(), this));
			}
		public:			
			virtual String GetName() {
				return this->name;
			}

			static Type* GetType(String type){
				if (Type::typesByName.count(type) > 0){
					return Type::typesByName[type];
				}
				else {
					return Type::typesByName[type] = new Type(type);
				}
			}
		};

		map<String, Type*> Type::typesByName = map<String, Type*>();

		class Instance {
			Type* type;
			void* value;
		public:
			Instance(Type* type, void* value){
				this->type = type;
				this->value = value;
			}

			void* GetValue() {
				return this->value;
			}

			static Instance* Void() {
				return new Instance(Type::GetType("void"), NULL);
			}
		};

		class Method {
		public:
			virtual String GetName() = 0;
			virtual Type* GetReturnType() = 0;
			virtual Instance* Invoke(vector<Instance*> args) = 0;
		};

		class Node {

		};

		struct State;

		struct State {
			vector<State*> arguments;
			virtual Instance* Invoke(vector<Instance*> argumentInstances) = 0;
		};

		struct ConstantState: public State {
			Instance* value;

			ConstantState() {}
			ConstantState(Instance* instance) { this->value = instance;}

			virtual Instance* Invoke(vector<Instance*>){
				return this->value;
			}
		};

		struct MethodState: public State {
			Method* method;

			virtual Instance* Invoke(vector<Instance*> argumentInstances){
				return method->Invoke(argumentInstances);
			}
		};


		class Data2D_Method: public Method{
			virtual String GetName() {return "Data_2D";}
			virtual Type* GetReturnType() {return Type::GetType("Data_2D");}
			virtual Instance* Invoke(vector<Instance*> args){
				void* value = new Data_2D(
					*((int*) (args[0]->GetValue())),
					*((int*) (args[1]->GetValue())),
					*((int*) (args[2]->GetValue()))
					);
					return new Instance(Type::GetType("Data_2D"), value);				
			}
		};

		class NoiseMethod: public Method{
			virtual String GetName() {return "Noise";}
			virtual Type* GetReturnType() {return Type::GetType("void");}
			virtual Instance* Invoke(vector<Instance*> args){
				((Data_2D*) (args[0]->GetValue()))->Noise(
					*((int*) (args[1]->GetValue())), 
					*((int*) (args[2]->GetValue())), 
					(Amplitudes*) (args[3]->GetValue())
					);
				return args[0];
			}
		};

		class GSLGenerator: public Generator
		{
		public:
			virtual bool SetScript(const String& script) {this->status = GGEN_SCRIPT_LOADED;return true;}
			virtual String GetInfo(const String& label) {return StaticString("");}
			virtual int GetInfoInt(const String& label) {return 0;}
			virtual int16* Generate(){
				this->status = GGEN_GENERATING;

#include "ggen_presets.h"

				Type* intType = Type::GetType("int");
				Type* amplitudesType = Type::GetType("Amplitudes");

				int* widthValue = new int;
				*widthValue = 1000;
				Instance* width = new Instance(intType, widthValue);

				int* heightValue = new int;
				*heightValue = 500;
				Instance* height = new Instance(intType, heightValue);

				int* defaultValue = new int;
				*defaultValue = 0;
				Instance* default = new Instance(intType, defaultValue);

				int* minValue = new int;
				*minValue = 4;
				Instance* min = new Instance(intType, minValue);

				int* maxValue = new int;
				*maxValue = 256;
				Instance* max = new Instance(intType, maxValue);

				Instance* amplitudes = new Instance(amplitudesType, ggen_std_noise);

				Method* constructor = new Data2D_Method();
				Method* noiseMethod = new NoiseMethod();

				MethodState* constStep = new MethodState();
				constStep->arguments.push_back(new ConstantState(width));
				constStep->arguments.push_back(new ConstantState(height));
				constStep->arguments.push_back(new ConstantState(default));
				constStep->method = constructor;

				MethodState* noiseStep = new MethodState();
				noiseStep->arguments.push_back(constStep);
				noiseStep->arguments.push_back(new ConstantState(min));
				noiseStep->arguments.push_back(new ConstantState(max));
				noiseStep->arguments.push_back(new ConstantState(amplitudes));
				noiseStep->method = noiseMethod;

				stack<State*> stateStack;
				stack<Instance*> instanceStack;
				stack<bool> wayBackStack;
				stack<int> previousStackSizeStack;

				stateStack.push(noiseStep);
				wayBackStack.push(false);
				previousStackSizeStack.push(0);

				while(!stateStack.empty()){
					cout << previousStackSizeStack.size() << endl;
					State* state = stateStack.top();
					int currentArgumentCount = state->arguments.size();
					int originalInstanceStackSize = previousStackSizeStack.top();
					bool isOnWayBack = wayBackStack.top();

					if(currentArgumentCount > 0)
						cout << "Entered method state " << ((MethodState*)state)->method->GetName() << (isOnWayBack ? "WB" : "") << endl;
					else
						cout << "Entered const state " << (((ConstantState*)state)->value) << endl;

					if(originalInstanceStackSize + currentArgumentCount == instanceStack.size()){
						vector<Instance*> argumentInstances;

						//wayBackStack.pop();
						wayBackStack.push(true);

						for(int i = 0; i < currentArgumentCount; i++){
							argumentInstances.push_back(instanceStack.top());
							instanceStack.pop();							
						}

						std::reverse(argumentInstances.begin(), argumentInstances.end());

						Instance* returnValue = state->Invoke(argumentInstances);
						instanceStack.push(returnValue);						

						previousStackSizeStack.pop();

						//wayBackStack.pop();
						stateStack.pop();
					}
					else {
//						for(int i = 0; i < currentArgumentCount ; i++){
							int i = instanceStack.size() - originalInstanceStackSize;

							stateStack.push(state->arguments[i]);
							wayBackStack.push(false);
							previousStackSizeStack.push(instanceStack.size());
						//}

						wayBackStack.push(false);
					}
				}

				this->output_width = 1000;
				this->output_height = 500;

				return ((Data_2D*) (instanceStack.top()->GetValue()))->data;
			}

			virtual void RegisterPreset(Data_1D* preset, const String& label){}
			virtual void RegisterPreset(Data_2D* preset, const String& label){}
			virtual void RegisterPreset(Amplitudes* preset, const String& label){}
		};
	}
}

