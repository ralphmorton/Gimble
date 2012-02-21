
#include "GimbleApp.h"
#include "Graphics/GimbleRenderSystem.h"
#include "Graphics/GimbleComputeShader.h"
#include "Graphics/GimbleMultiBuffer.h"

using namespace Gimble;

const UINT NUM_ELEMENTS = 1024;

struct BufType
{
    int i;
    float f;
};

BufType gBuf1[NUM_ELEMENTS];
BufType gBuf2[NUM_ELEMENTS];

class ComputeShaderTestApp : public App 
{
public:
	bool init()
	{
		for (int i = 0; i < NUM_ELEMENTS; ++i) 
		{
			gBuf1[i].i = i;
			gBuf1[i].f = (float)i;
			gBuf2[i].i = i;
			gBuf2[i].f = (float)i;
		}

		Graphics::ComputeShader *cs = (Graphics::ComputeShader*)ResourceManager::getPtr()->get("compute.hlsl", typeid(Graphics::ComputeShader));

		Graphics::Buffer *buf1 = mRender->createBuffer(Graphics::Buffer::BUFFERTYPE_COMPUTE_STRUCTURED_UNORDERED, Graphics::Buffer::BUFFERUSAGE_DYNAMIC, NUM_ELEMENTS*sizeof(BufType), sizeof(BufType), &gBuf1);
		Graphics::Buffer *buf2 = mRender->createBuffer(Graphics::Buffer::BUFFERTYPE_COMPUTE_STRUCTURED_UNORDERED, Graphics::Buffer::BUFFERUSAGE_DYNAMIC, NUM_ELEMENTS*sizeof(BufType), sizeof(BufType), &gBuf2);
		Graphics::Buffer *bufout = mRender->createBuffer(Graphics::Buffer::BUFFERTYPE_COMPUTE_STRUCTURED_UNORDERED, Graphics::Buffer::BUFFERUSAGE_DYNAMIC, NUM_ELEMENTS*sizeof(BufType), sizeof(BufType), 0);

		Graphics::MultiBuffer srvs;
		srvs.add(buf1);
		srvs.add(buf2);
		Graphics::MultiBuffer uavs;
		uavs.add(bufout);
		Graphics::MultiBuffer cbufs;
		
		cs->run(srvs, uavs, cbufs, NUM_ELEMENTS, 1, 1);

		BufType* data = (BufType*)bufout->map(Graphics::Buffer::BUFFERMAPTYPE_READ);
		
        for ( int i = 0; i < NUM_ELEMENTS; ++i )
            if (data[i].i != 0)
				std::cout << data[i].i << std::endl;

		bufout->unmap();

		return true;
	}

	bool update(float elapsedSeconds)
	{
		return true;
	}

	void draw()
	{

	}

	void shutdown()
	{

	}
};