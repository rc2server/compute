#pragma once

#include <boost/noncopyable.hpp>
#define STRICT_R_HEADERS
#include <Rcpp.h>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include "SessionCommon.hpp"
#include "RJsonEncoder.hpp"

using json = nlohmann::json;
using Rcpp::RObject;

namespace RC2 {
	class RJsonEncoder;
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
	 * @brief creates a new environmnet inside parent
	 * 
	 * @param parent the parent environment
	 */
	EnvironmentWatcher(const EnvironmentWatcher * const parent);
	
	/**
	 * @brief create a new watcher wrapping a new environment contained in the global environment
	 * 
	 * @param callback a lambda that will execute a statement and return the results without 
	 * effecting the user output. This is used to execute commands to manipulate the output
	 */
	EnvironmentWatcher(ExecuteCallback callback);

	~EnvironmentWatcher();

	/**
	 * @brief Returns the environment's contents a a json string
	 * 
	 * @return std::__cxx11::string
	 */
	std::string toString()const;
	
	/**
	 * @brief Returns the specified variable as a json object
	 * 
	 * @param varName name of the variable to return
	 * 
	 * @return json::value_type
	 */
	json::value_type toJson(std::string varName);
	
	/**
	 * @brief assigns a value in the environment
	 * 
	 * @param name the name to assign value to
	 * @param value the value to assign to name
	 */
	void assign(std::string name, RObject &value);
	
	/**
	* @brief adds variables to jsonContainer. If apiVersion > 0, adds "variables" (map of name to value), 
	* "variablesAdded" (map to new variables), "veriablesRemoved" (array of variable names)
	* If apiVersion == 0, returns old format of variables map with added and removed keys
	* 
	* @param jsonContainer the container to add values to
	* @param delta should this be changes, or everyting
	* @param apiVersion the apiVersion to use. version 1 changed format
	*/
	void addVariables(json::value_type &jsonContainer, bool delta, int apiVersion);
	
	/**
	 * @brief converts an RObject to a json object
	 * 
	 * @param varName the key to insert the json into jobc under
	 * @param robj the robject to convert to json
	 * @param includeListChildren should children of a list object be included
	 * 
	 * @return Rcpp::Environment*
	 */
	void valueToJson(std::string varName, RObject& robj, json& jobj, bool includeListChildren=false);

	Rcpp::Environment const * getEnvironment() const { return &_env; }
	void getEnvironment(Rcpp::Environment &env) { env = _env.get__(); }
//	operator Rcpp::Environment&() const { return _env; } 
	
	/**
	 * @brief stores the current contents of the enviornment to allow a later call to jsonDelta()
	 * 
	 * @return void
	 */
	void captureEnvironment();
	void clear() { _lastVars.erase(_lastVars.begin(), _lastVars.end()); }
	
protected:
	const Rcpp::Environment _env;
	std::vector<Variable> _lastVars;
	ExecuteCallback _execCallback;
	RJsonEncoder _encoder;

	// addds delta entries (variables, variablesAdded, avariablesRemoved) to container
	void addDelta(json::value_type &container);	

};

	
} //namespace RC2
