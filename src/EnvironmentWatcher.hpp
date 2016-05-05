#include <boost/noncopyable.hpp>
#define STRICT_R_HEADERS
#include <Rcpp.h>
#include <map>
#include <string>
#include "json.hpp"

using json = nlohmann::json;
using Rcpp::RObject;

namespace RC2 {
	typedef std::pair<std::string, RObject> Variable;
	
class EnvironmentWatcher : private boost::noncopyable {
public:
	EnvironmentWatcher(SEXP environ);
	~EnvironmentWatcher();

	json::value_type toJson();
	json::value_type toJson(std::string varName);
	json::value_type jsonDelta();
	
	void captureEnvironment();
	void clear() { _lastVars.erase(_lastVars.begin(), _lastVars.end()); }
	
protected:
	Rcpp::Environment _env;
	std::vector<Variable> _lastVars;
	
	void valueToJson(RObject& robj, json& jobj, bool includeListChildren);
	//returns array
	json rvectorToJsonArray(RObject& robj);
	
	void setObjectData(RObject& robj, json& jobj);
	void setFactorData(RObject& robj, json& jobj);
	void setDataFrameData(RObject& robj, json& jobj);
	void setGenericObjectData(RObject& robj, json& jobj);
	void setEnvironmentData(RObject& robj, json& jobj);
	void setFunctionData(RObject& robj, json& jobj);
	void setPrimitiveData(RObject& robj, json& jobj);
	void setDimNames(RObject& robj, json& jobj);
	void setListData(RObject& robj, json& jobj, bool includeListChildren);
};

	
} //namespace RC2
