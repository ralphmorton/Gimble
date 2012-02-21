
#include "GimbleConfig.h"
#include "GimbleLog.h"

namespace Gimble
{
	string Config::mDefault = "";

	Config::Config(string configFile) : mCfgPath(configFile)
	{
	
	}

	Config::~Config()
	{
	
	}

	void Config::addValueFromLine(string &line)
	{
		uint epos = line.find("=");
		if (epos == string::npos) return; //Not a valid config line
	
		string key = line.substr(0, epos);
		string val = line.substr(epos + 1);
		setValue(key, val);
	}

	string Config::getValue(const string &key) const
	{
		std::map<string,string>::const_iterator iter = mVals.find(key);
		if (iter == mVals.end()) return mDefault;

		return iter->second;
	}

	int Config::getValueAsInt(const string &key) const
	{
		string val = getValue(key);

		if (val=="") return 0;

		return StringUtils::toInt(val);
	}

	float Config::getValueAsFloat(const string &key) const
	{
		string val = getValue(key);

		if (val=="") return 0.0f;

		return StringUtils::toFloat(val);
	}

	bool Config::getValueAsBool(const string &key) const
	{
		string val = getValue(key);

		if (val=="") return false;

		return StringUtils::toBool(val);
	}

	void Config::setValue(const string &key, const string &value)
	{
		std::map<string,string>::iterator iter = mVals.find(key);
		if (iter != mVals.end())
		{
			iter->second = value;
			return;
		}

		mVals.insert(std::pair<string,string>(key, value));
	}

	bool Config::save()
	{
		std::ofstream of;
		of.open(mCfgPath.c_str(), std::ios::trunc);
		if (!of.is_open()) 
		{
			Log::getPtr()->warning("Could not save config to file [" + mCfgPath + "]");
			return false; //Cockup
		}
		else 
			Log::getPtr()->info("Saved config to file [" +mCfgPath + "]");

		for (std::map<string,string>::iterator iter = mVals.begin(); iter != mVals.end(); iter++)
			of << iter->first << "=" << iter->second << "\n";

		of.close();
		return true; //ok
	}

	Config *Config::load(const string &cfgPath)
	{
		std::ifstream f(cfgPath.c_str());
		if (!f.is_open()) 
		{
			Log::getPtr()->warning("Could not load config from file [" + cfgPath + "]");
			return 0; //Couldnt open config file
		}
		else 
			Log::getPtr()->info("Loaded config from file [" + cfgPath + "]");

		Config *cfg = new Config(cfgPath);

		string line;
		while (!f.eof())
		{
			std::getline(f,line);
			cfg->addValueFromLine(line);
		}
		f.close();

		return cfg;
	}
}