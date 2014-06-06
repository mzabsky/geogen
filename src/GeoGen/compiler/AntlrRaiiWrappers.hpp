#pragma once

#include <vector>

#include <antlr3.h>

#include "../runtime/FunctionDefinition.hpp"
#include "../runtime/VariableDefinition.hpp"
#include "../runtime/SymbolDefinitionTable.hpp"
#include "../runtime/CodeBlock.hpp"
#include "../Grammar/output\GeoGenScriptLexer.h"
#include "../Grammar/output\GeoGenScriptParser.h"
#include "../Grammar/output\GeoGenScriptDecls.h"

namespace geogen
{
	namespace compiler
	{
		class AnlrInputStreamWrapper
		{
			pANTLR3_INPUT_STREAM ptr;

			AnlrInputStreamWrapper(AnlrInputStreamWrapper const& other);
		public:
			AnlrInputStreamWrapper(pANTLR3_UINT8 data, ANTLR3_UINT32 encoding, ANTLR3_UINT32 size, pANTLR3_UINT8 name)
			{
				ptr = antlr3StringStreamNew(data, encoding, size, name);
			}			

			~AnlrInputStreamWrapper()
			{
				ptr->close(ptr);
			}

			inline pANTLR3_INPUT_STREAM GetPtr()
			{
				return this->ptr;
			}
		};

		class AntlrLexerWrapper
		{
			pGeoGenScriptLexer ptr;

			AntlrLexerWrapper(AntlrLexerWrapper const& other);
		public:
			AntlrLexerWrapper(AnlrInputStreamWrapper& input)
			{
				ptr = GeoGenScriptLexerNew(input.GetPtr());
			}

			~AntlrLexerWrapper()
			{
				ptr->free(ptr);
			}

			inline pGeoGenScriptLexer GetPtr()
			{
				return this->ptr;
			}
		};

		class AntlrTokenStreamWrapper
		{
			pANTLR3_COMMON_TOKEN_STREAM ptr;

			AntlrTokenStreamWrapper(AntlrTokenStreamWrapper const& other);
		public:
			AntlrTokenStreamWrapper(ANTLR3_UINT32 hint, AntlrLexerWrapper& lex)
			{
				ptr = antlr3CommonTokenStreamSourceNew(hint, TOKENSOURCE(lex.GetPtr()));
			}

			~AntlrTokenStreamWrapper()
			{
				ptr->free(ptr);
			}

			inline pANTLR3_COMMON_TOKEN_STREAM GetPtr()
			{
				return this->ptr;
			}
		};

		class AntlrParserWrapper
		{
			pGeoGenScriptParser ptr;

			AntlrParserWrapper(AntlrParserWrapper const& other);
		public:
			AntlrParserWrapper(AntlrTokenStreamWrapper& tokens)
			{
				ptr = GeoGenScriptParserNew(tokens.GetPtr());
			}

			~AntlrParserWrapper()
			{
				ptr->free(ptr);
			}

			inline pGeoGenScriptParser GetPtr()
			{
				return this->ptr;
			}
		};

		class AntlrTreeNodeStreamWrapper
		{
			pANTLR3_COMMON_TREE_NODE_STREAM ptr;

			AntlrTreeNodeStreamWrapper(AntlrTreeNodeStreamWrapper const& other);
		public:
			AntlrTreeNodeStreamWrapper(pANTLR3_BASE_TREE tree, ANTLR3_UINT32 hint)
			{
				ptr = antlr3CommonTreeNodeStreamNewTree(tree, hint);
			}

			~AntlrTreeNodeStreamWrapper()
			{
				ptr->free(ptr);
			}

			inline pANTLR3_COMMON_TREE_NODE_STREAM GetPtr()
			{
				return this->ptr;
			}
		};

		class AntlrTreeWalkerWrapper
		{
			pGeoGenScriptDecls ptr;

			AntlrTreeWalkerWrapper(AntlrTreeWalkerWrapper const& other);
		public:
			AntlrTreeWalkerWrapper(AntlrTreeNodeStreamWrapper& nodes)
			{
				ptr = GeoGenScriptDeclsNew(nodes.GetPtr());
			}

			~AntlrTreeWalkerWrapper()
			{
				ptr->free(ptr);
			}

			inline pGeoGenScriptDecls GetPtr()
			{
				return this->ptr;
			}
		};


		class AntlrVectorFactoryWrapper
		{
			pANTLR3_VECTOR_FACTORY ptr;

			AntlrVectorFactoryWrapper(AntlrVectorFactoryWrapper const& other);
		public:
			AntlrVectorFactoryWrapper()
			{
				ptr = antlr3VectorFactoryNew(0);
			}

			~AntlrVectorFactoryWrapper()
			{
				ptr->close(ptr);
			}

			inline pANTLR3_VECTOR_FACTORY GetPtr()
			{
				return this->ptr;
			}
		};
	}
}