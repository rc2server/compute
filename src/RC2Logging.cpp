#include "RC2Logging.h"

#include <boost/log/core/core.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <fstream>
#include <ostream>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;

BOOST_LOG_GLOBAL_LOGGER_INIT(logger, src::severity_logger) {
		src::severity_logger<logging::trivial::severity_level> logger;
		
		typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
		boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();
		
		//add console output
		boost::shared_ptr<std::ostream> cstream(&std::clog, boost::null_deleter());
		sink->locked_backend()->add_stream(cstream);
		
		logging::add_common_attributes();
		//format
		logging::formatter formatter = expr::stream
			<< "["  << logging::trivial::severity << "] " 
			//<< std::setfill(' ') << " | "
			<< expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d, %H:%M:%S") << " "
			<< "[" << filename << ":" << line_num << "]"
			<< " - " << expr::smessage;
		sink->set_formatter(formatter);
		sink->set_filter(logging::trivial::severity >= logging::trivial::info);
		
		logging::core::get()->add_sink(sink);
		
		return logger;
}
