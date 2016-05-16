#pragma once
#include <stdint.h>
#include <string.h>
#include <vector>
#include <map>
#include "gason.h"

using namespace std;

enum tflag
{
	flag_number,
	flag_string,
	flag_boolean,
	flag_null
};

typedef struct _JValue
{
	double		fValue;
	string		strValue;
	bool		bValue;
	tflag vType;

	_JValue()
		:fValue(0.0), strValue(""), bValue(false), vType(flag_null)
	{} 
}JValue;

class jsonparse
{
public:
	jsonparse(const string& strJson, bool& bOk);

	//Demo json: {"k1": "v1", "k2":[{"k21":"v21", "k22":22, "k23":true},{"k21":"v222", "k22":223, "k23":false}], "k3":{"k31":"v31", "k32":32}}
	//GetString("k1", str) ==> v1
	//GetString(MakeKeys("k3","k31"), str) ==> v31
	//GetInt(MakeKeys("k3","k32"), num) ==> 32
	//
	//GetIntArray(MakeKeys("k2","k22"), veNums) ==> vector([22,223])
	//GetBoolArray(MakeKeys("k2","k23"), veBools) ==> vector([true,false])

	bool GetInt(const char* key, int64_t& i64Value);
	bool GetInt(const vector<string>& veKeys, int64_t& i64Value);
	bool GetFloat(const char* key, double& dValue);
	bool GetFloat(const vector<string>& veKeys, double& dValue);
	bool GetString(const char* key, string& strValue);
	bool GetString(const vector<string>& veKeys, string& strValue);
	bool GetBool(const char* key, bool& bValue);
	bool GetBool(const vector<string>& veKeys, bool& bValue);

	bool GetIntArray(const char* key, vector<int64_t>& veValue);
	bool GetIntArray(const vector<string>& veKeys, vector<int64_t>& veValue);
	bool GetFloatArray(const char* key, vector<double>& veValue);
	bool GetFloatArray(const vector<string>& veKeys, vector<double>& veValue);
	bool GetStringArray(const char* key, vector<string>& veValue);
	bool GetStringArray(const vector<string>& veKeys, vector<string>& veValue);
	bool GetBoolArray(const char* key, vector<bool>& veValue);
	bool GetBoolArray(const vector<string>& veKeys, vector<bool>& veValue);

	static vector<string> MakeKeys(const string& key1, const string& key2 = "", const string& key3 = "", const string& key4 = "", const string& key5 = "",
		const string& key6 = "", const string& key7 = "", const string& key8 = "", const string& key9 = "", const string& key10 = "");

private:
	void ParseObject(JsonValue obj, vector<string>& veKeys);
	JValue GetValue(const vector<string>& veKeys);
	vector<JValue> GetValueArray(const vector<string>& veKeys);

	bool m_bParseOk;
	multimap<string, JValue> m_mpValues;
};

