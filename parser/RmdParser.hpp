#pragma once
#include <string>
#include <vector>

namespace RC2 {

	class RmdParser {
	public:
		RmdParser();
		~RmdParser();
		
		/**
		* @brief Get all the quoted strings in source
		* 
		* @param source R source code
		* @return std::vector< std::__cxx11::string >
		*/
		std::vector<std::string> stringsInRCode(std::string source);
	};
	
};
