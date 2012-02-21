
#include "GimbleMatrix.h"
#include "GimbleQuaternion.h"
#include "GimbleVector3.h"

#include "GimbleResourceManager.h"
#include "GimbleProfiler.h"
#include "Game/GimbleEngine.h"
#include "Game/GimbleApp.h"
#include "Graphics/GimbleBuffer.h"
#include "Graphics/GimbleColor.h"
#include "Graphics/GimbleDepthStencil.h"
#include "Graphics/GimbleDepthStencilState.h"
#include "Graphics/GimbleInputLayout.h"
#include "Graphics/GimbleMaterial.h"
#include "Graphics/GimbleMesh.h"
#include "Graphics/GimbleModel.h"
#include "Graphics/GimbleMultiRenderTarget.h"
#include "Graphics/GimblePrimitiveTopology.h"
#include "Graphics/GimbleRenderSystem.h"
#include "Graphics/GimbleRenderTarget.h"
#include "Graphics/GimbleShader.h"
#include "Graphics/GimbleShaderResourceGroup.h"
#include "Graphics/GimbleSampler.h"
#include "Input/GimbleInputSystem.h"

using namespace Gimble;

struct VSMATRIX2
{
	Matrix World;
	Matrix ViewProj;
};

Graphics::Model *createFSQuad(Graphics::RenderSystem *rsys)
{
	Graphics::InputLayout *layout = rsys->createInputLayout();
	layout->add(Graphics::VERTEXELEMENT_FLOAT3, Graphics::VERTEXELEMENTSEMANTIC_POSITION, 0, 0, 0);
	layout->add(Graphics::VERTEXELEMENT_FLOAT2, Graphics::VERTEXELEMENTSEMANTIC_TEXTURECOORDS, 0, 0, sizeof(float)*3);

	float data[] = 
	{
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f
	};

	int idata[] = { 0, 3, 2, 2, 1, 0 };//{ 0, 1, 2, 2, 3, 0 };

	Graphics::Buffer *vbuf = rsys->createBuffer(Graphics::Buffer::BUFFERTYPE_VERTEX, Graphics::Buffer::BUFFERUSAGE_STATIC, sizeof(float) * 20, sizeof(float) * 5, &data);
	Graphics::Buffer *ibuf = rsys->createBuffer(Graphics::Buffer::BUFFERTYPE_INDEX, Graphics::Buffer::BUFFERUSAGE_STATIC, sizeof(int) * 6, sizeof(int), &idata);

	Graphics::Mesh *mesh = new Graphics::Mesh("Quad", ibuf);
	std::vector<Graphics::Mesh*> meshes;
	meshes.push_back(mesh);

	return new Graphics::Model(layout, vbuf, meshes);
}

class RenderDevApp : public Game::App
{
private:
	Graphics::RenderSystem *mRender;
	Input::InputSystem *mInput;

	Graphics::DepthStencil *mDepthStencil;
	Graphics::DepthStencilState *mDepthState;
	std::map<string, Graphics::Material *> mMats;
	std::map<string, Graphics::ShaderResourceGroup*> mMatGroups;
	Graphics::Model *mModel;
	Graphics::VertexShader *mVShader;
	Graphics::PixelShader *mPShader;
	Graphics::Sampler *mSampler;
	Graphics::Buffer *mMatBuffer;

	Matrix mWorld, mView, mProj;
	Vector3 mCamPosition;
	float mRotX, mRotY;

	Profiler mProfiler;

public:
	bool init()
	{
		mRender = Game::Engine::getPtr()->getRenderSystem();

		mInput = Game::Engine::getPtr()->getInputSystem();
		mInput->setMouseLocked(true);

		mDepthStencil = mRender->createDepthStencil(mRender->getWidth(), mRender->getHeight(), Graphics::PIXELFORMAT_R32_TYPELESS, Graphics::PIXELFORMAT_D32_FLOAT, Graphics::PIXELFORMAT_R32_FLOAT);

		mDepthState = mRender->createDepthStencilState();
		mRender->setDepthStencilState(*mDepthState);

		mModel = (Graphics::Model *)ResourceManager::getPtr()->get("assets/hor/hor.gbm", typeid(Graphics::Model));

		if (!mModel)
			return false;

		for (int i = -1; ++i < mModel->getMeshCount();)
		{
			string name = mModel->getMesh(i)->getName();
			Graphics::Material *mat = (Graphics::Material*)ResourceManager::getPtr()->get("assets/hor/" + name + ".gbmt", typeid(Graphics::Material));
			if (mat) 
			{
				mMats[name] = mat;
				Graphics::ShaderResourceGroup *group = mRender->createShaderResourceGroup(1);
				group->set(mat->getTexture("DiffuseMap"), 0);
				mMatGroups[name] = group;
			}
		}

		mVShader = (Graphics::VertexShader *)ResourceManager::getPtr()->get("assets/diffuse.vs", typeid(Graphics::VertexShader));
		mPShader = (Graphics::PixelShader *)ResourceManager::getPtr()->get("assets/diffuse.ps", typeid(Graphics::PixelShader));

		if (!mVShader || !mPShader) 
			return false;

		mSampler = mRender->createSampler(
			Graphics::Sampler::FILTER_COMPARISON_ANISOTROPIC,
			Graphics::Sampler::ADDRESS_WRAP,
			Graphics::Sampler::ADDRESS_WRAP,
			Graphics::Sampler::ADDRESS_WRAP,
			0.0f,
			16,
			Graphics::COMP_NEVER,
			-3.402823466e+38f,
			3.402823466e+38f,
			0, 0, 0, 0);
		mRender->setPixelSamplers(0, 1, &mSampler);

		mCamPosition = Vector3(125, 125, 125);
		mRotX = mRotY = 0;

		mWorld = Matrix::IDENTITY;
		mView = Matrix::createLookAt(mCamPosition, Vector3::ZERO, Vector3::UNIT_Y);
		mProj = Matrix::createPerspectiveFOV(Math::PI / 4.0f, (float)mRender->getWidth()/(float)mRender->getHeight(), 1.0f, 1000.0f);

		Matrix w = mWorld.transpose();
		Matrix vp = (mView * mProj).transpose();
		VSMATRIX2 mats;
		mats.World = w;
		mats.ViewProj = vp;
		
		mMatBuffer = mRender->createBuffer(Graphics::Buffer::BUFFERTYPE_CONSTANT, Graphics::Buffer::BUFFERUSAGE_DYNAMIC_WRITEONLY, sizeof(VSMATRIX2), sizeof(VSMATRIX2), &mats);

		return true;
	}

