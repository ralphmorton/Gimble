
#include "Script\GimbleLuaScriptEngine.h"
#include "Script\GimbleScriptFunction.h"
#include "Script\GimbleScriptClass.h"
#include "GimbleMutex.h"
#include "GimbleThread.h"
#include "Game/GimbleApp.h"
#include "GimbleLog.h"
#include "Game/GimbleEngine.h"
#include "Graphics/GimbleColor.h"
#include "Graphics/GimbleDepthStencil.h"
#include "Graphics/GimbleDepthStencilState.h"
#include "Graphics/GimbleRenderSystem.h"
#include "Graphics/GimbleShader.h"
#include "Graphics/GimbleSampler.h"
#include "Graphics/GimblePrimitiveTopology.h"
#include "Graphics/GimbleModel.h"
#include "Graphics/GimbleMesh.h"
#include "GimbleResourceManager.h"
#include "GimbleMatrix.h"
#include "Graphics/GimbleComputeShader.h"
#include "Graphics/GimbleMultiBuffer.h"
#include "Physics/GimbleSimulator.h"
#include "Physics/GimbleBoxShape.h"
#include "Physics/GimbleSphereShape.h"
#include "Physics/GimbleCollisionObject.h"
#include "Physics/GimbleRigidBody.h"
#include "GimbleTransform.h"
#include "GimbleQuaternion.h"
#include "GimbleVector3.h"
#include "GimbleMatrix.h"
#include "GimbleLinkedList.h"
#include "Physics/GimbleSAP.h"
#include "GimbleTimer.h"
#include "GimbleProfiler.h"
#include "Input/GimbleInputSystem.h"
#include "Physics/GimbleRevoluteJoint.h"
#include "Graphics/GimbleMaterial.h"

using namespace Gimble;

struct VSMATRIX2
{
	Matrix World;
	Matrix ViewProj;
};

void testfunc(Gimble::string str)
{
	std::cout << str << std::endl;
};

class PhysicsTestApp : public Game::App
{
private:
	float mTotal;
	float mAccum;

	Graphics::DepthStencil *mDepthStencil;
	Graphics::DepthStencilState *mDepthState;
	Graphics::RenderSystem *mRender;
	Graphics::VertexShader *mVShader;
	Graphics::PixelShader *mPShader;
	Graphics::Buffer *mMatBuffer;
	Graphics::Model *mModel, *mModelSphere;

	Physics::Simulator *mSim;
	Physics::CollisionObject *mFloor;
	Physics::BoxShape *mBoxShape;
	Physics::SphereShape *mSphereShape;
	Physics::RigidBody **mBodies;
	Physics::Joint **mJoints;
	Physics::RigidBody *mIslandCrasher, *mIslandCrasher2;
	bool mIslandCrasherReleased, mIslandCrasherReleased2;
	uint mNumBodies, mNumJoints;

	Matrix mView, mProj;
	Vector3 mCamPosition;
	float mRotX, mRotY;

	Profiler mProfiler;

	Input::InputSystem *mInput;

public:
	PhysicsTestApp() : mTotal(0), mAccum(0), mFloor(0), mJoints(0), mBodies(0), mNumBodies(0), mNumJoints(0) { }

