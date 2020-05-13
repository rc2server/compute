#pragma once

#include <boost/noncopyable.hpp>
#define STRICT_R_HEADERS
#include <Rcpp.h>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include "SessionCommon.hpp"

using json = nlohmann::json;
using Rcpp::RObject;

namespace RC2 {
class RJsonEncoder {
public:
	
	RJsonEncoder(ExecuteCallback cbacks) 
		: _execCallback(cbacks) {};
		
		json toJson(RObject& robj, bool includeSummaries = true);

		
	/**
	 * @brief converts an RObject to a json object
	 * 
	 * @param varName the key to insert the json into jobc under
	 * @param robj the robject to convert to json
	 * @param jobj the json that the specified variables will be added to
	 * @param includeListChildren should children of a list object be included
	 * @param includeSummaries defaults to true, adds small amount of overhead
	 * 
	 */
	void valueToJson(std::string varName, RObject& robj, json& jobj, bool includeListChildren=false, bool includeSummaries=true);
	
	
protected:
	ExecuteCallback _execCallback;

	json rvectorToJsonArray(RObject& robj);
	
	void getAttributes(RObject &robj, json& results);
	
	void setObjectData(RObject& robj, json& jobj);
	void setFactorData(RObject& robj, json& jobj);
	void setDataFrameData(RObject& robj, json& jobj);
	void setGenericObjectData(RObject& robj, json& jobj);
	void setEnvironmentData(RObject& robj, json& jobj);
	void setFunctionData(RObject& robj, json& jobj);
	void setPrimitiveData(RObject& robj, json& jobj);
	void setDimNames(RObject& robj, json& jobj);
	void setListData(RObject& robj, json& jobj, bool includeListChildren);
	void setPairListData(RObject& robj, json& jobj);
	
	void addSummary(std::string& varName, json& jobj);
};

} // end namespace
