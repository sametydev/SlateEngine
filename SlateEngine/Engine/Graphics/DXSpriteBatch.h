#pragma once
#include <SlateEngine/Engine/Math/vec4f.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderCache.h>
#include <SlateEngine/Engine/Graphics/Buffer/BufferCache.h>
#include <SlateEngine/Engine/Graphics/Vertex.h>

class Batch
{
public:
	Batch();
	virtual ~Batch();


	virtual void Init() = 0;

	//Gathering resources
	virtual void Begin() = 0;

	virtual void Render(const char* text, int x, int y,
		const vec4f& color = vec4f(1.f)) = 0;

	virtual void End() = 0;
};

#define MAX_SPRITE 8192


/*
	LT(0) ----- RT(1)
	|				|
	|				|
	LB(2) ----- RB(3)
*/


struct SpriteBatch {
	VertexPT LT, RT, LB, RB;
	DXTexture* pTexture;
};

struct BatchItem {

	BatchItem(int _offset, UINT _nVert, DXTexture* _tex)
		: offset(_offset), nVertices(_nVert), pTexture(_tex) {
	}

	int offset;    //vertex offset
	UINT nVertices; //count of vertex
	DXTexture* pTexture;
};

enum class SortType : unsigned int {
	TEXTURE, DEPTH, NUMBER
};

class DXSpriteBatch : Batch {
public:
	DXSpriteBatch();

	void Init() override;
	void Begin() override;
	void Render(const char* text, int x, int y,
		const vec4f& color = vec4f(1.f)) override;
	void End() override;

	void Render(DXTexture* tex, int x, int y, int h, int w);

	void CreateBuffer();
	void CreateBatches();
	void RenderBatches();

	static DXSpriteBatch* Instance;
	static bool TextureSort(SpriteBatch* A, SpriteBatch* B);
protected:
	void DoSort();

private:
	DXConstantBuffer* mCBO = nullptr;
	ID3D11Buffer* mVBO = nullptr;
	ID3D11Buffer* mIBO = nullptr;
	SpriteBatch* spriteData;
	DXVertexShader* mScreenVS = nullptr;
	DXPixelShader* mScreenPS = nullptr;

	std::vector<SpriteBatch*> mSortedSprites;
	std::vector<BatchItem>    mBatchItems;

	UINT                           mQueuedIndex = 0;
	std::unique_ptr<SpriteBatch[]> mQueuedSprites;

	bool bIsBegin = false;;
};