	bool init()
	{
		Log::getPtr()->info("PhysicsTestApp initialising");

		//Script::ScriptEngine *engine = Game::Engine::getPtr()->getScriptEngine();
		//Script::ScriptFunction1<void, Gimble::string> *func = new Script::ScriptFunction1<void, Gimble::string>("testfunc", testfunc);
		//engine->registerFunction(func);
		//engine->run("testfunc('this is a test');");

		mRender = Game::Engine::getPtr()->getRenderSystem();
		mRender->setPrimitiveTopology(Graphics::PRIMITIVETOPOLOGY_TRIANGLELIST);

		mDepthStencil = mRender->createDepthStencil(mRender->getWidth(), mRender->getHeight(), Graphics::PIXELFORMAT_R32_TYPELESS, Graphics::PIXELFORMAT_D32_FLOAT, Graphics::PIXELFORMAT_R32_FLOAT);

		mDepthState = mRender->createDepthStencilState();
		mRender->setDepthStencilState(*mDepthState);

		ResourceManager *resman = ResourceManager::getPtr();

		mVShader = (Graphics::VertexShader*)resman->get("test.vs", typeid(Graphics::VertexShader));
		mPShader = (Graphics::PixelShader*)resman->get("test.ps", typeid(Graphics::PixelShader));

		if (!mVShader || !mPShader) 
			return false;
		
		VSMATRIX2 mats;
		mMatBuffer = mRender->createBuffer(Graphics::Buffer::BUFFERTYPE_CONSTANT, Graphics::Buffer::BUFFERUSAGE_DYNAMIC_WRITEONLY, sizeof(VSMATRIX2), sizeof(VSMATRIX2), &mats);

		mInput = Game::Engine::getPtr()->getInputSystem();
		mInput->setMouseLocked(true);

		mModel = (Graphics::Model *)resman->get("cube.gbm", typeid(Graphics::Model));
		mModelSphere = (Graphics::Model *)resman->get("sphere.gbm", typeid(Graphics::Model));

		mSim = Physics::Simulator::getPtr();
		mSim->setGravity(-Vector3::UNIT_Y * 10.f);
		mSim->setCollisionBiasFactor(0.2f);
		//mSim->setIterations(20);
		mBoxShape = new Physics::BoxShape();
		mSphereShape = new Physics::SphereShape();

		Transform tfloor(Quaternion::IDENTITY, Vector3(0,-5.f,0), Vector3(100,10,100));
		mFloor = new Physics::CollisionObject(tfloor, mBoxShape);
		mSim->addCollisionObject(mFloor);

		mNumBodies = 1000;
		mBodies = new Physics::RigidBody*[mNumBodies];
		mNumJoints = 25;
		mJoints = new Physics::Joint*[mNumJoints];
		
		mIslandCrasher = new Physics::RigidBody(mBoxShape, Transform(Quaternion::IDENTITY, Vector3(0, 100, 1), Vector3(4,4,4)), 200.0f, 0.1f, 0.4f, 0.1f, 0.1f);
		mIslandCrasherReleased = false;

		mIslandCrasher2 = new Physics::RigidBody(mBoxShape, Transform(Quaternion::IDENTITY, Vector3(0, 200, 0), Vector3(4,4,4)), 300.0f, 0.1f, 0.4f, 0.1f, 0.1f);
		mIslandCrasherReleased2 = false;

		float zs[] = { -0.1f, 0.0f, 0.1f };
		uint zcnt = 0;

		uint numAct = 0;
		float x = 20.0f;
        for (int i = 0; i < 10; i++)
        {
            float hx = x / 2.0f;
            if (hx >= 0.5f)
            {
                for (float j = -hx; j < hx; j += 2.001f)
                {
					Physics::RigidBody *box = new Physics::RigidBody(mBoxShape, Transform(Quaternion::IDENTITY, Vector3(j, 1.0f + (i * 2), 0), Vector3(2,2,2)), 50.0f, 0.1f, 0.4f, 0.0f, 0.0f);
					box->setActive(false);
					mBodies[numAct++] = box;
                }
            }
            x -= 2.0f;
        }

		uint numActJ = 0;
		float startY = 7;
        float spacing = 1.25f;
        float y = startY - spacing;
        float halfspacing = spacing / 2.0f;
        Physics::RigidBody *lastBody = 0;
        for (int i = 0; i < 12; i++)
        {
            Physics::RigidBody *r = new Physics::RigidBody(mBoxShape, Transform(Quaternion::IDENTITY, Vector3(y, 20, 8), Vector3(1,1,1)), 25.0f, 0.1f, 0.4f, 0.0f, 0.0f);
			mBodies[numAct++] = r;

            if (i > 0)
            {
				Physics::RevoluteJoint *j = new Physics::RevoluteJoint(lastBody, r, Vector3(y + halfspacing, 20, 8), 0.0f, 0.2f);
                mJoints[numActJ++] = j;
            }

            lastBody = r;

            y -= spacing;
        }

		Physics::RigidBody *sp1 = new Physics::RigidBody(mSphereShape, Transform(Quaternion::IDENTITY, Vector3(0, 10, 6), Vector3(3,3,3)), 50.0f, 0.25f, 0.4f, 0.15f, 0.15f);
		Physics::RigidBody *sp2 = new Physics::RigidBody(mSphereShape, Transform(Quaternion::IDENTITY, Vector3(3, 25, 6), Vector3(2,2,2)), 50.0f, 0.25f, 0.4f, 0.15f, 0.15f);
		sp2->applyImpulse(Vector3(0, -1000, 0), Vector3::ZERO);
		mBodies[numAct++] = sp1;
		mBodies[numAct++] = sp2;

		mNumBodies = numAct;
		for (int i = -1; ++i < numAct;) mSim->addBody(mBodies[i]);
		mNumJoints = numActJ;
		for (int i = -1; ++i < numActJ;) mSim->addJoint(mJoints[i]);

		mCamPosition = Vector3(30, 30, 30);
		mRotX = mRotY = 0;

		mView = Matrix::createLookAt(mCamPosition, Vector3::ZERO, Vector3::UNIT_Y);
		mProj = Matrix::createPerspectiveFOV(Math::PI / 4.0f, (float)mRender->getWidth()/(float)mRender->getHeight(), 1.0f, 1000.0f);

		return true;
	}

