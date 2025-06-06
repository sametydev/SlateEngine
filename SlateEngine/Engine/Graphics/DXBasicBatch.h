#pragma once
#include <SlateEngine/Engine/Math/vec4f.h>
#include <SlateEngine/Engine/Graphics/DXApplication.h>
#include <wrl.h>
#include <vector>
#include <SlateEngine/Engine/DXConfig.h>
#include <SlateEngine/Engine/Graphics/Vertex.h>
#include <SlateEngine/Engine/Graphics/Texture/DXTexture.h>
#include <SlateEngine/Engine/Graphics/Shader/ShaderCache.h>
#include <SlateEngine/Engine/Graphics/Buffer/BufferCache.h>

typedef ComPtr<ID3D11VertexShader> VertexShaderPtr;
typedef ComPtr<ID3D11PixelShader> PixelShaderPtr;

enum class SortType : UINT {
	BackToFront,
	FrontToBack,
	TextureIndex
};

/*
	LT(0) ----- RT(1)
	|				|
	|				|
	LB(2) ----- RB(3)
*/

struct SpriteRect {
	VertexPT LT, RT, LB, RB;
	vec4f color;
};
enum DrawType {
	LINE, QUAD, TRIANGLE, LINESTRIP
};
struct BatchCommandList {
	DrawType type;
	UINT bufferOffset; //vertex curr pos
	UINT drawCount;
};


#define DEFAULT_SPRITE_SIZE 1024
#define BATCH_MAX_VERTEX   1024*4
#define INDEX_PER_QUAD 6
#define VERTEX_PER_QUAD 4
#define VERTEX_PER_LINE 2

class DXBasicBatch
{
public:
	DXBasicBatch(ID3D11Device* device, ID3D11DeviceContext* context);
	void Init();
	void Begin();
	void DrawRect(int x, int y, int w, int h,
		const vec3f& color = vec3f(1.f));
	void DrawLine(const vec2f& p1, const vec2f& p2,
		const vec3f& color = vec3f(1.f));
	void End();
	void Shutdown();
	void Update();
	static DXBasicBatch* Instance;

private:
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mContext;

	DXVertexShader* mShaderVS;
	DXPixelShader* mShaderPS;

	ComPtr<ID3D11Buffer> mConstantBuffer;
	ComPtr<ID3D11Buffer> mColorBuffer;

	ComPtr<ID3D11Buffer> mVertexBuffer;
	ComPtr<ID3D11Buffer> mIndexBuffer;

	void CreateShader();
	void CreateBuffer();

	std::unique_ptr<vec4f[]> mQueuedVertices;
	std::vector<BatchCommandList> mCmdList;
	std::vector<vec4f> mColorList;

	bool mIsBegin = false;
	UINT mQueueIndex = 0;

	mat4x4 screenNDC;

	SortType mType = SortType::BackToFront;
};