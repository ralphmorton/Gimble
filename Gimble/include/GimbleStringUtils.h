
#ifndef _GIMBLE_STRINGUTILS_
#define _GIMBLE_STRINGUTILS_

#include "GimbleReqs.h"

namespace Gimble
{
	class StringUtils
	{
	public:
		static std::vector<string> *split(const string &input, const string &separator, bool stripEmptyEntries = true)
		{
			std::vector<string> *list = new std::vector<string>();
			int p = input.find(separator);
			if (p == string::npos)
			{
				if (stripEmptyEntries && input.length() == 0) return list;

				list->push_back(input);
				return list;
			}
			
			int sl = input.length();
			string fp = input.substr(0, p);
			if (fp.length() > 0) list->push_back(fp);
			
			while (p != string::npos && sl > p + 1)
			{
				int np = input.find(separator, p + 1);
				int nlen = np == string::npos ? sl - p + 1 : np - p - 1;

				if (!stripEmptyEntries || nlen > 0)
				{
					string rp = input.substr(p+1, nlen);
					list->push_back(rp);
				}
				
				p = input.find(separator, p + 1);
			}

			return list;
		}

		static int toInt(const string &input)
		{
			int res = atoi(input.c_str());
			return res;
		}

		static float toFloat(const string &input)
		{
			return (float)atof(input.c_str());
		}

		static bool toBool(const string &input)
		{
			return 
				input == "1" || 
				input == "true" || 
				input == "True" || 
				input == "TRUE" || 
				input == "yes" || 
				input == "Yes" || 
				input == "y" ||  
				input == "Y" ? true : false;
		}

		static string fromInt(int input)
		{
			char output[12];
			_itoa_s(input,output,10);

			string ret(output);

			return ret;
		}
	};
}

#endif