
#ifndef _GIMBLE_MATERIAL_
#define _GIMBLE_MATERIAL_

#include "GimbleReqs.h"
#include "GimbleResource.h"
#include "GimbleVector2.h"
#include "GimbleVector3.h"
#include "GimbleVector4.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API Material : public Resource
		{
		private:
			static string mTypeName;

		private:
			std::map<string, Texture*> mTextures;
			std::map<string, float> mFloats;
			std::map<string, Vector2> mVec2s;
			std::map<string, Vector3> mVec3s;
			std::map<string, Vector4> mVec4s;

		public:
			Material();
			~Material();

			Texture *getTexture(const string &name);
			float getFloat(const string &name);
			Vector2 getVector2(const string &name);
			Vector3 getVector3(const string &name);
			Vector4 getVector4(const string &name);

			void setTexture(const string &name, Texture *val);
			void setFloat(const string &name, float val);
			void setVector2(const string &name, Vector2 val);
			void setVector3(const string &name, Vector3 val);
			void setVector4(const string &name, Vector4 val);

			bool haveTexture(const string &name);
			bool haveFloat(const string &name);
			bool haveVector2(const string &name);
			bool haveVector3(const string &name);
			bool haveVector4(const string &name);

			inline const std::type_info &getType() { return typeid(Material); }
			inline const string &getTypeName() { return mTypeName; }
		};
	}
}

#endif