#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Chunk.hpp"

namespace RC2 {
	struct ParserData;
	std::string CalculateSHA256Hash(std::string& input); 
		
	class RmdParser {
	public:
		RmdParser();
		~RmdParser();
		
		/**
		 * @brief parses the document and returns an array of Chunk pointers. The elements are owned by the parser and are freed the next time this method is called or the parser is destroyed
		 * 
		 * @param source Rmd source code
		 * @return std::vector< Chunk* >
		 */
		std::vector<Chunk*> parseRmdSource(std::string source);
		
		/**
		* @brief Get all the quoted strings in source
		* 
		* @param source R source code
		* @return std::vector< std::__cxx11::string >
		*/
		std::vector<std::string> stringsInRCode(std::string source);
		
	protected:
		std::unique_ptr<ParserData> _impl;
	};
	
};
