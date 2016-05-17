#include "jsonparse.h"
#include <stdio.h>

static string JoinKeys(const vector<string>& veKeys)
{
	string strKeys = "";
	for (auto k : veKeys){
		strKeys += k;
		strKeys += "/";
	}
	return strKeys;
}

jsonparse::jsonparse(const string& strJson, bool& bOk) :m_bParseOk(false)
{
	JValue val;
	char *endptr;
	JsonValue value;
	JsonAllocator allocator;

	string strJson_ = strJson;
	bOk = m_bParseOk = (JSON_OK == jsonParse((char*)strJson_.c_str(), &endptr, &value, allocator));
	if (m_bParseOk)
	{
		vector<string> keys;
		ParseObject(value, keys);
		printf("jsonparse ok! json string: %s\n", strJson.c_str());
	}
	else
	{
		printf("jsonparse fail! json string: %s\n", strJson.c_str());
	}
}

void jsonparse::ParseObject(JsonValue o, vector<string>& veKeys) 
{
	JValue val;
	switch (o.getTag()) 
	{
	case JSON_NUMBER:
		printf("key: %s\n", JoinKeys(veKeys).c_str());
		printf("value: %g\n", o.toNumber());
		val.vType = flag_number;
		val.fValue = o.toNumber();
		m_mpValues.insert(make_pair(JoinKeys(veKeys), val));
		break;
	case JSON_STRING:
		printf("key: %s\n", JoinKeys(veKeys).c_str());
		printf("value: %s\n", o.toString());
		val.vType = flag_string;
		val.strValue = o.toString();
		m_mpValues.insert(make_pair(JoinKeys(veKeys), val));
		break;
	case JSON_ARRAY:
		{
			int iIndex = 0;
			for (auto i : o)
			{
				char szBuf[10] = { 0 };
				itoa(iIndex, szBuf, 10);
				veKeys.push_back(szBuf);
				ParseObject(i->value, veKeys);
				veKeys.pop_back();
				iIndex++;
			}

			//remember array size
			printf("key: %s\n", JoinKeys(veKeys).c_str());
			printf("value: %d\n", iIndex);
			val.vType = flag_number;
			val.fValue = (double)iIndex;
			m_mpValues.insert(make_pair(JoinKeys(veKeys), val));
		}
		break;
	case JSON_OBJECT:
		for (auto i : o) 
		{
			veKeys.push_back(i->key);
			ParseObject(i->value, veKeys);
			veKeys.pop_back();
		}
		break;
	case JSON_TRUE:
		printf("key: %s\n", JoinKeys(veKeys).c_str());
		printf("value: true\n");
		val.vType = flag_boolean;
		val.bValue = true;
		m_mpValues.insert(make_pair(JoinKeys(veKeys), val));
		break;
	case JSON_FALSE:
		printf("key: %s\n", JoinKeys(veKeys).c_str());
		printf("value: false\n");
		val.vType = flag_boolean;
		val.bValue = false;
		m_mpValues.insert(make_pair(JoinKeys(veKeys), val));
		break;
	case JSON_NULL:
		printf("key: %s\n", JoinKeys(veKeys).c_str());
		printf("value: null\n");
		val.vType = flag_null;
		m_mpValues.insert(make_pair(JoinKeys(veKeys), val));
		break;
	}
}

JValue jsonparse::GetValue(const vector<string>& veKeys)
{
	JValue val;
	if (!m_bParseOk)
		return val;
	multimap<string, JValue>::iterator it;
	it = m_mpValues.find(JoinKeys(veKeys));
	if (it == m_mpValues.end())
		return val;
	val = it->second;

	return val;
}

vector<JValue> jsonparse::GetValueArray(const vector<string>& veKeys)
{
	bool bOk = false;
	int64_t iUbound = 0;
	vector<JValue> val;
	vector<string> veKeys_;

	if (!m_bParseOk)
		return val;

	//is it the style ? ==> {"arr":["v1","v2"]}
	//calling: GetValueArray("arr", veArr), return ["v1","v2"];
	bOk = GetInt(veKeys, iUbound);
	if (bOk){
		veKeys_ = veKeys;
		for (int i = 0; i < (int)iUbound; i++){
			char szIndex[10] = { 0 };
			itoa(i, szIndex, 10);
			veKeys_.push_back(szIndex);
			JValue v = GetValue(veKeys_);
			if (v.vType != flag_null)
				val.push_back(v);
			veKeys_.pop_back();
		}

		return val;
	}

	//is it the style ? ==> {"arr":[{"k1":"v1","k2":"v2"}, {"k1":"v11","k2":"v22"}]}
	//calling: GetValueArray(MakeKeys("arr","k1"), veArr)), return ["v1","v11"];
	if (veKeys.size() < 2)
		return val;
	veKeys_ = veKeys;
	veKeys_.pop_back();
	bOk = GetInt(veKeys_, iUbound);
	if (bOk){
		string strLastKey = veKeys.at(veKeys.size() - 1);
		for (int i = 0; i < (int)iUbound; i++){
			char szIndex[10] = { 0 };
			itoa(i, szIndex, 10);
			veKeys_.push_back(szIndex);
			veKeys_.push_back(strLastKey);
			JValue v = GetValue(veKeys_);
			if (v.vType != flag_null)
				val.push_back(v);
			veKeys_.pop_back();
			veKeys_.pop_back();
		}

		return val;
	}

	return val;
}

