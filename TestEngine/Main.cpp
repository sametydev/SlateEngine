#include <stdio.h>
#include <iostream>
#include <ostream>
#include <windows.h>

#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"DXGI.lib")

#ifdef _DEBUG
#pragma comment(lib,"dxguid.lib")
#endif

using namespace DirectX;
using namespace std;

ostream& operator<<(ostream& os, FXMVECTOR v)
{
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);
	os << "(" << dest.x << ", " << dest.y << ", " << dest.z << ")";
	return os;
}


int main() {
	
	std::cout << "TestEngine Init" << "\n";

	cout.setf(ios_base::boolalpha);
	
	if (!XMVerifyCPUSupport())
	{
		cout << "DirectX math not supported" << endl;
		return 0;
	}

	return -1;
}