	bool update(float elapsedSeconds)
	{
		if (mInput->isKeyDown(Gimble::Input::GIKEY_ESCAPE))
			return false;

		mTotal += elapsedSeconds;
		mAccum += elapsedSeconds;

		if (mTotal > 10 && !mIslandCrasherReleased)
		{
			mSim->addBody(mIslandCrasher);
			mIslandCrasherReleased = true;
		}

		if (mTotal > 20 && !mIslandCrasherReleased2)
		{
			mSim->addBody(mIslandCrasher2);
			mIslandCrasherReleased2 = true;
		}

		if (mAccum >= 0.01f)
		{
			mProfiler.startCycle();
			mSim->step(mAccum);
			mAccum = 0.0f;
			mProfiler.endCycle();

			if (mProfiler.getCycleCount() == 75)
			{
				std::cout << mProfiler << std::endl;
				mProfiler.reset();
			}
		}

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
		mRender->clear(Graphics::Color(1.f, 1.f, 1.f, 1), true, 1.0f, 0);

		Vector3 cd(0.0f, -10.0f, -20.0f);

		mDepthState->setDepthEnabled(true);
		mRender->setDepthStencilState(*mDepthState);

		mRender->setVertexShader(mVShader);
		mRender->setPixelShader(mPShader);

		if (mModel && mModelSphere)
		{
			for (int i = -1; ++i < mNumBodies;)
			{
				Transform *t = mBodies[i]->getTransform();
				Matrix world = t->getMat();
				
				VSMATRIX2 *bdata = (VSMATRIX2*)mMatBuffer->map(Graphics::Buffer::BUFFERMAPTYPE_WRITE);
				bdata->World = world.transpose();
				bdata->ViewProj = (mView * mProj).transpose();
				mMatBuffer->unmap();
				mRender->setVertexConstantBuffers(0, 1, &mMatBuffer);

				Graphics::Model *mod = mBodies[i]->getShape() == mBoxShape ? mModel : mModelSphere;

				mRender->setInputLayout(*mod->getInputLayout());
				mRender->setVertexBuffer(0, *mod->getVertexBuffer());

				for (unsigned int m = 0; m < mod->getMeshCount(); m++)
				{
					mRender->setIndexBuffer(*mod->getMesh(m)->getIndexBuffer());
					unsigned int icount = mod->getMesh(m)->getIndexCount();
					mRender->drawIndexed(icount, 0, 0);
				}
			}

			mRender->setInputLayout(*mModel->getInputLayout());
			mRender->setVertexBuffer(0, *mModel->getVertexBuffer());

			Transform *tc = mIslandCrasher->getTransform();

			VSMATRIX2 *bdata = (VSMATRIX2*)mMatBuffer->map(Graphics::Buffer::BUFFERMAPTYPE_WRITE);
			bdata->World = tc->getMat().transpose();
			bdata->ViewProj = (mView * mProj).transpose();
			mMatBuffer->unmap();
			mRender->setVertexConstantBuffers(0, 1, &mMatBuffer);

			for (unsigned int m = 0; m < mModel->getMeshCount(); m++)
			{
				mRender->setIndexBuffer(*mModel->getMesh(m)->getIndexBuffer());
				unsigned int icount = mModel->getMesh(m)->getIndexCount();
				mRender->drawIndexed(icount, 0, 0);
			}

			tc = mIslandCrasher2->getTransform();
			
			bdata = (VSMATRIX2*)mMatBuffer->map(Graphics::Buffer::BUFFERMAPTYPE_WRITE);
			bdata->World = tc->getMat().transpose();
			bdata->ViewProj = (mView * mProj).transpose();
			mMatBuffer->unmap();
			mRender->setVertexConstantBuffers(0, 1, &mMatBuffer);

			for (unsigned int m = 0; m < mModel->getMeshCount(); m++)
			{
				mRender->setIndexBuffer(*mModel->getMesh(m)->getIndexBuffer());
				unsigned int icount = mModel->getMesh(m)->getIndexCount();
				mRender->drawIndexed(icount, 0, 0);
			}

			Transform *tf = mFloor->getTransform();
			
			bdata = (VSMATRIX2*)mMatBuffer->map(Graphics::Buffer::BUFFERMAPTYPE_WRITE);
			bdata->World = tf->getMat().transpose();
			bdata->ViewProj = (mView * mProj).transpose();
			mMatBuffer->unmap();
			mRender->setVertexConstantBuffers(0, 1, &mMatBuffer);

			for (unsigned int m = 0; m < mModel->getMeshCount(); m++)
			{
				mRender->setIndexBuffer(*mModel->getMesh(m)->getIndexBuffer());
				unsigned int icount = mModel->getMesh(m)->getIndexCount();
				mRender->drawIndexed(icount, 0, 0);
			}
		}

		mRender->present();
	}

	void shutdown()
	{
		delete mFloor;
		for (int i = -1; ++i < mNumBodies; delete mBodies[i]);;
		delete mBodies;
		for (int i = -1; ++i < mNumJoints; delete mJoints[i]);;
		delete mJoints;

		Log::getPtr()->info("PhysicsTestApp shutting down");
	}
};