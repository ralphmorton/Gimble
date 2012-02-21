
#ifndef _GIMBLE_SHADER_
#define _GIMBLE_SHADER_

#include "GimbleReqs.h"
#include "GimbleResource.h"

namespace Gimble
{
	namespace Graphics
	{
		class GIMBLE_API VertexShader : public Resource
		{
		private:
			static string mTypeName;
			
		public:
			virtual ~VertexShader() {}

			inline const std::type_info &getType() { return typeid(VertexShader); }
			inline const string &getTypeName() { return mTypeName; }
		};

		class GIMBLE_API PixelShader : public Resource
		{
		private:
			static string mTypeName;
			
		public:
			virtual ~PixelShader() {}

			inline const std::type_info &getType() { return typeid(PixelShader); }
			inline const string &getTypeName() { return mTypeName; }
		};

		class GIMBLE_API GeometryShader : public Resource
		{
		private:
			static string mTypeName;
			
		public:
			virtual ~GeometryShader() {}

			inline const std::type_info &getType() { return typeid(GeometryShader); }
			inline const string &getTypeName() { return mTypeName; }
		};

		class GIMBLE_API HullShader : public Resource
		{
		private:
			static string mTypeName;
			
		public:
			virtual ~HullShader() {}

			inline const std::type_info &getType() { return typeid(HullShader); }
			inline const string &getTypeName() { return mTypeName; }
		};

		class GIMBLE_API DomainShader : public Resource
		{
		private:
			static string mTypeName;
			
		public:
			virtual ~DomainShader() {}

			inline const std::type_info &getType() { return typeid(DomainShader); }
			inline const string &getTypeName() { return mTypeName; }
		};
	}
}

#endif