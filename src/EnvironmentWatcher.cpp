//#include <algorithm>
#include <vector>
#include <map>
#include <sys/time.h>
#include "RC2Logging.h"
#include "EnvironmentWatcher.hpp"
#include "../common/RC2Utils.hpp"

const int kMaxLen = 100;
const char *kNotAVector = "notAVector";
const char *kPrimitive = "primitive";
const char *kClass = "class";
const char *kValue = "value";
const char *kName = "name";
const char *kType = "type";

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

inline bool isOrderedFactor(Rcpp::StringVector& classNames, RObject& robj) {
	return classNames.length() > 1 && classNames[0] == "ordered" && classNames[1] == "factor";
}

std::string
columnType(RObject& robj, json& jobj, int colNum) {
	if (Rf_isFactor(robj)) {
		Rcpp::StringVector classVal(robj.attr(kClass));
		if (isOrderedFactor(classVal, robj)) {
			return "of";
		} else {
			return "f";
		}
		char namebuf[16];
		snprintf(namebuf, 16, "f%d", colNum);
		jobj[namebuf] = Rcpp::StringVector(robj.attr("levels"));
	} else {
		switch(robj.sexp_type()) {
			case LGLSXP: return "b";
			case INTSXP: return "i";
			case REALSXP: return "d";
			case STRSXP: return "s";
		}
	}
	return "-";
}

RC2::EnvironmentWatcher::EnvironmentWatcher ( SEXP environ, ExecuteCallback callback )
	: _env(environ), _execCallback(callback)
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

json::value_type 
RC2::EnvironmentWatcher::toJson ( std::string varName )
{
	json results;
	Rcpp::RObject robj(_env.get(varName));
	valueToJson(varName, robj, results, true);
	return results;
}

json::value_type 
RC2::EnvironmentWatcher::toJson()
{
	json results;
	Rcpp::StringVector names(_env.ls(false));
	std::for_each(names.begin(), names.end(), [&](const char* aName) { 
		results[aName] = toJson(aName);
	});
	return results;
}

json::value_type 
RC2::EnvironmentWatcher::jsonDelta()
{
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
	json results, jsonAdded;
	results["removed"] = removedNames;
	std::for_each(added.begin(), added.end(), [&](Variable aVar) {
		json varValue;
		valueToJson(aVar.first, aVar.second, varValue, false);
		jsonAdded[aVar.first] = varValue;
	});
	results["assigned"] = jsonAdded;
	return results;
}

void
RC2::EnvironmentWatcher::addSummary(std::string& varName, json& jobj)
{
	std::string vname = varName;
	StripQuotes(vname);
	std::string cmd = "capture.output(str(" + vname + "))";
	RObject result;
	_execCallback(cmd, result);
	try {
		Rcpp::StringVector strs(result);
		std::string summary;
		std::for_each(strs.begin(), strs.end(), [&](const char* str) { 
			if (summary.length() > 0)
				summary += "\n";
			summary += str; 
		});
		jobj["summary"] = summary;
	} catch (...) {
		LOG(INFO) << "exception for summary of " << varName << std::endl;
	}
}

void
RC2::EnvironmentWatcher::valueToJson (std::string& varName, RObject& robj, json& jobj, bool includeListChildren )
{
	jobj[kName] = varName;
	if (Rf_isObject(robj)) {
		setObjectData(robj, jobj);
		addSummary(varName, jobj);
		return;
	}
	switch(robj.sexp_type()) {
		case VECSXP:
			setListData(robj, jobj, includeListChildren);
			addSummary(varName, jobj);
			break;
		case ENVSXP:
			setEnvironmentData(robj, jobj);
			break;
		case CLOSXP: //3
		case SPECIALSXP: //7
		case BUILTINSXP: //8
			setFunctionData(robj, jobj);
			break;
		default:
			setPrimitiveData(robj, jobj);
			break;
	}
}

void
RC2::EnvironmentWatcher::setListData ( RObject& robj, json& jobj, bool includeListChildren )
{
	int len = LENGTH(robj);
	jobj[kClass] = "list";
	RObject names(robj.attr("names"));
	json nameArray = rvectorToJsonArray(names);
	jobj["names"] = nameArray;
	jobj["length"] = len;
	std::string emptyStr;
	if (includeListChildren) {
		json children;
		int maxLen = len < kMaxLen ? len : kMaxLen;
		for (int i=0; i < maxLen; i++) {
			RObject aChild(VECTOR_ELT(robj, i));
			json childObj;
			valueToJson(emptyStr,  aChild, childObj, false);
			childObj[kName] = nameArray[i];
			children.push_back(childObj);
		}
		jobj[kValue] = children;
	}
}

