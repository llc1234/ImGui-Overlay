#include <vector>
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"



namespace Memory {
    HWND hwnd;
    DWORD pid;
    HANDLE handle;

    uintptr_t client_dll;

    uintptr_t GetModuleBaseAddress(const char* modName) {
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        if (hSnap != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 modEntry;
            modEntry.dwSize = sizeof(modEntry);
            if (Module32First(hSnap, &modEntry)) {
                do {
                    if (!strcmp(modEntry.szModule, modName)) {
                        CloseHandle(hSnap);
                        return (uintptr_t)modEntry.modBaseAddr;
                    }
                } while (Module32Next(hSnap, &modEntry));
            }
        }
    }

    void SetTargetWindowTitle(const char* title) {
        hwnd = FindWindowA(0, title);

        if (hwnd == NULL) {
            std::cerr << title << ". NOT found" << std::endl;
        }

        GetWindowThreadProcessId(hwnd, &pid);

        handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

        if (handle == NULL) {
            std::cerr << "Unable to open process. Error code: " << GetLastError() << std::endl;
        }

        std::cout << "hwnd  : " << hwnd << std::endl;
        std::cout << "pid   : " << pid << std::endl;
        std::cout << "handle: " << handle << std::endl;

        client_dll = GetModuleBaseAddress("client.dll");

        std::cout << "client.dll : " << client_dll << std::endl;
    }

    bool IsProcessRunning() {
        hwnd = FindWindowA(0, "Counter-strike 2");

        if (hwnd == NULL) {
            return false;
        }

        return true;
    }

    float ReadPointerAddressFloat(uintptr_t base, std::vector<uintptr_t> pointer) {
        uintptr_t addr = NULL;
        ReadProcessMemory(handle, (uintptr_t*)(base + pointer[0]), &addr, sizeof(addr), 0);

        for (int i = 1; i < pointer.size() - 1; i++) {
            ReadProcessMemory(handle, (uintptr_t*)(addr + pointer[i]), &addr, sizeof(addr), 0);
        }

        float value = 0.0f;
        ReadProcessMemory(handle, (uintptr_t*)(addr + pointer.back()), &value, sizeof(value), 0);

        return value;
    }
    
    void Setup() {

    }
}
namespace Menu {
    const char* WindowTitle;

    void Setup() {
        
    }

    void Update() {
        ImGui::SetNextWindowSize(ImVec2(700, 400));
        ImGui::Begin(WindowTitle, NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::End();
    }
}
namespace Cheat {
    void Update() {
        
    }
}
namespace Easy_overlay {
    GLFWwindow* window;
    GLFWmonitor* glfwPrimaryMonitor;

    int window_width;
    int window_height;
    const char* WindowTitle = "RetroWare V2";
    
    bool MenuOpen = true;
    bool Menu_Switch_Open = true;

    int Create_Window() {
        if (!glfwInit())
            return -1;

        glfwPrimaryMonitor = glfwGetPrimaryMonitor();

        int window_width = glfwGetVideoMode(glfwPrimaryMonitor)->width;
        int window_height = glfwGetVideoMode(glfwPrimaryMonitor)->height;

        glfwWindowHint(GLFW_FLOATING, true);
        glfwWindowHint(GLFW_RESIZABLE, false);
        glfwWindowHint(GLFW_MAXIMIZED, true);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

        window = glfwCreateWindow(window_width, window_height, WindowTitle, NULL, NULL);
        if (!window) {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);

        glfwSetWindowAttrib(window, GLFW_DECORATED, false);
        return 1;
    }

    void OpenExitMenu() {
        if (!Menu_Switch_Open && GetAsyncKeyState(VK_INSERT) & 0x8000) {
            Menu_Switch_Open = true;
            MenuOpen = !MenuOpen;
        }

        if (!(GetAsyncKeyState(VK_INSERT) & 0x8000)) {
            Menu_Switch_Open = false;
        }

        if (MenuOpen) {
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);
        } else {
            glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, true);
        }
    }

    void setup_imgui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL2_Init();
    }

    void SetupImguiStyle() {
        ImVec4* colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
        colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowPadding = ImVec2(8.00f, 8.00f);
        style.FramePadding = ImVec2(5.00f, 2.00f);
        style.CellPadding = ImVec2(6.00f, 6.00f);
        style.ItemSpacing = ImVec2(6.00f, 6.00f);
        style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
        style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
        style.IndentSpacing = 25;
        style.ScrollbarSize = 15;
        style.GrabMinSize = 10;
        style.WindowBorderSize = 1;
        style.ChildBorderSize = 1;
        style.PopupBorderSize = 1;
        style.FrameBorderSize = 1;
        style.TabBorderSize = 3;
        style.WindowRounding = 7;
        style.ChildRounding = 4;
        style.FrameRounding = 3;
        style.PopupRounding = 4;
        style.ScrollbarRounding = 9;
        style.GrabRounding = 3;
        style.LogSliderDeadzone = 4;
        style.TabRounding = 3;
    }

    void ImguiNewFrame() {
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImguiRender() {
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    }

    void IsGameOpen() {
        if (!Memory::IsProcessRunning()) {
            MessageBox(NULL, "Counter-Strike 2 is NOT running", "error", MB_OK | MB_ICONINFORMATION);
            exit(-1);
        }
    }

    void Setup() {
        IsGameOpen();
        Create_Window();
        setup_imgui();
        SetupImguiStyle();
        Menu::Setup();
        Menu::WindowTitle = WindowTitle;
    }

    bool WindowShouldClose() {
        return glfwWindowShouldClose(window);
    }

    void Terminate() {
        glfwTerminate();
    }

    void Update() {
        glClear(GL_COLOR_BUFFER_BIT);

        OpenExitMenu();
        ImguiNewFrame();

        if (MenuOpen) {
            Menu::Update();
        }
        Cheat::Update();

        ImguiRender();

        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwSwapInterval(0);
    }
}


int main() {
    Easy_overlay::Setup();

    while (!Easy_overlay::WindowShouldClose()) {
        Easy_overlay::Update();
    }
    
    Easy_overlay::Terminate();

    return 0;
}
