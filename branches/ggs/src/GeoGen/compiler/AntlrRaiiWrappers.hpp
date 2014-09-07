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
		/// RAII wrapper for ANTRL input stream.
		class AnlrInputStreamWrapper
		{
			pANTLR3_INPUT_STREAM ptr;

			AnlrInputStreamWrapper(AnlrInputStreamWrapper const& other);
		public:

			/// Constructor.
			/// @param data The data.
			/// @param encoding The encoding.
			/// @param size The size.
			/// @param name The name.
			AnlrInputStreamWrapper(pANTLR3_UINT8 data, ANTLR3_UINT32 encoding, ANTLR3_UINT32 size, pANTLR3_UINT8 name)
			{
				ptr = antlr3StringStreamNew(data, encoding, size, name);
			}			

			/// Destructor.
			~AnlrInputStreamWrapper()
			{
				ptr->close(ptr);
			}

			/// Gets the pointer to the wrapped object.
			/// @return The pointer.
			inline pANTLR3_INPUT_STREAM GetPtr()
			{
				return this->ptr;
			}
		};

		/// RAII wrapper for ANTRL lexer.
		class AntlrLexerWrapper
		{
			pGeoGenScriptLexer ptr;

			AntlrLexerWrapper(AntlrLexerWrapper const& other);
		public:

			/// Constructor.
			/// @param input The input stream.
			AntlrLexerWrapper(AnlrInputStreamWrapper& input)
			{
				ptr = GeoGenScriptLexerNew(input.GetPtr());
			}

			/// Destructor.
			~AntlrLexerWrapper()
			{
				ptr->free(ptr);
			}

			/// Gets the pointer to the wrapped object.
			/// @return The pointer.
			inline pGeoGenScriptLexer GetPtr()
			{
				return this->ptr;
			}
		};

		/// RAII wrapper for ANTRL token stream.
		class AntlrTokenStreamWrapper
		{
			pANTLR3_COMMON_TOKEN_STREAM ptr;

			AntlrTokenStreamWrapper(AntlrTokenStreamWrapper const& other);
		public:

			/// Constructor.
			/// @param hint The hint.
			/// @param lex The lexer.
			AntlrTokenStreamWrapper(ANTLR3_UINT32 hint, AntlrLexerWrapper& lex)
			{
				ptr = antlr3CommonTokenStreamSourceNew(hint, TOKENSOURCE(lex.GetPtr()));
			}

			/// Destructor.
			~AntlrTokenStreamWrapper()
			{
				ptr->free(ptr);
			}

			/// Gets the pointer to the wrapped object.
			/// @return The pointer.
			inline pANTLR3_COMMON_TOKEN_STREAM GetPtr()
			{
				return this->ptr;
			}
		};

		/// RAII wrapper for ANTRL parser.
		class AntlrParserWrapper
		{
			pGeoGenScriptParser ptr;

			AntlrParserWrapper(AntlrParserWrapper const& other);
		public:

			/// Constructor.
			/// @param tokens The token stream.
			AntlrParserWrapper(AntlrTokenStreamWrapper& tokens)
			{
				ptr = GeoGenScriptParserNew(tokens.GetPtr());
			}

			/// Destructor.
			~AntlrParserWrapper()
			{
				ptr->free(ptr);
			}

			/// Gets the pointer to the wrapped object.
			/// @return The pointer.
			inline pGeoGenScriptParser GetPtr()
			{
				return this->ptr;
			}
		};

		/// RAII wrapper for ANTRL node stream.
		class AntlrTreeNodeStreamWrapper
		{
			pANTLR3_COMMON_TREE_NODE_STREAM ptr;

			AntlrTreeNodeStreamWrapper(AntlrTreeNodeStreamWrapper const& other);
		public:

			/// Constructor.
			/// @param tree The tree.
			/// @param hint The hint.
			AntlrTreeNodeStreamWrapper(pANTLR3_BASE_TREE tree, ANTLR3_UINT32 hint)
			{
				ptr = antlr3CommonTreeNodeStreamNewTree(tree, hint);
			}

			/// Destructor.
			~AntlrTreeNodeStreamWrapper()
			{
				ptr->free(ptr);
			}

			/// Gets the pointer to the wrapped object.
			/// @return The pointer.
			inline pANTLR3_COMMON_TREE_NODE_STREAM GetPtr()
			{
				return this->ptr;
			}
		};

		/// RAII wrapper for ANTRL tree walker.
		class AntlrTreeWalkerWrapper
		{
			pGeoGenScriptDecls ptr;

			AntlrTreeWalkerWrapper(AntlrTreeWalkerWrapper const& other);
		public:

			/// Constructor.
			/// @param nodes The node stream.
			AntlrTreeWalkerWrapper(AntlrTreeNodeStreamWrapper& nodes)
			{
				ptr = GeoGenScriptDeclsNew(nodes.GetPtr());
			}

			/// Destructor.
			~AntlrTreeWalkerWrapper()
			{
				ptr->free(ptr);
			}

			/// Gets the pointer to the wrapped object.
			/// @return The pointer.
			inline pGeoGenScriptDecls GetPtr()
			{
				return this->ptr;
			}
		};

		/// RAII wrapper for ANTRL vector factory.
		class AntlrVectorFactoryWrapper
		{
			pANTLR3_VECTOR_FACTORY ptr;

			AntlrVectorFactoryWrapper(AntlrVectorFactoryWrapper const& other);
		public:
			/// Default constructor.
			AntlrVectorFactoryWrapper()
			{
				ptr = antlr3VectorFactoryNew(0);
			}

			/// Destructor.
			~AntlrVectorFactoryWrapper()
			{
				ptr->close(ptr);
			}

			/// Gets the pointer to the wrapped object.
			/// @return The pointer.
			inline pANTLR3_VECTOR_FACTORY GetPtr()
			{
				return this->ptr;
			}
		};
	}
}