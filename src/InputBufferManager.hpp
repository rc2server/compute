#pragma once

#include <event2/event.h>
#include <event2/buffer.h>
#include <memory>
#include <boost/noncopyable.hpp>

extern const uint32_t kRSessionMagicNumber;

namespace RC2 {

	class InputBufferManager : private boost::noncopyable {
	
		public:
			InputBufferManager();
			virtual ~InputBufferManager();
			
			void appendData(struct evbuffer *inBuffer);
			bool hasCompleteMessage();
			std::string popCurrentMessage();
			
		private:
			void reset();
		
			struct evbuffer*	_buffer;
			int32_t				_expectedSize;
			std::string			_currentJson;
	};

};
