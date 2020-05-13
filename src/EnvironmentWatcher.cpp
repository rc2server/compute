//#include <algorithm>
#include <vector>
#include <map>
#include <sys/time.h>
#include "RC2Logging.h"
#include "EnvironmentWatcher.hpp"
#include "../common/RC2Utils.hpp"

// documentation of code flow is in rc2root:/documentation/varaibleFormat.md

extern const int kMaxLen = 100;

namespace RC2 {
void get_var_names(std::vector<Variable> &vars, std::vector<std::string> &names) {
	for_each(vars.begin(), vars.end(), [&](Variable& aVar) { 
		names.push_back(aVar.first); 
	});
}

bool compareVariablesByName(const Variable& v1, const Variable& v2) {
	return v1.first < v2.first;
}
} //namespace RC2



RC2::EnvironmentWatcher::EnvironmentWatcher ( SEXP environ, ExecuteCallback callback )
	: _env(environ), _execCallback(callback), _encoder(callback)
{

}

RC2::EnvironmentWatcher::EnvironmentWatcher ( ExecuteCallback callback )
	: _env(Rcpp::new_env(Rcpp::Environment::global_env())), _execCallback(callback), _encoder(callback)
{

}

RC2::EnvironmentWatcher::EnvironmentWatcher(const RC2::EnvironmentWatcher *const parent)
	: _env(parent->_env), _execCallback(parent->_execCallback), _encoder(parent->_execCallback)
{
}


RC2::EnvironmentWatcher::~EnvironmentWatcher()
{

}

void 
RC2::EnvironmentWatcher::captureEnvironment()
{
	_lastVars.clear();
	Rcpp::StringVector names(_env.ls(false));
	//reorder values for easier comparison
//	std::sort(names.begin(), names.end());
	std::for_each(names.begin(), names.end(), [&](const char* aName) { 
		std::string name(aName);
		_lastVars.push_back(Variable(name, _env.get(name)));
	});
	std::sort(_lastVars.begin(), _lastVars.end(), compareVariablesByName);
}

void
RC2::EnvironmentWatcher::assign(std::string name, RObject &value)
{
	_env.assign(name, value);
}

void
RC2::EnvironmentWatcher::addVariables(json::value_type &jsonContainer, bool delta, int apiVersion)
{
	if (apiVersion > 0) {
		if (delta) {
			addDelta(jsonContainer);
		} else {
			json results;
			Rcpp::StringVector names(_env.ls(false));
			std::for_each(names.begin(), names.end(), [&](const char* aName) { 
				results[aName] = toJson(aName);
			});
			jsonContainer["variables"] = results;
		}
	} else if (delta) {
		addDelta(jsonContainer);
	} else {
		json results;
		Rcpp::StringVector names(_env.ls(false));
		std::for_each(names.begin(), names.end(), [&](const char* aName) { 
			results[aName] = toJson(aName);
		});
		jsonContainer["variables"] = results;
	}
}

void
RC2::EnvironmentWatcher::addDelta(json::value_type &container) {
	std::vector<Variable> added;
	std::vector<Variable> removed;
	
	//get new variables
	Rcpp::StringVector names(_env.ls(false));
	std::vector<Variable> newVars;
	std::for_each(names.begin(), names.end(), [&](const char *aName) {
		newVars.push_back(Variable(aName, _env.get(aName))); 
	});
	std::sort(newVars.begin(), newVars.end(), compareVariablesByName);

	//sort old variables
	std::sort(_lastVars.begin(), _lastVars.end(), compareVariablesByName);

	//figure out what was removed
	std::set_difference(_lastVars.begin(), _lastVars.end(), 
						newVars.begin(), newVars.end(),
						std::back_inserter(removed), compareVariablesByName);
	//figure out what was added
	std::set_difference(newVars.begin(), newVars.end(),
						_lastVars.begin(), _lastVars.end(), 
						std::back_inserter(added));
	//craft into json
	std::vector<std::string> removedNames;
	get_var_names(removed, removedNames);
	json variables, jsonAdded;
	container["variablesRemoved"] = removedNames;
	std::for_each(added.begin(), added.end(), [&](Variable aVar) {
		json varValue;
		valueToJson(aVar.first, aVar.second, varValue, false);
		jsonAdded[aVar.first] = varValue;
	});
	container["variablesAdded"] = jsonAdded;
}

std::string 
RC2::EnvironmentWatcher::toString() const {
	json results;
	Rcpp::StringVector names(_env.ls(true));
	// this function should not modify this, but would have to audit entire source. For now, just strip away constness
	auto nonconst = const_cast<EnvironmentWatcher*> (this);
	std::for_each(names.begin(), names.end(), [&](const char* aName) { 
		results[aName] = nonconst->toJson(aName);
	});
	return results.dump(0);
}

json::value_type  
RC2::EnvironmentWatcher::toJson ( std::string varName ) 
{ 
	json results; 
	Rcpp::RObject robj(_env.get(varName)); 
	valueToJson(varName, robj, results, true); 
	return results; 
} 

void
RC2::EnvironmentWatcher::valueToJson (std::string varName, RObject& robj, json& jobj, bool includeListChildren )
{
	_encoder.valueToJson(varName, robj, jobj, includeListChildren);
}

