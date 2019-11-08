#pragma once

//#include <glog/logging.h>

#include <iostream>
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include <g3log/std2_make_unique.hpp>

namespace RC2 {

struct CustomSink {
    
    // Linux xterm color
    // http://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
    enum FG_Color {YELLOW = 33, RED = 31, GREEN=32, WHITE = 97};
    
    FG_Color GetColor(const LEVELS level) const {
        if (level.value == WARNING.value) { return YELLOW; }
        if (level.value == DEBUG.value) { return GREEN; }
        if (g3::internal::wasFatal(level)) { return RED; }
        
        return WHITE;
    }
    
    void ReceiveLogMessage(g3::LogMessageMover logEntry) {
        auto level = logEntry.get()._level;
        auto color = GetColor(level);
		std::string msg = logEntry.get().message();
//		if (msg.length() > 40)
//			msg = msg.substr(0, 37) + "\u2026";
        std::cout << logEntry.get().timestamp("%H:%M:%S") << " " << logEntry.get().level() << " [" << logEntry.get().file() << "->" << logEntry.get().function() << ":" << logEntry.get().line() << "] "		 << msg << std::endl;
//        std::cout << logEntry.get().toString() << std::endl;
//        std::cout << "\033[" << color << "m" 
//        << logEntry.get().toString() << "\033[m" << std::endl;
    }
};

};
