#include <stdio.h>
#include <iostream>
#include <ostream>
#include <windows.h>
#include <TestEngine/DXConfig.h>

std::ostream& operator<<(std::ostream& os, FXMVECTOR v)
{
	XMFLOAT3 dest;
	XMStoreFloat3(&dest, v);
	os << "Vec (" << dest.x << ", " << dest.y << ", " << dest.z << ")";
	return os;
}


int main() {
	
	std::cout << "TestEngine Init" << "\n";
	

	return -1;
}