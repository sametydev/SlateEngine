#include <Windows.h>
#include <SlateEditor/Editor/EditorUI.h>

//Main Entry for Windows Subsystem
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE prevInstance,
    _In_ LPSTR cmdLine,
    _In_ int showCmd
)
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    Game game(hInstance, L"> SlateEngine | Renderer[DirectX11.1]", 1280, 720);

    new EditorUI();

    game.SetEditorSystem(EditorUI::Instance);
    game.SetWorkingDirectory("TestProject\\");
    game.SetLogger(LogWindow::Instance);

    if (!game.OnInit())
        return 0;

    //Loop
    return game.OnRun();
}