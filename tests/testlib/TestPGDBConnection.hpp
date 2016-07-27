#pragma once

#include "../common/PGDBConnection.hpp"

namespace RC2 {
	
class TestPGDBConnection: public PGDBConnection {
public:
	TestPGDBConnection();
	
	virtual void connect(std::string connectStr);
	
protected:
	std::string readConfig();

private:
	//disable copying so move semantics are used
	TestPGDBConnection(const TestPGDBConnection&);
	TestPGDBConnection& operator=(const TestPGDBConnection&);
	
};

}; //namespace RC2
