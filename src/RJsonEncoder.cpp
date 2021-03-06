#include <vector>
#include <map>
#include <sys/time.h>
#include "RJsonEncoder.hpp"
//#include "g3log/loglevels.hpp"
//#include "RC2Logging.h"
#include "../common/RC2Utils.hpp"


const int kMaxEnvDepth = 1;
const int kMaxLen = 100;
const char *kNotAVector = "notAVector";
const char *kPrimitive = "primitive";
const char *kClass = "class";
const char *kValue = "value";
const char *kName = "name";
const char *kType = "type";
const char *vNoType = "-";

namespace RC2 {
	inline bool isOrderedFactor(Rcpp::StringVector& classNames, RObject& robj) {
		return classNames.length() > 1 && classNames[0] == "ordered" && classNames[1] == "factor";
	}
	
	std::string
	columnType(RObject& robj) {
		if (Rf_isFactor(robj)) {
			Rcpp::StringVector classVal(robj.attr(kClass));
			if (isOrderedFactor(classVal, robj)) {
				return "of";
			} else {
				return "f";
			}
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
	
	json
	basicColumnValues(RObject &colValue, int rowCount) {
		if(rowCount < 1) { return json::array(); }
		switch(colValue.sexp_type()) {
			case LGLSXP: {
				// Rcpp returns logical values as ints of value 0 or 1, not bool
				Rcpp::LogicalVector bvector(colValue);
				json jvalues;
				for (int i = 0; i < bvector.length(); i++) {
					if (Rcpp::LogicalVector::is_na(bvector[i])) {
						jvalues.push_back(json(nullptr));
					} else {
						jvalues.push_back(json(bvector[i] == 1));
					}
				}
				return jvalues;
			}
			case INTSXP: {
				Rcpp::IntegerVector ivector(colValue);
				json jvalues;
				for (int i = 0; i < ivector.length(); i++) {
					if (Rcpp::IntegerVector::is_na(ivector[i])) {
						jvalues.push_back(json(nullptr));
					} else {
						jvalues.push_back(json(ivector[i]));
					}
				}
				return jvalues;
			}
			case REALSXP: {
				Rcpp::NumericVector dvals(colValue);
				json jvalues;
				for (int i=0; i < dvals.length(); i++) {
					// This is the only test that really works. See https://stackoverflow.com/questions/26241085/rcpp-function-check-if-missing-value/26262984#26262984
					if (R_IsNA(dvals[i])) {
						jvalues.push_back(json(nullptr));
					} else {
						double d = dvals[i];
						if (d == R_NaN || std::isnan(d)) { jvalues.push_back("NaN"); }
						else if (d == R_NegInf || d == -std::numeric_limits< double >::infinity()) { jvalues.push_back("-Inf"); }
						else if (d == R_PosInf || std::isinf(d)) { jvalues.push_back("Inf"); }
						else { jvalues.push_back(d); }
					}
				}
				return jvalues;
			}
			case CPLXSXP:
			case STRSXP: {
				Rcpp::StringVector svals(colValue);
				json jvalues;
				for (int i = 0; i < svals.length(); i++) {
					if (Rcpp::StringVector::is_na(svals[i])) {
						jvalues.push_back(json(nullptr));
					} else {
						jvalues.push_back(json(svals[i]));
					}
				}
				return jvalues;
			}
			default:
//				LOG_INFO << "dataframe invalid col type:" << colValue.sexp_type() << std::endl;
				return json(nullptr);
		}
		
	}
		
	json RJsonEncoder::toJson ( Rcpp::RObject& robj, bool includeSummaries ) {
		json results;
		valueToJson("xxx", robj, results, true, includeSummaries);
		return results;
	}

	void
	RC2::RJsonEncoder::setDimNames ( RObject& robj, json& jobj )
	{
		Rcpp::IntegerVector dims(robj.attr("dim"));
		jobj["nrow"] = dims[0];
		jobj["ncol"] = dims[1];
		if (dims.length() > 2) {
			jobj["dims"] = rvectorToJsonArray(robj);
		}
		if (robj.hasAttribute("dimnames")) {
			Rcpp::List mlist(robj.attr("dimnames"));
			json dnames;
			for (int i=0; i < mlist.length(); i++) {
				RObject cl(mlist[i]);
				dnames.push_back(rvectorToJsonArray(cl));
			}
			jobj["dimnames"] = dnames;
		}
	}
	
	void
	RC2::RJsonEncoder::addSummary(std::string& varName, json& jobj)
	{
		if (varName.length() < 1) return;
		std::string vname = varName;
		StripQuotes(vname);
		std::string cmd = "capture.output(str(" + vname + "))";
		RObject result;
		_execCallback(cmd, result);
		if (result.isNULL()) {
//			LOG_INFO << "summary result " << varName << " is null" << std::endl;
			return;
		}
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
//			LOG_INFO << "exception for summary of " << varName << std::endl;
		}
	}

	void
	RC2::RJsonEncoder::valueToJson (std::string varName, RObject& robj, json& jobj, bool includeListChildren, bool includeSummaries )
	{
		int depth = 2;
		valueToJsonInternal(varName, robj, jobj, includeListChildren, includeSummaries, depth);
	}
	
	void RC2::RJsonEncoder::valueToJsonInternal(std::string varName, RObject& robj, json& jobj, bool includeListChildren, bool includeSummaries, int& envDepthLeft)
	{
		jobj[kName] = varName;
		if (Rf_isObject(robj)) {
			setObjectData(robj, jobj);
			if (includeSummaries)
				addSummary(varName, jobj);
			return;
		}
		switch(robj.sexp_type()) {
			case LISTSXP: // 1
				setPairListData(robj, jobj);
				break;
			case VECSXP: // 19
				setListData(robj, jobj, includeListChildren);
				if (includeSummaries)
					addSummary(varName, jobj);
				break;
			case ENVSXP:
				setEnvironmentData(robj, jobj, envDepthLeft);
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
	RC2::RJsonEncoder::setPairListData(Rcpp::RObject& robj, json& jobj)
	{
		std::string emptyStr("");
		// Rcpp discards the names when a pairlist is converted to a vector.
		// couldn't figure out how to get names via Rcpp::PairList, so
		// manually walk through using lower-level R api
		SEXP s = robj;
		std::vector<std::string> names;
		while (TYPEOF(s) == LISTSXP) {
			json child;
			if (TAG(s) != R_NilValue && Rf_isSymbol(TAG(s))) {
				Rcpp::Symbol tagS(TAG(s));
				names.push_back(tagS.c_str());
			} else {
				names.push_back("<NA>");
			}
			s = CDR(s);
		}
		// attaching names to robj wasn't working when passed to setListData.
		// works when jumping through these hoops. Not sure why, but it works.
		Rcpp::List slist(robj);
		Rcpp::StringVector snames(names.size());
		snames = names;
		slist.attr("names") = snames;
		RObject sobj((SEXP)slist);
		
		setListData(sobj, jobj, true);
		jobj[kClass] = "pairlist";
	}
	
	void
	RC2::RJsonEncoder::setListData ( RObject& robj, json& jobj, bool includeListChildren )
	{
		Rcpp::List list(robj);
		int len = list.length();
		jobj[kClass] = "list";
		Rcpp::StringVector names;
		if (robj.hasAttribute("names")) {
			names = robj.attr("names");
		}
		jobj["names"] = names;
		jobj["length"] = len;
		jobj[kType] = vNoType;
		std::string emptyStr;
		if (includeListChildren) {
			json children;
			int maxLen = len < kMaxLen ? len : kMaxLen;
			for (int i=0; i < maxLen; i++) {
				RObject aChild(list[i]);
				json childObj;
				valueToJson(emptyStr, aChild, childObj, false);
				if (names.size() > 0) {
					childObj[kName] = names[i];
				}
				children.push_back(childObj);
			}
			jobj[kValue] = children;
		}
	}
	
	void 
	RC2::RJsonEncoder::setObjectData ( RObject& robj, json& jobj )
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
			snprintf(datebuf, 16, "%4d-%02d-%02d", date.getYear(), date.getMonth(), date.getDay());
			jobj[kValue] = datebuf;
			jobj[kType] = "date";
		} else if (jobj[kClass] == "POSIXct") {
			jobj[kValue] = REAL(robj)[0];
			jobj[kType] = "date";
		} else if (jobj[kClass] == "POSIXlt") {
			Rcpp::Datetime dt(robj);
			jobj[kValue] = dt.getFractionalTimestamp();
			jobj[kType] = "date";
		}  else if (jobj[kClass] == "data.frame") {
			setDataFrameData(robj, jobj);
		} else {
			setGenericObjectData(robj, jobj);
		}
	}
	
	void 
	RC2::RJsonEncoder::setFactorData ( RObject& robj, json& jobj )
	{
		jobj[kType] = "f";
		Rcpp::StringVector levs(robj.attr("levels"));
		jobj["levels"] = levs;
		jobj[kValue] = Rcpp::IntegerVector(robj);
	}
	
	void 
	RC2::RJsonEncoder::setDataFrameData ( RObject& robj, json& jobj )
	{
		jobj[kType] = "d";
		int colCount = LENGTH(robj);
		jobj["ncol"] = colCount;
		RObject rowList(robj.attr("row.names"));
		if (LENGTH(rowList) > 0) {
			jobj["row.names"] = Rcpp::StringVector(rowList);
		}
		json columns;
		int rowCount = 0;
		Rcpp::StringVector colNames(robj.attr("names"));
		for (int i=0; i < colCount; i++) {
			RObject element(VECTOR_ELT(robj, i));
			rowCount = LENGTH(element);
			json aCol;
			std::string aType = columnType(element);
			aCol[kType] = aType;
			if(aType == "of" || aType == "f") {
				aCol["levels"] = Rcpp::StringVector(element.attr("levels"));
			}
			aCol["values"] = basicColumnValues(element, rowCount);
			aCol["name"] = colNames[i];
			columns.push_back(aCol);
		}
		jobj["columns"] = columns;
		jobj["nrow"] = rowCount;
	}
	
	void
	RC2::RJsonEncoder::setFunctionData ( RObject& robj, json& jobj )
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
//			LOG_INFO << "failed to get function body:" << e.what() << std::endl;
		} 
	}
	
	void
	RC2::RJsonEncoder::setGenericObjectData ( RObject& robj, json& jobj )
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
	RC2::RJsonEncoder::setEnvironmentData ( RObject& robj, json& jobj, int& depthCount )
	{
		json childArray;
		Rcpp::Environment cenv(robj);
		if (--depthCount > 0) {
			Rcpp::StringVector cnames(cenv.ls(false));
			int len = cnames.size() < kMaxLen ? cnames.size() : kMaxLen;
			for (int i=0; i < len; i++) {
				json cobj;
				std::string varName(cnames[i]);
				cobj[kName] = varName;
				RObject childVal(cenv.get(varName));
				valueToJsonInternal(varName, childVal, cobj, true, true, depthCount);
				childArray.push_back(cobj);
			}
		}
		jobj[kClass] = "environment";
		jobj[kValue] = childArray;
	}
	
	void 
	RC2::RJsonEncoder::setPrimitiveData ( RObject& robj, json& jobj )
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
			case STRSXP: { //16
				jobj[kClass] = "string";
				jobj[kType] = "s";
				jobj[kValue] = Rcpp::StringVector(robj);
			}
			break;
			case CPLXSXP:
				jobj[kClass] = "complex";
				jobj[kType] = "c";
				jobj[kValue] = Rcpp::StringVector(robj);
				break;
			case RAWSXP: //24
				jobj[kClass] = "raw";
				jobj[kType] = "r";
				{
					//				Rcpp::RawVector raw(robj);
					//				std::vector<char> bytes = 
					//				jobj[kValue] = raw.asStdVectorInt();
				}
				break;
			case SYMSXP: //not actually a primitive type, but handle here for convience
			{
				Rcpp::Symbol sym(robj);
				jobj[kName] = sym.c_str();
			}
			jobj[kClass] = "symbol";
			notVector = true;
			jobj[kPrimitive] = false;
			break;
			default:
				jobj[kClass] = "unsupported type";
				notVector = true;
				jobj[kPrimitive] = false;
//				LOG_INFO  << "unsupported type:" << robj.sexp_type() << std::endl;
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
	RC2::RJsonEncoder::rvectorToJsonArray ( RObject& robj )
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
//				LOG_INFO << "setDimNames called for unsupported type:" << robj.sexp_type() << std::endl;
				break;
		}
		return array;
	}
		
};
