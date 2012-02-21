
#ifndef _GIMBLE_CONFIG_
#define _GIMBLE_CONFIG_

#include "GimbleReqs.h"
#include "GimbleStringUtils.h"

namespace Gimble
{
	class GIMBLE_API Config
	{
	private:
		static string mDefault;

	private:
		string mCfgPath;
		std::map<string,string> mVals;

	private:
		Config(string configFile);
		void addValueFromLine(string &line);

	public:
		~Config();

		string getValue(const string &key) const;
		int getValueAsInt(const string &key) const;
		float getValueAsFloat(const string &key) const;
		bool getValueAsBool(const string &key) const;

		void setValue(const string &key, const string &val);

		bool save();

		static Config *load(const string &cfgPath);
	};
}

#endif