//currently using cajun 2.0.2

#include <math.h>
#include <sys/time.h>
#include <Rcpp.h>
#include <Rinternals.h>
#include "json/reader.h"
#include "json/writer.h"
#include "json/elements.h"

#define kMaxLen 100

static void valueToJson(Rcpp::RObject val, json::Object &obj, bool includeListChildren=false);
static void handleGenericObject(Rcpp::RObject &val, json::Object &obj);


static json::Array rvector2jsonArray(Rcpp::RObject robj)
{
	json::Array outArray;
	SEXP sexp = robj;
	int len = LENGTH(sexp);
	if (len > kMaxLen)
		len = kMaxLen;
	switch(robj.sexp_type()) {
		case LGLSXP:
			for (int i=0; i < len; i++)
				outArray.Insert(json::Boolean(LOGICAL(sexp)[i]));
		case INTSXP: //13
			for (int i=0; i < len; i++)
				outArray.Insert(json::Number(INTEGER(sexp)[i]));
			break;
		case REALSXP: //13
			for (int i=0; i < len; i++)
				outArray.Insert(json::Number(REAL(sexp)[i]));
			break;
		case STRSXP:
			for (int i=0; i < len; i++) {
				Rcpp::StringVector strV(STRING_ELT(sexp, i));
				outArray.Insert(json::String((char*)strV[0]));
			}
			break;
		case NILSXP:
			outArray.Insert(json::Null());
			break;
		default:
			std::cerr << "rvector2jsonArray called for unsupported type:" << robj.sexp_type() << std::endl;
			break;
	}
	return outArray;
}

static void setDimNames(json::Object &obj, Rcpp::RObject &robj)
{
	Rcpp::IntegerVector dims(robj.attr("dim"));
	obj["nrow"] = json::Number(dims[0]);
	obj["ncol"] = json::Number(dims[1]);
	if (dims.length() > 2) //for arrays, we return the third dimension length, too
		obj["dims"] = rvector2jsonArray(dims);
	if (robj.hasAttribute("dimnames")) {
		SEXP masterlist = robj.attr("dimnames");
		json::Array dnames;
		for (int k=0; k < LENGTH(masterlist); k++) {
			SEXP ml = VECTOR_ELT(masterlist, k);
			Rcpp::RObject cl(ml);
			dnames.Insert(rvector2jsonArray(cl));
		}
		obj["dimnames"] = dnames;
	}
}

static void setFunctionData(json::Object &obj, Rcpp::RObject & robj)
{
	using namespace std;
	using namespace Rcpp;
	try {
		Environment env("package:rc2");
		Function fun = env["rc2.defineFunction"];
		RObject val = fun(robj);
		json::Array lines;
		
		StringVector strvec(val);
		ostringstream stream;
		for(int i=0; i < strvec.size(); ++i) {
			stream << (string)strvec[i] << endl;
		}
		obj["body"] = json::String(stream.str().c_str());
	} catch (std::exception &e) {
		std::cerr << "failed to get function body:" << std::endl;
	}
}

static void setPrimitiveData(json::Object& obj, int stype, Rcpp::RObject& robj)
{
	std::string clz, dtype;
	bool prim=true, notAVector=false;
	json::Array values;
	SEXP sexp = robj;
	int len = LENGTH(sexp);
	if (len > kMaxLen)
		len = kMaxLen;
	switch (stype) {
		case NILSXP: //0
			clz = "NULL"; 
			dtype = "n";
			notAVector = true;
			break;
		case LGLSXP: //10
			clz = "logical"; 
			dtype = "b";
			for (int i=0; i < len; i++)
				values.Insert(json::Boolean(LOGICAL(sexp)[i]));
			break;
		case INTSXP: //13
			clz = "integer vector"; 
			dtype = "i";
			for (int i=0; i < len; i++)
				values.Insert(json::Number(INTEGER(sexp)[i]));
			break;
		case REALSXP: //14
			clz =  "numeric vector"; 
			dtype = "d";
			for (int i=0; i < len; i++) {
				double dval = REAL(sexp)[i];
				if (isnan(dval))
					values.Insert(json::String("NaN"));
				else if (isinf(dval))
					values.Insert(json::String(std::signbit(dval) ? "-Inf" : "Inf"));
				else
					values.Insert(json::Number(REAL(sexp)[i]));
			}
			break;
		case STRSXP: //16
			clz = "string"; 
			dtype = "s";
			for (int i=0; i < len; i++) {
				Rcpp::StringVector strV(STRING_ELT(sexp, i));
				values.Insert(json::String((char*)strV[0]));
			}
			break;
		case CPLXSXP:
			clz = "complex";
			dtype = "c";
			sexp = Rf_coerceVector(sexp, STRSXP);
			for (int i=0; i < len; i++) {
				Rcpp::StringVector strV(STRING_ELT(sexp, i));
				values.Insert(json::String((char*)strV[0]));
			}
			break;
		case RAWSXP: //24
			clz = "raw";
			dtype = "r";
			break;
		case VECSXP: //19
			clz = "list"; 
			break;
		case ENVSXP: //4
			clz = "environment";
			notAVector = true;
			break;
		case CLOSXP: //3
		case SPECIALSXP: //7
		case BUILTINSXP: //8
			clz = "function";
			notAVector = true;
			prim = false;
			setFunctionData(obj, robj);
			break;
		default:
			clz = "unsupported type";
			notAVector = true;
			std::cout << "unsupported type:" << stype << std::endl;
			prim = false;
	}
	if (notAVector)
		obj["notAVector"] = json::Boolean(true);
	else
		obj["length"] = json::Number(LENGTH(sexp));
	obj["primitive"] = json::Boolean(prim);
	obj["class"] = json::String(clz);
	obj["type"] = json::String(dtype);
	obj["value"] = values;
	if (Rf_isArray(sexp)) {
		obj["primitive"] = json::Boolean(false);
		setDimNames(obj, robj);
		if (Rf_isMatrix(sexp))
			obj["class"] = json::String("matrix");
		else
			obj["class"] = json::String("array");
	}
}

