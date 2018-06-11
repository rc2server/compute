#pragma once
#include <RInside.h>
#include <functional>
#include <boost/noncopyable.hpp>

namespace RC2 {


	class RSessionCallbacks : public Callbacks, private boost::noncopyable 
	{
		public:
			RSessionCallbacks() {}
			virtual ~RSessionCallbacks() {}
		
			void ShowMessage(const char *message) override;
			void WriteConsole( const std::string& line, int type ) override;
		
			bool has_ShowMessage() override { return true; }
			bool has_WriteConsole() override { return true; }

			//used for WriteConsole
			std::function<void(const std::string&, bool)> _writeLambda;
	};
};

