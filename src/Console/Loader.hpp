#pragma once

#include <vector>
#include <map>

#include "../GeoGen/GeoGen.hpp"
#include "CommandTable.hpp"
#include "ProgramArguments.hpp"


namespace geogen
{
	namespace console
	{
		class Loader
		{
			compiler::Compiler compiler;
			CommandTable commandTable;

			geogen::IStream& in;
			geogen::OStream& out;

			bool debug;
			String currentFile;
			String outputDirectory;
			runtime::CompiledScript* compiledScript;
			//String code;
			String dump;
			//ProgramArguments programArguments;
		public:
			Loader(geogen::IStream& in, geogen::OStream& out, ProgramArguments parameters);
			~Loader() { if (compiledScript != NULL) delete compiledScript; };

			inline geogen::IStream& GetIn() { return this->in; }
			inline geogen::OStream& GetOut() { return this->out; }

			inline compiler::Compiler* GetCompiler() { return &this->compiler; };
			inline CommandTable& GetCommandTable() { return this->commandTable; };

			inline String GetCurrentFile() const { return this->currentFile; }
			inline String SetCurrentFile(String currentFile) { this->currentFile = currentFile; }

			inline String GetOutputDirectory() const { return this->outputDirectory; }
			inline void SetOutputDirectory(String outputDirectory) { this->outputDirectory = outputDirectory; }

			inline String GetDump() const { return this->dump; }
			inline void SetDump(String dump) { this->dump = dump; }

			inline bool IsInDebugMode() const { return this->debug; }
			inline void SetDebugMode(bool debugMode) { this->debug = debug; }

			inline runtime::CompiledScript* GetCompiledScript() const { return this->compiledScript; }
			inline void SetCompiledScript(runtime::CompiledScript* compiledScript) { this->compiledScript = compiledScript; }

			void Run();
			void SaveRenderedMaps(renderer::RenderedMapTable& renderedMaps);
		};
	}
}