static json::Array factorLevels(Rcpp::RObject &val)
{
	Rcpp::StringVector levelsVal((SEXP)val.attr("levels"));
	json::Array levels;
	for (int i=0; i < levelsVal.length(); i++) {
		levels.Insert(json::String((char*)levelsVal[i]));
	}
	return levels;
}

static void setFactorData(Rcpp::RObject &val, json::Object &obj)
{
	obj["type"] = json::String("f");
	obj["levels"] = factorLevels(val);
	SEXP sexp = val;
	json::Array values;
	for (int i=0; i < LENGTH(sexp); i++) {
		values.Insert(json::Number(INTEGER(sexp)[i]));
	}
	obj["value"] = values;
}

static void setDatetimeData(Rcpp::RObject &val, json::Object &obj)
{
	int len = LENGTH(val);
	if (len != 9) {
		std::cerr << "got POSIXlt object without length of 9" << std::endl;
		return;
	}
	struct tm dvals;
	bzero(&dvals, sizeof(dvals));
	dvals.tm_sec = REAL(VECTOR_ELT(val, 0))[0];
	dvals.tm_min = INTEGER(VECTOR_ELT(val, 1))[0];
	dvals.tm_hour = INTEGER(VECTOR_ELT(val, 2))[0];
	dvals.tm_mday = INTEGER(VECTOR_ELT(val, 3))[0];
	dvals.tm_mon = INTEGER(VECTOR_ELT(val, 4))[0];
	dvals.tm_year = INTEGER(VECTOR_ELT(val, 5))[0];
	dvals.tm_wday = INTEGER(VECTOR_ELT(val, 6))[0];
	dvals.tm_yday = INTEGER(VECTOR_ELT(val, 7))[0];
	dvals.tm_isdst = INTEGER(VECTOR_ELT(val, 8))[0];
	obj["value"] = json::Number(timegm(&dvals));
}

