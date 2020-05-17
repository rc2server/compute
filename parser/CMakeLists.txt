CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)
project (parser)

include_directories (${CMAKE_SOURCE_DIR}/vendor/antlr/include/antlr4-runtime/ )

# Antlr displays an error message in ATN.cpp. This is used to disable those warnings
add_compile_options(-Wno-attributes)

set(PARSER_SRC RmdParser.cpp RStringVisitor.cpp Chunk.cpp Rc2ParserListener.cpp Rc2RFilter.cpp PrivateChunks.cpp )
set(PARSER_GENERATED generated/Rc2Lexer.cpp generated/Rc2RawParser.cpp generated/Rc2RawParserBaseListener.cpp generated/Rc2RawParserBaseVisitor.cpp generated/Rc2RawParserListener.cpp generated/Rc2RawParserVisitor.cpp generated/RParser.cpp generated/RFilter.cpp generated/RLexer.cpp)
add_library (parser ${PARSER_SRC} ${PARSER_GENERATED})
