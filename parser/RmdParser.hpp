#pragma once
#include <string>
#include <vector>
#include "Chunk.hpp"

namespace RC2 {

	class RmdParser {
	public:
		RmdParser();
		~RmdParser();
		
		std::vector<std::reference_wrapper<Chunk>> parseRmdSource(std::string source);
		
		/**
		* @brief Get all the quoted strings in source
		* 
		* @param source R source code
		* @return std::vector< std::__cxx11::string >
		*/
		std::vector<std::string> stringsInRCode(std::string source);
	};
	
};