static void setDataFrameData(Rcpp::RObject &val, json::Object &obj)
{
	using namespace json;
	using namespace Rcpp;
	using namespace std;
	int colCount = LENGTH(val);
	SEXP colList = val.attr("names");
	Rcpp::RObject cno(Rf_coerceVector(colList, STRSXP));
	Array colNames = rvector2jsonArray(cno);
	obj["cols"] = colNames;
	obj["ncol"] = Number(colCount);
	
	SEXP rowList = val.attr("row.names");
	if (LENGTH(rowList) > 0) {
		RObject rnames(rowList);
		obj["row.names"] = rvector2jsonArray(rnames);
	}

	Array colTypes;
	std::vector<RObject> colObjs;
	for (int i=0; i < colCount; i++) {
		SEXP sexp = VECTOR_ELT(val, i);
		//coerce complex types to string
		if (TYPEOF(sexp) == CPLXSXP)
			sexp = Rf_coerceVector(sexp, STRSXP);
		RObject o(sexp);
		colObjs.push_back(o);
		Boolean alreadySet=false;
		if (Rf_isFactor(sexp)) {
			StringVector clVal((SEXP)o.attr("class"));
			if (0 == strcmp(clVal[0], "factor")) {
				colTypes.Insert(json::String("f"));
			} else if (clVal.length() > 1 && 0 == strcmp(clVal[0], "ordered") && 
				0 == strcmp(clVal[1], "factor"))
			{
				colTypes.Insert(json::String("of"));
			}
			char namebuf[16];
			sprintf(namebuf, "f%d", i);
			obj[namebuf] = factorLevels(o);
		}
		if (!alreadySet) {
			switch(TYPEOF(sexp)) {
				case LGLSXP:
					colTypes.Insert(json::String("b")); break;
				case INTSXP:
					colTypes.Insert(json::String("i")); break;
				case REALSXP:
					colTypes.Insert(json::String("d")); break;
				case STRSXP:
					colTypes.Insert(json::String("s")); break;
				default:
					colTypes.Insert(json::String("-")); break;
			}
		}
	}
	obj["types"] = colTypes;
	
	int rowCount = LENGTH(colObjs[0]);
	obj["nrow"] = Number(rowCount);
	//create robjs for each column list
	Array rows;
	for (int row=0; row < kMaxLen && row < rowCount; row++) {
		Array aRow;
		for (int col=0; col < colNames.Size(); col++) {
			SEXP asexp = colObjs[col];
			switch(colObjs[col].sexp_type()) {
				case INTSXP:
					aRow.Insert(Number(INTEGER(asexp)[row]));
					break;
				case REALSXP:
					aRow.Insert(Number(REAL(asexp)[row]));
					break;
				case STRSXP:
					{
						StringVector sv(asexp);
						aRow.Insert(json::String((char*)sv[0]));
					}
					break;
				case LGLSXP:
					aRow.Insert(Boolean(LOGICAL(asexp)[row]));
					break;
				case CPLXSXP: //should never happen
				default:
					cerr << "col type=" << colObjs[col].sexp_type() << endl;
					aRow.Insert(json::Null());
					break;
			}
		}
		rows.Insert(aRow);
	}
	obj["rows"] = rows;
}

static void setObjectData(Rcpp::RObject &val, json::Object &obj)
{
	using namespace json;
	using namespace Rcpp;
	using namespace std;
	json::String clz = obj["class"];
	if (Rf_isFactor(val))
		setFactorData(val, obj);
	else if ((std::string)clz == "Date") {
		Rcpp::Date date(val);
		char dateBuf[16];
		snprintf(dateBuf, 16, "%02d-%02d-%02d", date.getYear(), date.getMonth(), date.getDay());
		obj["value"] = json::String(dateBuf);
	} else if ((std::string)clz == "POSIXct") {
		json::Number num(REAL(val)[0]);
		obj["value"] = num;
	} else if ((std::string)clz == "POSIXlt") {
		setDatetimeData(val, obj);
	} else if ((std::string)clz == "data.frame") {
		setDataFrameData(val, obj);
	} else {
		handleGenericObject(val, obj);
	}
}

static void handleGenericObject(Rcpp::RObject &val, json::Object &obj)
{
	json::Array values;
	json::Array nameArray;
	std::vector<std::string> cAttrs = val.attributeNames();
	std::vector<std::string>::iterator itr;
	for (itr=cAttrs.begin(); itr != cAttrs.end(); itr++) {
		std::string aname = *itr;
		if (0 == aname.compare("class"))
			continue;
		nameArray.Insert(json::String(aname));
		Rcpp::RObject attrVal((SEXP)val.attr(aname));
		json::Object jobj;
		valueToJson(attrVal, jobj, true);
		jobj["name"] = json::String(aname);
		values.Insert(jobj);
	}
	obj["generic"] = json::Boolean(true);
	obj["names"] = nameArray;
	obj["value"] = values;
	obj["length"] = json::Number(values.Size());
}

static void handleList(Rcpp::RObject &val, json::Object &obj, bool includeChildren)
{
	int len = LENGTH((SEXP)val);
	obj["class"] = json::String("list");
	Rcpp::RObject names((SEXP)val.attr("names"));
	json::Array nameArray = rvector2jsonArray(names);
	obj["names"] = nameArray;
	obj["length"] = json::Number(len);
	if (includeChildren) {
		json::Array children;
		int max = len < kMaxLen ? len : kMaxLen;
		for (int i=0; i < max; i++) {
			Rcpp::RObject aChild(VECTOR_ELT(val, i));
			json::Object childObj;
			valueToJson(aChild, childObj);
			childObj["name"] = nameArray[i];
			children.Insert(childObj);
		}
		obj["value"] = children;
	}
}

static void handleEnvironment(Rcpp::RObject &val, json::Object &obj)
{
	Rcpp::Language call("ls", val);
	SEXP sr = call.eval();
	Rcpp::StringVector varNames(sr);
	json::Array childArray;
	Rcpp::Environment env(val);
	int max = varNames.size() < kMaxLen ? varNames.size() : kMaxLen;
	for (int i=0; i < max; i++) {
		json::Object cobj;
		cobj["name"] = json::String((std::string)varNames[i]);
		valueToJson(env.get((std::string)varNames[i]), cobj, true);
		childArray.Insert(cobj);
	}
	obj["value"] = childArray;
	
}

