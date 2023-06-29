#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <dxgi1_3.h>
#define _XM_NO_INTRINSICS_
#define WIN32_LEAN_AND_MEAN
#include <DirectXMath.h>


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"DXGI.lib")

#ifdef _DEBUG
#pragma comment(lib,"dxguid.lib")
#endif

#define HR(x)\
{ \
HRESULT hr = (x); \
if(FAILED(hr)) \
{ \
std::cout << "\033[1;31m**************************\n" <<"Error on this file : \n " << __FILE__ << "\n In This Line : " <<(DWORD)__LINE__ << "  HR Code : "<< hr << "\n**************************\n\033[0m\n"<< std::endl; \
} \
}
#define SAFE_RELEASE(x) if(x) {x->Release(); x = nullptr;}
#define SAFE_DELETE(x) if(x) {delete x; x = nullptr;}
using namespace DirectX;


//operator overloading for testing purposes
std::ostream& operator<<(std::ostream& os, FXMVECTOR v)
{
	XMFLOAT4 dest;
	XMStoreFloat4(&dest, v);
	os << "Vec (" << dest.x << ", " << dest.y << ", " << dest.z << ", " << dest.w << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, CXMMATRIX m)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
			os << m(i,j) << "\t";
		os << std::endl;
	}
	return os;
}