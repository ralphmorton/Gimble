
#include "Graphics/GimbleMaterial.h"
#include "Graphics/GimbleTexture.h"

namespace Gimble
{
	namespace Graphics
	{
		string Material::mTypeName = "Material";

		Material::Material()
		{

		}

		Material::~Material()
		{

		}

		Texture *Material::getTexture(const string &name)
		{
			std::map<string, Texture*>::const_iterator iter = mTextures.find(name);
			return iter == mTextures.end() ? 0 : iter->second;
		}

		float Material::getFloat(const string &name)
		{
			std::map<string, float>::const_iterator iter = mFloats.find(name);
			return iter == mFloats.end() ? 0 : iter->second;
		}

		Vector2 Material::getVector2(const string &name)
		{
			std::map<string, Vector2>::const_iterator iter = mVec2s.find(name);
			return iter == mVec2s.end() ? Vector2::ZERO : iter->second;
		}

		Vector3 Material::getVector3(const string &name)
		{
			std::map<string, Vector3>::const_iterator iter = mVec3s.find(name);
			return iter == mVec3s.end() ? Vector3::ZERO : iter->second;
		}

		Vector4 Material::getVector4(const string &name)
		{
			std::map<string, Vector4>::const_iterator iter = mVec4s.find(name);
			return iter == mVec4s.end() ? Vector4::ZERO : iter->second;
		}

		void Material::setTexture(const string &name, Texture *val)
		{
			mTextures[name] = val;
		}

		void Material::setFloat(const string &name, float val)
		{
			mFloats[name] = val;
		}

		void Material::setVector2(const string &name, Vector2 val)
		{
			mVec2s[name] = val;
		}

		void Material::setVector3(const string &name, Vector3 val)
		{
			mVec3s[name] = val;
		}

		void Material::setVector4(const string &name, Vector4 val)
		{
			mVec4s[name] = val;
		}

		bool Material::haveTexture(const string &name)
		{
			std::map<string, Texture*>::const_iterator iter = mTextures.find(name);
			return !(iter == mTextures.end());
		}

		bool Material::haveFloat(const string &name)
		{
			std::map<string, float>::const_iterator iter = mFloats.find(name);
			return !(iter == mFloats.end());
		}

		bool Material::haveVector2(const string &name)
		{
			std::map<string, Vector2>::const_iterator iter = mVec2s.find(name);
			return !(iter == mVec2s.end());
		}

		bool Material::haveVector3(const string &name)
		{
			std::map<string, Vector3>::const_iterator iter = mVec3s.find(name);
			return !(iter == mVec3s.end());
		}

		bool Material::haveVector4(const string &name)
		{
			std::map<string, Vector4>::const_iterator iter = mVec4s.find(name);
			return !(iter == mVec4s.end());
		}
	}
}