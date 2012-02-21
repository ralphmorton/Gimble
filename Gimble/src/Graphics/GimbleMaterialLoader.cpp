
#include "Graphics/GimbleMaterialLoader.h"
#include "GimbleResourceManager.h"
#include "GimbleResourceStream.h"
#include "GimbleStringUtils.h"
#include "GimbleLog.h"

#include <algorithm>

namespace Gimble
{
	namespace Graphics
	{
		void MaterialLoader::read(string resid, string type, string name, string val, Material *mat)
		{
			if (type == "texture")
			{
				val.erase(std::remove(val.begin(), val.end(), '\r'), val.end());
				Texture *tex = (Texture*)ResourceManager::getPtr()->get(val, typeid(Texture));

				if (!tex)
					Log::getPtr()->warning("Could not load texture at [" + val + "] in material [" + resid + "]");
				else
					mat->setTexture(name, tex);
			}
			else if (type == "float")
			{
				float fval = StringUtils::toFloat(val);
				mat->setFloat(name, fval);
			}
			else if (type == "float2")
			{
				std::vector<string> *fparts = StringUtils::split(val, " ", true);
				if (fparts->size() == 2)
				{
					float f1 = StringUtils::toFloat((*fparts)[0]);
					float f2 = StringUtils::toFloat((*fparts)[1]);
					mat->setVector2(name, Vector2(f1, f2));
				}
				else
					Log::getPtr()->warning("Could not read float2 value [" + val + "] in material [" + resid + "]");

				delete fparts;
			}
			else if (type == "float3")
			{
				std::vector<string> *fparts = StringUtils::split(val, " ", true);
				if (fparts->size() == 3)
				{
					float f1 = StringUtils::toFloat((*fparts)[0]);
					float f2 = StringUtils::toFloat((*fparts)[1]);
					float f3 = StringUtils::toFloat((*fparts)[2]);
					mat->setVector3(name, Vector3(f1, f2, f3));
				}
				else
					Log::getPtr()->warning("Could not read float3 value [" + val + "] in material [" + resid + "]");

				delete fparts;
			}
			else if (type == "float4")
			{
				std::vector<string> *fparts = StringUtils::split(val, " ", true);
				if (fparts->size() == 4)
				{
					float f1 = StringUtils::toFloat((*fparts)[0]);
					float f2 = StringUtils::toFloat((*fparts)[1]);
					float f3 = StringUtils::toFloat((*fparts)[2]);
					float f4 = StringUtils::toFloat((*fparts)[3]);
					mat->setVector4(name, Vector4(f1, f2, f3, f4));
				}
				else
					Log::getPtr()->warning("Could not read float4 value [" + val + "] in material [" + resid + "]");

				delete fparts;
			}
			else
				Log::getPtr()->warning("Unknown material parameter type [" + type + "] in material [" + resid + "]");
		}

		Material *MaterialLoader::load(ResourceStream &stream)
		{
			int len = stream.getSize();
			char *data = new char[len];
			stream.read(data, 0, len);

			std::stringstream in(data, std::ios::in);
			string line;

			Material *mat = new Material();

			while (!in.eof())
			{
				std::getline(in, line);

				std::vector<string> *parts = StringUtils::split(line, ":", true);

				if (parts->size() > 1)
				{
					string type = (*parts)[0];
					std::vector<string> *parts2 = StringUtils::split((*parts)[1], "=", true);

					if (parts2->size() > 1)
					{
						string name = (*parts2)[0];
						string path = (*parts2)[1];
						read(stream.getResourceID(), type, name, path, mat);
					}
					else
						Log::getPtr()->warning("Could not parse material parm in line [" + line + "] in material [" + stream.getResourceID() + "]");

					delete parts2;
				}

				delete parts;
			}

			return mat;
		}
	}
}