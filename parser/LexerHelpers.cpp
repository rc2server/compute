
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
