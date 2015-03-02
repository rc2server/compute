#pragma once
#include <RInside.h>
#include <functional>

namespace RC2 {


	class RSessionCallbacks : public Callbacks {
		public:
			RSessionCallbacks() {}
			virtual ~RSessionCallbacks() {}
		
			void ShowMessage(const char *message) override;
			void WriteConsole( const std::string& line, int type ) override;
		
			bool has_ShowMessage() override { return true; }
			bool has_WriteConsole() override { return true; }

			//used for WriteConsole
			std::function<void(std::string, bool)> _writeLambda;
	};
};