void 
RC2::EnvironmentWatcher::setObjectData ( RObject& robj, json& jobj )
{
	Rcpp::StringVector klazzNames(robj.attr(kClass));
	if (isOrderedFactor(klazzNames, robj)) {
		jobj[kClass] = "ordered factor";
	} else {
		jobj[kClass] = klazzNames[0];
	}
	if (Rf_isS4(robj))
		jobj["S4"] = true;
	if (Rf_isFactor(robj)) {
		setFactorData(robj, jobj);
	} else if (jobj[kClass] == "Date") {
		Rcpp::Date date(robj);
		char datebuf[16];
		snprintf(datebuf, 16, "%02d-%02d-%02d", date.getYear(), date.getMonth(), date.getDay());
		jobj[kValue] = datebuf;
	} else if (jobj[kClass] == "POSIXct") {
		jobj[kValue] = REAL(robj)[0];
	} else if (jobj[kClass] == "POSIXlt") {
		Rcpp::Datetime dt(robj);
		jobj[kValue] = dt.getFractionalTimestamp();
	}  else if (jobj[kClass] == "data.frame") {
		setDataFrameData(robj, jobj);
	} else {
		setGenericObjectData(robj, jobj);
	}
}

void 
RC2::EnvironmentWatcher::setFactorData ( RObject& robj, json& jobj )
{
	jobj["type"] = "f";
	Rcpp::StringVector levs(robj.attr("levels"));
	jobj["levels"] = levs;
	jobj[kValue] = Rcpp::IntegerVector(robj);
}

void 
RC2::EnvironmentWatcher::setDataFrameData ( RObject& robj, json& jobj )
{
	int colCount = LENGTH(robj);
	Rcpp::StringVector colNames(robj.attr("names"));
	jobj["cols"] = colNames;
	jobj["ncol"] = colCount;
	RObject rowList(robj.attr("row.names"));
	if (LENGTH(rowList) > 0) {
		jobj["row.names"] = Rcpp::StringVector(rowList);
	}
	json colTypes;
	std::vector<RObject> colObjs;
	for (int i=0; i < colCount; i++) {
		RObject element(VECTOR_ELT(robj, i));
		if (element.sexp_type() == CPLXSXP) //coerce complex to string
			element = Rf_coerceVector(element, STRSXP);
		colObjs.push_back(element);
		colTypes.push_back(columnType(element, jobj, i));
	}
	jobj["types"] = colTypes;
	int rowCount = LENGTH(colObjs[0]);
	jobj["nrow"] = rowCount;
	//create robjs for each column list
	json rows;
	for (int row=0; row < kMaxLen && row < rowCount; row++) {
		json aRow;
		for (int col=0; col < colNames.length(); col++) {
			RObject &val = colObjs[col];
			switch(val.sexp_type()) {
				case LGLSXP: aRow.push_back(LOGICAL(val)[row]); break;
				case INTSXP: aRow.push_back(INTEGER(val)[row]); break;
				case REALSXP: aRow.push_back(REAL(val)[row]); break;
				case STRSXP: 
					aRow.push_back(Rcpp::StringVector(val)[0]); 
					break;
				default:
					LOG(WARNING) << "dataframe invalid col type:" << val.sexp_type() << std::endl;
					aRow.push_back(nullptr);
					break;
			}
		}
		rows.push_back(aRow);
	}
	jobj["rows"] = rows;
}

void
RC2::EnvironmentWatcher::setFunctionData ( RObject& robj, json& jobj )
{
	jobj[kClass] = "function";
	jobj[kNotAVector] = true;
	jobj[kPrimitive] = false;
	try {
		Rcpp::Environment env("package:rc2");
		Rcpp::Function fun(env["rc2.defineFunction"]);
		RObject val = fun(robj);
		json lines;
		std::ostringstream stream;
		Rcpp::StringVector svec(val);
		std::for_each(svec.begin(), svec.end(), [&](const char *aLine) 
		{
			stream << aLine << std::endl;
		});
		jobj["body"] = stream.str();
	} catch (std::exception &e) {
		LOG(WARNING) << "failed to get function body:" << e.what() << std::endl;
	} 
}

void
RC2::EnvironmentWatcher::setGenericObjectData ( RObject& robj, json& jobj )
{
	auto attrNames = robj.attributeNames();
	json nameArray;
	json valueArray;
	std::for_each(attrNames.begin(), attrNames.end(),[&](std::string aName) {
		if (aName == kClass) return;
		nameArray.push_back(aName);
		Rcpp::RObject attrVal(robj.attr(aName));
		json attrJval;
		valueToJson(aName, attrVal, attrJval, true);
		attrJval[kName] = aName;
		valueArray.push_back(attrJval);
	});
	jobj["generic"] = true;
	jobj["names"] = nameArray;
	jobj[kValue] = valueArray;
	jobj["length"] = valueArray.size();
}

