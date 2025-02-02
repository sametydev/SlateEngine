#pragma once
struct Bindable {
public:
	virtual void Bind(unsigned int offset = 0) = 0;
};