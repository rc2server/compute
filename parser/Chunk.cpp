#include "Chunk.hpp"

ostream& operator<<(ostream& os, const NSRange range) {
	os << "{loc=" << range.location << ",len=" << range.length << "}";
	return os;
}

ostream& operator<<(ostream& os, const ChunkType type)
{
	switch(type) {
		case markdown: 
			os << "markdown"; break;
		case code:
			os << "code"; break;
		case equation:
			os << "equation"; break;
		case inlineCode:
			os << "inlineCode"; break;
		case inlineEquation:
			os << "inlineEquation"; break;
	}
	return os;
}