void
RC2::EnvironmentWatcher::setEnvironmentData ( RObject& robj, json& jobj )
{
	json childArray;
	Rcpp::Environment cenv(robj);
	Rcpp::StringVector cnames(cenv.ls(false));
	int len = cnames.size() < kMaxLen ? cnames.size() : kMaxLen;
	for (int i=0; i < len; i++) {
		json cobj;
		std::string varName(cnames[i]);
		cobj[kName] = varName;
		RObject childVal(cenv.get(varName));
		valueToJson(varName, childVal, cobj, true);
		childArray.push_back(cobj);
	}
	jobj[kValue] = childArray;
}

void 
RC2::EnvironmentWatcher::setPrimitiveData ( RObject& robj, json& jobj )
{
	jobj[kPrimitive] = true; //override below if necessary
	bool notVector = false;
	switch(robj.sexp_type()) {
		case NILSXP: //0
			jobj[kClass] = "NULL";
			jobj[kType] = "n";
			notVector = true;
			break;
		case LGLSXP: //10
			jobj[kClass] = "logical";
			jobj[kType] = "b";
			{ //logicalvector gets stored in json as ints, not bools. Convert manually
				Rcpp::LogicalVector logics(robj);
				std::vector<bool> vals;
				std::for_each(logics.begin(), logics.end(), [&](int aVal) { vals.push_back(aVal == 0 ? false : true); });
				jobj[kValue] = vals;
			}
			break;
		case INTSXP: //13
			jobj[kClass] = "integer vector";
			jobj[kType] = "i";
			jobj[kValue] = Rcpp::IntegerVector(robj);
			break;
		case REALSXP: //14
			jobj[kClass] = "numeric vector";
			jobj[kType] = "d";
			{
				Rcpp::NumericVector dvals(robj);
				json jvals;
				std::for_each(dvals.begin(), dvals.end(), [&](double d) 
				{
					if (d == R_NaN) jvals.push_back("NaN");
					else if (d == R_PosInf) jvals.push_back("Inf");
					else if (d == R_NegInf) jvals.push_back("-Inf");
					else jvals.push_back(d);
				});
				jobj[kValue] = jvals;
			}
			break;
		case STRSXP: //16
			jobj[kClass] = "string";
			jobj[kType] = "s";
			jobj[kValue] = Rcpp::StringVector(robj);
			break;
		case CPLXSXP:
			jobj[kClass] = "complex";
			jobj[kType] = "c";
			jobj[kValue] = Rcpp::StringVector(robj);
			break;
		case RAWSXP: //24
			jobj[kClass] = "raw";
			jobj[kType] = "r";
			break;
		default:
			jobj[kClass] = "unsupported type";
			notVector = true;
			jobj[kPrimitive] = false;
			LOG(INFO)  << "unsupported type:" << robj.sexp_type() << std::endl;
			break;
	}
	if (notVector)
		jobj[kNotAVector] = true;
	else
		jobj["length"] = LENGTH(robj);
	if (Rf_isArray(robj)) {
		jobj[kPrimitive] = false;
		setDimNames(robj, jobj);
		if (Rf_isMatrix(robj))
			jobj[kClass] = "matrix";
		else
			jobj[kClass] = "array";
	}
}

json 
RC2::EnvironmentWatcher::rvectorToJsonArray ( RObject& robj )
{
	json array;
	switch(robj.sexp_type()) {
		case LGLSXP:
			array = Rcpp::LogicalVector(robj);
			break;
		case INTSXP:
			array = Rcpp::IntegerVector(robj);
			break;
		case REALSXP:
			array = Rcpp::NumericVector(robj);
			break;
		case STRSXP:
			array = Rcpp::StringVector(robj);
			break;
		case NILSXP:
			array = nullptr;
			break;
		default:
			LOG(INFO) << "setDimNames called for unsupported type:" << robj.sexp_type() << std::endl;
			break;
	}
	return array;
}


void
RC2::EnvironmentWatcher::setDimNames ( RObject& robj, json& jobj )
{
	Rcpp::IntegerVector dims(robj.attr("dim"));
	jobj["nrow"] = dims[0];
	jobj["ncol"] = dims[1];
	if (dims.length() > 2) {
		jobj["dims"] = rvectorToJsonArray(robj);
	}
	if (robj.hasAttribute("dimnames")) {
		RObject mlist(robj.attr("dimnames"));
		json dnames;
		for (int i=0; i < LENGTH(robj); i++) {
			RObject cl(VECTOR_ELT(mlist, i));
			dnames.push_back(rvectorToJsonArray(cl));
		}
		jobj["dimnames"] = dnames;
	}
}

