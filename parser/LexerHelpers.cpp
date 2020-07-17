#include <boost/algorithm/algorithm.hpp>

bool Rc2Lexer::isInlineEqStart()
{
	auto prev = _input->LA(-2);
	auto next = _input->LA(1);
	if (prev == '$' || next == '$')
		return false;
	return true;
}

bool Rc2Lexer::isEQStart()
{
	return 
		_input->LA(-1) != '$' &&
		_input->LA(1) == '$' &&
		_input->LA(2) == '$' &&
		_input->LA(3) != '$';
}

bool Rc2Lexer::isCodeBackticks() 
{
	if (_input ==  nullptr) return false;
	if (_input->LA(1) != '`' ||
		_input->LA(2) != '`' ||
		_input->LA(3) != '`') {
		return false;
	}
	for (int i = -1; i < -getCharPositionInLine(); --i) {
		if(_input->LA(i) != 9 || _input->LA(i) != 32) // tab or space
			return false;
	}
	return true;
}
