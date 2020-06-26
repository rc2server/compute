#pragma once

#include <iostream>

#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>

BOOST_LOG_ATTRIBUTE_KEYWORD(line_num, "LineNum", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(filename, "File", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(function, "Function", std::string)

BOOST_LOG_GLOBAL_LOGGER(logger, boost::log::sources::severity_logger<boost::log::trivial::severity_level>)
// helper macro, should only be called by following macros
#define LOG(severity) BOOST_LOG_SEV(logger::get(), boost::log::trivial::severity) \
    << boost::log::add_value("LineNum", __LINE__) \
    << boost::log::add_value("File", __FILE__) \
    << boost::log::add_value("Function", __FUNCTION__)

#define LOG_TRACE   LOG(trace)
#define LOG_DEBUG   LOG(debug)
#define LOG_INFO    LOG(info)
#define LOG_WARNING LOG(warning)
#define LOG_ERROR   LOG(error)
#define LOG_FATAL   LOG(fatal)
