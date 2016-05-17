// demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "jsonparse.h"
#include <string>
#include <iostream>

using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "../debug/jsonparse.lib")
#else
#pragma comment(lib, "../release/jsonparse.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	//somebody's infos
	string strJson = R"({"name":"swift", "age":35, "sex":"male", "phones":["+8613011111111","+8613022222222","+8613033333333"])";
	strJson += R"(, "friends":[{"name":"mikel", "age":33, "sex":"male", "phones":["+8613311111111", "+8613322222222"]},{"name":"lili", "age":25, "sex":"female", "phones":["+8613611111111", "+8613622222222"]}]})";
	//strJson += R"(, "friends":[{"name":"mikel", "age":33, "sex":"male"},{"name":"lili", "age":25, "sex":"female"}]})";

	//parse json
	bool bOk = false;
	jsonparse parse(strJson, bOk);
	if (!bOk){
		cout << "Parse json fail! json string: " << strJson << endl;
		return -1;
	}

	//get base infos
	string name, sex;
	int64_t age;
	bOk = parse.GetString("name", name);
	if (bOk) cout << "name:\t" << name << endl; else cout << "parse \"name\" error!" << endl;
	bOk = parse.GetString("sex", sex);
	if (bOk) cout << "sex:\t" << sex << endl; else cout << "parse \"sex\" error!" << endl;
	bOk = parse.GetInt("age", age);
	if (bOk) cout << "age:\t" << age << endl; else cout << "parse \"age\" error!" << endl;
	
	//get phones
	vector<string> vePhones;
	bOk = parse.GetStringArray("phones", vePhones);
	if (bOk){
		cout << "phones:\t";
		for (auto itm : vePhones)
			cout << itm << "\t";
		cout << endl;
	}
	else {
		cout << "parse \"phones\" error!" << endl;
	}

	//get friends
	vector<string> frdname, frdsex, frdphones;
	vector<int64_t> frdage;
	parse.GetStringArray(jsonparse::MakeKeys("friends", "name"), frdname);
	parse.GetStringArray(jsonparse::MakeKeys("friends", "sex"), frdsex);
    parse.GetIntArray(jsonparse::MakeKeys("friends", "age"), frdage);

	cout << "friends:" << endl;
	for (size_t i = 0; i < frdname.size(); i++){
		cout << "\tname:\t" << frdname[i] << endl;
		cout << "\tsex:\t" << frdsex[i] << endl;
		cout << "\tage:\t" << frdage[i] << endl;

		char szIndex[10] = { 0 };
		itoa((int)i, szIndex, 10);
		parse.GetStringArray(jsonparse::MakeKeys("friends", szIndex, "phones"), frdphones);
		
		cout << "\tphones:\t";
		for (size_t j = 0; j < frdphones.size(); j++){
			cout << frdphones[j] << "\t";
		}
		cout << endl;

		cout << endl;
	}

	return 0;
}

