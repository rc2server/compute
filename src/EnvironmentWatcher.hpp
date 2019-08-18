#include <boost/noncopyable.hpp>
#define STRICT_R_HEADERS
#include <Rcpp.h>
#include <map>
#include <string>
#include "json.hpp"
#include "SessionCommon.hpp"

using json = nlohmann::json;
using Rcpp::RObject;

namespace RC2 {
	typedef std::pair<std::string, RObject> Variable;
	
class EnvironmentWatcher : private boost::noncopyable {
public:
	/**
	 * @brief create a new watcher wrapping an R environment
	 * 
	 * @param environ the environment this watcher is wrapping
	 * @param callback a lambda that will execute a statement and return the results without effecting the user output
	 */
	EnvironmentWatcher(SEXP environ, ExecuteCallback callback);

	/**
	 * @brief create a new watcher wrapping a new environment contained in the global environment
	 * 
	 * @param callback a lambda that will execute a statement and return the results without 
	 * effecting the user output. This is used to execute commands to manipulate the output
	 */
	EnvironmentWatcher(ExecuteCallback callback);

	~EnvironmentWatcher();

	/**
	 * @brief returns the current enviornment as a json object
	 * 
	 * @return json::value_type
	 */
	json::value_type toJson();
	/**
	 * @brief Returns the specified variable as a json object
	 * 
	 * @param varName name of the variable to return
	 * 
	 * @return json::value_type
	 */
	json::value_type toJson(std::string varName);
	/**
	 * @brief returns the changes since the last captureEnvironment call
	 * 
	 * @return json::value_type
	 */
	json::value_type jsonDelta();
	
	/**
	 * @brief converts an RObject to a json object
	 * 
	 * @param varName the key to insert the json into jobc under
	 * @param robj the robject to convert to json
	 * @param includeListChildren should children of a list object be included
	 * 
	 * @return Rcpp::Environment*
	 */
	void valueToJson(std::string& varName, RObject& robj, json& jobj, bool includeListChildren=false);

	Rcpp::Environment* getEnvironment() { return &_env; }
	operator Rcpp::Environment&() { return _env; } 
	
	/**
	 * @brief stores the current contents of the enviornment to allow a later call to jsonDelta()
	 * 
	 * @return void
	 */
	void captureEnvironment();
	void clear() { _lastVars.erase(_lastVars.begin(), _lastVars.end()); }
	
protected:
	Rcpp::Environment _env;
	std::vector<Variable> _lastVars;
	ExecuteCallback _execCallback;
	
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
	
	void addSummary(std::string& varName, json& jobj);
};

	
} //namespace RC2