static void valueToJson(Rcpp::RObject val, json::Object &obj, bool includeListChildren)
{
	int stype = val.sexp_type();
	if (Rf_isObject(val)) {
		Rcpp::StringVector clVal((SEXP)val.attr("class"));
		if (LENGTH(clVal) > 1 && 0 == strcmp(clVal[0], "ordered") && 
			0 == strcmp(clVal[1], "factor"))
		{
			obj["class"] = json::String("ordered factor");
		} else {
			obj["class"] = json::String((char*)clVal[0]);
		}
		if (stype == S4SXP)
			obj["S4"] = json::Boolean(true);
		setObjectData(val, obj);
	} else if (stype == VECSXP) {
		handleList(val, obj, includeListChildren);
	} else if (stype == ENVSXP) {
		obj["class"] = json::String("environment");
		handleEnvironment(val, obj);
	} else {
		setPrimitiveData(obj, stype, val);
	}
}

extern "C" SEXP Rc2_SublistValue(SEXP elemName, SEXP subscripts)
{
	using namespace Rcpp;
	using namespace json;

	std::string baseName(CHAR(STRING_ELT(elemName, 0)));
	Rcpp::IntegerVector subVector(Rf_coerceVector(subscripts, INTSXP));

	Rcpp::Environment env;
	json::Object obj;

	RObject robj(env.get(baseName));
	SEXP curSexp = robj;
	for (int i=0; i < subVector.size(); i++) {
		SEXP elem = VECTOR_ELT(robj, subVector[i]-1); //convert from 1-based to 0-based
		if (NULL == elem) {
			std::cerr << "failed to get item " << subVector[i] << " of obj " << curSexp << std::endl;
			break;
		}
		std::cerr << "vector_elt finished " << TYPEOF(elem) << std::endl;
		robj = RObject(elem);
		curSexp = elem;
	}
	//now use that object
	obj["name"] = json::String(baseName);
	valueToJson(robj, obj, true);

	std::stringstream stream;
	Writer::Write(obj, stream);
	CharacterVector out;
	out.push_back(stream.str());
	return out;
}

extern "C" SEXP Rc2_ListVariables(SEXP varNames, SEXP deletedSymbols)
{
	using namespace Rcpp;
	using namespace json;

	try {
		Object dict;
		
		struct timespec startTime, endTime;
		clock_gettime(CLOCK_MONOTONIC, &startTime);

		if (!(TYPEOF(varNames) == VECSXP && LENGTH(VECTOR_ELT(varNames, 0)) == 0)) {
			Array array;
			SEXP nameVector = Rf_coerceVector(varNames, STRSXP);
			StringVector x(nameVector);
			Environment env;
			for (int i=0; i < x.length(); i++) {
				json::Object obj;
				obj["name"] = json::String((char*)x[i]);
				valueToJson(env.get((char*)x[i]), obj, true);
				array.Insert(obj);
			}
			dict["values"] = array;
		}
		if (!Rf_isNull(deletedSymbols)) {
			if (TYPEOF(deletedSymbols) != STRSXP)
				deletedSymbols = Rf_coerceVector(deletedSymbols, STRSXP);
			if (LENGTH(deletedSymbols) > 0) {
				json::Array delKeys;
				for (int i=0; i < LENGTH(deletedSymbols); i++) {
					Rcpp::StringVector strV(STRING_ELT(deletedSymbols, i));
					delKeys.Insert(json::String((char*)strV[0]));
				}
				if (delKeys.Size() > 1 || 0 != strcmp("NULL", CHAR(STRING_ELT(deletedSymbols,0))))
					dict["deleted"] = delKeys;
			}
		}

		if (dict.Size() > 0) {
			//calculate elapsed time
			clock_gettime(CLOCK_MONOTONIC, &endTime);
			struct timeval startVal, endVal, elapsedVal;
			TIMESPEC_TO_TIMEVAL(&startVal, &startTime);
			TIMESPEC_TO_TIMEVAL(&endVal, &endTime);
			timersub(&endVal, &startVal, &elapsedVal);
			unsigned long millisecs =  ((elapsedVal.tv_sec * 1000) + (elapsedVal.tv_usec / 1000));

			dict["timeElapsed"] = json::Number(millisecs);
		}
		dict["rc2!var"] = json::Number(1);
		
		std::stringstream stream;
		Writer::Write(dict, stream);
		CharacterVector out;
		out.push_back(stream.str());
		return out;
	} catch (std::exception& e) {
		std::cerr << "caught exception:" << e.what() << ", line " << __LINE__ << std::endl;
	}
	return NULL;
}
