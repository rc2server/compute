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

bool Rc2Lexer::isCodeStartBackticks()
{
	if (_input ==  nullptr) return false;
	if (getCharPositionInLine() > 3) return false;
	if (!(_input->LA(1) == '`' &&
		_input->LA(2) == '`' &&
		_input->LA(3) == '`' &&
		_input->LA(4) == '{')) {
		return false;
	}
	if (getCharPositionInLine() > 0) {
		for (int i = getCharPositionInLine() - 1;  i >= 0; --i) {
			if(_input->LA(i) != ' ' || _input->LA(i) != '\t') {
				return false;
			}
		}
	}
	return true;
}
bool Rc2Lexer::isCodeEndBackticks()
{
	if (_input ==  nullptr) return false;
	if (getCharPositionInLine() > 3) return false;
	if (_input->LA(1) != '`' || _input->LA(2) != '`' || _input->LA(3) != '`')
		return false;
	if (getCharPositionInLine() > 0) {
		for (int i = getCharPositionInLine() - 1;  i >= 0; --i) {
			if(_input->LA(i) != ' ' || _input->LA(i) != '\t') {
				return false;
			}
		}
	}
	return true;
}
