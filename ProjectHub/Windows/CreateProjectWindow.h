#pragma once
#include "Window.h"

class CreateProjectWindow : Window
{
public:
	CreateProjectWindow();
	~CreateProjectWindow();

	void Render() override;

	void SetOpen(bool val);
private:
	bool* p_closable;
};