	bool update(float elapsedSeconds)
	{
		if (mInput->isKeyDown(Gimble::Input::GIKEY_ESCAPE))
			return false;

		long mdx = mInput->getMouseDX();
		long mdy = mInput->getMouseDY();

		mRotX += (mdx) * (2.5f * elapsedSeconds);
        mRotY += (mdy) * (2.5f * elapsedSeconds);

        if (mRotY < -1.0f) mRotY = -1.0f;
        if (mRotY > 1.0f) mRotY = 1.0f;

        if (mRotX > Math::PI) mRotX -= 2 * (float)Math::PI;
        if (mRotX < -Math::PI) mRotX += 2 * (float)Math::PI;

        Quaternion qY(Vector3(0, 1, 0), -mRotX);
        Quaternion qX(Vector3(1, 0, 0), -mRotY);
		Matrix r(qY * qX);

		bool w = mInput->isKeyDown(Gimble::Input::GIKEY_W);
		bool a = mInput->isKeyDown(Gimble::Input::GIKEY_A);
		bool s = mInput->isKeyDown(Gimble::Input::GIKEY_S);
		bool d = mInput->isKeyDown(Gimble::Input::GIKEY_D);

		if (w)
			mCamPosition -= (Vector3::UNIT_Z * (30 * elapsedSeconds)) * r;
		if (s)
			mCamPosition += (Vector3::UNIT_Z * (30 * elapsedSeconds)) * r;
		if (a)
			mCamPosition -= (Vector3::UNIT_X * (30 * elapsedSeconds)) * r;
		if (d)
			mCamPosition += (Vector3::UNIT_X * (30 * elapsedSeconds)) * r;

		mView = (r * Matrix::createTranslation(mCamPosition)).inverse();

		return true;
	}

	void draw()
	{
		mProfiler.startCycle();

		mRender->clear(Graphics::Color(0.25f,0.25,0.25f,1.0f), true, 1.0f, 0);

		mDepthState->setDepthEnabled(true);
		mRender->setDepthStencilState(*mDepthState);

		mRender->setVertexShader(mVShader);
		mRender->setPixelShader(mPShader);

		mRender->setInputLayout(*mModel->getInputLayout());
		mRender->setVertexBuffer(0, *mModel->getVertexBuffer());

		VSMATRIX2 *bdata = (VSMATRIX2*)mMatBuffer->map(Graphics::Buffer::BUFFERMAPTYPE_WRITE);
		bdata->World = mWorld.transpose();
		bdata->ViewProj = (mView * mProj).transpose();
		mMatBuffer->unmap();

		mRender->setVertexConstantBuffers(0, 1, &mMatBuffer);

		for (unsigned int m = 0; m < mModel->getMeshCount(); m++)
		{
			Graphics::Mesh *mesh = mModel->getMesh(m);
			
			Graphics::ShaderResourceGroup *grp = 0;
			string meshname = mesh->getName();
			std::map<string, Graphics::ShaderResourceGroup*>::const_iterator iter = mMatGroups.find(meshname);
			if (iter != mMatGroups.end()) grp = iter->second;

			if (grp)
				mRender->setPixelShaderResources(*grp);

			mRender->setIndexBuffer(*mesh->getIndexBuffer());

			unsigned int icount = mModel->getMesh(m)->getIndexCount();
			mRender->drawIndexed(icount, 0, 0);
		}

		mRender->present();

		mProfiler.endCycle();

		if (mProfiler.getCycleCount() == 5000)
		{
			std::cout << mProfiler << std::endl;
			mProfiler.reset();
		}
	}

	void shutdown()
	{

	}
};

int main(int argc, char *argv)
{
	Gimble::Game::Engine engine;
	engine.run("gimble.cfg", new RenderDevApp());
}