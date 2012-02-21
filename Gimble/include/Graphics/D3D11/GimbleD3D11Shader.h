
#ifndef _GIMBLE_D3D11SHADER_
#define _GIMBLE_D3D11SHADER_

#include "GimbleReqs.h"
#include "Graphics/D3D11/GimbleD3D11Reqs.h"
#include "Graphics/GimbleShader.h"

namespace Gimble
{
	namespace Graphics
	{
		class D3D11VertexShader : public VertexShader
		{
		private:
			ID3D10Blob *mVertexBlob;
			ID3D11VertexShader *mShader;

		public:
			D3D11VertexShader(ID3D10Blob *vertexBlob, ID3D11VertexShader *vertex);
			~D3D11VertexShader();

			inline ID3D10Blob *getBinary() const { return mVertexBlob; }
			inline ID3D11VertexShader *getShader() const { return mShader; }
		};

		class D3D11PixelShader : public PixelShader
		{
		private:
			ID3D11PixelShader *mShader;

		public:
			D3D11PixelShader(ID3D11PixelShader *shader);
			~D3D11PixelShader();

			inline ID3D11PixelShader *getShader() const { return mShader; }
		};

		class D3D11GeometryShader : public GeometryShader
		{
		private:
			ID3D11GeometryShader *mShader;

		public:
			D3D11GeometryShader(ID3D11GeometryShader *shader);
			~D3D11GeometryShader();

			inline ID3D11GeometryShader *getShader() const { return mShader; }
		};

		class D3D11HullShader : public HullShader
		{
		private:
			ID3D11HullShader *mShader;

		public:
			D3D11HullShader(ID3D11HullShader *shader);
			~D3D11HullShader();

			inline ID3D11HullShader *getShader() const { return mShader; }
		};

		class D3D11DomainShader : public DomainShader
		{
		private:
			ID3D11DomainShader *mShader;

		public:
			D3D11DomainShader(ID3D11DomainShader *shader);
			~D3D11DomainShader();

			inline ID3D11DomainShader *getShader() const { return mShader; }
		};
	}
}

#endif