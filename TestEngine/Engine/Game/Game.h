#include <TestEngine/Engine/Application/DXApplication.h>

class Game : public DXApplication
{
public:
    Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~Game();

    bool OnInit();
    void OnResize();
    void OnUpdateScene(float dt);

    //Todo, Scene*
    void OnRenderScene();
};