vector<string> jsonparse::MakeKeys(const string& key1, const string& key2/* = ""*/, const string& key3/* = ""*/, const string& key4/* = ""*/, const string& key5/* = ""*/,
	const string& key6/* = ""*/, const string& key7/* = ""*/, const string& key8/* = ""*/, const string& key9/* = ""*/, const string& key10/* = ""*/)
{
	vector<string> v;
	if (key1.length() > 0) v.push_back(key1);
	if (key2.length() > 0) v.push_back(key2);
	if (key3.length() > 0) v.push_back(key3);
	if (key4.length() > 0) v.push_back(key4);
	if (key5.length() > 0) v.push_back(key5);
	if (key6.length() > 0) v.push_back(key6);
	if (key7.length() > 0) v.push_back(key7);
	if (key8.length() > 0) v.push_back(key8);
	if (key9.length() > 0) v.push_back(key9);
	if (key10.length() > 0) v.push_back(key10);
	return v;
}

//get integer value
bool jsonparse::GetInt(const char* key, int64_t& i64Value)
{
	return GetInt(jsonparse::MakeKeys(key), i64Value);
}
bool jsonparse::GetInt(const vector<string>& veKeys, int64_t& i64Value)
{
	JValue vl = GetValue(veKeys);
	if (vl.vType != flag_number)
		return false;
	i64Value = (int64_t)vl.fValue;
	return true;
}

//get float value
bool jsonparse::GetFloat(const char* key, double& fValue)
{
	return GetFloat(jsonparse::MakeKeys(key), fValue);
}
bool jsonparse::GetFloat(const vector<string>& veKeys, double& fValue)
{
	JValue vl = GetValue(veKeys);
	if (vl.vType != flag_number)
		return false;
	fValue = vl.fValue;
	return true;
}

//get string value
bool jsonparse::GetString(const char* key, string& strValue)
{
	return GetString(jsonparse::MakeKeys(key), strValue);
}
bool jsonparse::GetString(const vector<string>& veKeys, string& strValue)
{
	JValue vl = GetValue(veKeys);
	if (vl.vType != flag_string)
		return false;
	strValue = vl.strValue;
	return true;
}

//get bool value
bool jsonparse::GetBool(const char* key, bool& bValue)
{
	return GetBool(jsonparse::MakeKeys(key), bValue);
}
bool jsonparse::GetBool(const vector<string>& veKeys, bool& bValue)
{
	JValue vl = GetValue(veKeys);
	if (vl.vType != flag_boolean)
		return false;
	bValue = vl.bValue;
	return true;
}


bool jsonparse::GetIntArray(const char* key, vector<int64_t>& veValue)
{
	return GetIntArray(jsonparse::MakeKeys(key), veValue);
}
bool jsonparse::GetIntArray(const vector<string>& veKeys, vector<int64_t>& veValue)
{
	veValue.clear();
	vector<JValue> ve = GetValueArray(veKeys);
	for (auto itm : ve)
	{
		if (itm.vType == flag_number)
			veValue.push_back((int64_t)itm.fValue);
		else{}
	}
	return (veValue.size() > 0);
}

bool jsonparse::GetFloatArray(const char* key, vector<double>& veValue)
{
	return GetFloatArray(jsonparse::MakeKeys(key), veValue);
}
bool jsonparse::GetFloatArray(const vector<string>& veKeys, vector<double>& veValue)
{
	veValue.clear();
	vector<JValue> ve = GetValueArray(veKeys);
	for (auto itm : ve)
	{
		if (itm.vType == flag_number)
			veValue.push_back(itm.fValue);
		else{}
	}
	return (veValue.size() > 0);
}

bool jsonparse::GetStringArray(const char* key, vector<string>& veValue)
{
	return GetStringArray(jsonparse::MakeKeys(key), veValue);
}
bool jsonparse::GetStringArray(const vector<string>& veKeys, vector<string>& veValue)
{
	veValue.clear();
	vector<JValue> ve = GetValueArray(veKeys);
	for (auto itm : ve)
	{
		if (itm.vType == flag_string)
			veValue.push_back(itm.strValue);
		else{}
	}
	return (veValue.size() > 0);
}

bool jsonparse::GetBoolArray(const char* key, vector<bool>& veValue)
{
	return GetBoolArray(jsonparse::MakeKeys(key), veValue);
}
bool jsonparse::GetBoolArray(const vector<string>& veKeys, vector<bool>& veValue)
{
	veValue.clear();
	vector<JValue> ve = GetValueArray(veKeys);
	for (auto itm : ve)
	{
		if (itm.vType == flag_boolean)
			veValue.push_back(itm.bValue);
		else{}
	}
	return (veValue.size() > 0);
}
