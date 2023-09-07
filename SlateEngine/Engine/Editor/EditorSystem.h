#pragma once
class EditorSystem
{
public:
	EditorSystem() {};
	virtual ~EditorSystem() {};

	virtual void OnInit() = 0;

	virtual void NewFrame() = 0;
	virtual void OnRender() = 0;

	//Update first render later
	virtual void OnUpdate(float deltaTime) = 0;

	virtual void ClearViewport(float rgba[4]) = 0;

	virtual void ResizeViewport(int w, int h) = 0;

};

