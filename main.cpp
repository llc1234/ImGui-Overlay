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
    
    void Setup() {

    }
}
namespace Menu {
    const char* WindowTitle;

    void Setup() {

    }

    void Update() {
        ImGui::Begin(WindowTitle);
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

    void setup_imgui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL2_Init();
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

    void Setup() {
        Create_Window();
        setup_imgui();
        Menu::Setup();
        Menu::WindowTitle = WindowTitle;
        Memory::SetTargetWindowTitle("steam");
    }

    bool WindowShouldClose() {
        return glfwWindowShouldClose(window);
    }

    void Terminate() {
        glfwTerminate();
    }

    void Update() {
        glClear(GL_COLOR_BUFFER_BIT);

        ImguiNewFrame();

        Menu::Update();
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


/*
namespace Memory {
    const char* WindowTitle = "Counter-Strike 2";

    HWND hwnd;
    DWORD pid;
    HANDLE handle;

    uintptr_t client_DLL;
    uintptr_t engine_DLL;

    struct WindowSP {
        int PosX;
        int PosY;

        int width;
        int height;
    };

    struct Vec3 {
        float x;
        float y;
        float z;
    };

    WindowSP GetWindowPos() {
        RECT rect;
        GetWindowRect(hwnd, &rect);

        WindowSP SP;
        SP.PosX = rect.left;
        SP.PosY = rect.top;

        SP.width = rect.right - rect.left;
        SP.height = rect.bottom - rect.top;

        return SP;
    }

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

    void GetWindowsAddr() {
        hwnd = FindWindowA(0, WindowTitle);

        if (hwnd == NULL) {
            std::cerr << WindowTitle << ". NOT found" << std::endl;
        }

        GetWindowThreadProcessId(hwnd, &pid);

        handle = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);

        if (handle == NULL) {
            std::cerr << "Unable to open process. Error code: " << GetLastError() << std::endl;
        }

        std::cout << "hwnd  : " << hwnd << std::endl;
        std::cout << "pid   : " << pid << std::endl;
        std::cout << "handle: " << handle << std::endl;

        client_DLL = GetModuleBaseAddress("client.dll");
        engine_DLL = GetModuleBaseAddress("engine.dll");

        std::cout << "client.dll : " << client_DLL << std::endl;
        std::cout << "engine.dll : " << engine_DLL << std::endl;
    }

    void ScanForInt(const int targetValue) {
        MEMORY_BASIC_INFORMATION memoryInfo;
        uintptr_t address = 0;

        std::cout << "target value: " << targetValue << std::endl;

        int values_found = 0;

        while (VirtualQueryEx(handle, (LPVOID)address, &memoryInfo, sizeof(memoryInfo))) {
            if (memoryInfo.State == MEM_COMMIT && (memoryInfo.Type == MEM_MAPPED || memoryInfo.Type == MEM_PRIVATE)) {
                int* buffer = new int[memoryInfo.RegionSize / sizeof(int)];
                SIZE_T bytesRead;

                if (ReadProcessMemory(handle, memoryInfo.BaseAddress, buffer, memoryInfo.RegionSize, &bytesRead)) {
                    for (size_t i = 0; i < bytesRead / sizeof(int); ++i) {
                        if (buffer[i] == targetValue) {
                            std::cout << "At address: 0x" << (uintptr_t)memoryInfo.BaseAddress + i * sizeof(int) << std::endl;
                            values_found++;
                        }
                    }
                }

                delete[] buffer;
            }

            address += memoryInfo.RegionSize;
        }

        CloseHandle(handle);
    } 

    int ReadAddressInt(uintptr_t addr) {
        int value;
        ReadProcessMemory(handle, (void*)(addr), &value, sizeof(value), 0);
        return value;
    }

    float ReadAddressFloat(uintptr_t addr) {
        float value;
        ReadProcessMemory(handle, (void*)(addr), &value, sizeof(value), 0);
        return value;
    }

    uintptr_t ReadAddressUintptr_t(uintptr_t addr) {
        uintptr_t value;
        ReadProcessMemory(handle, (void*)(addr), &value, sizeof(value), 0);
        return value;
    }

    uintptr_t GetLocalPlayer() {
        return ReadAddressUintptr_t(Memory::client_DLL + address::localplayer);
    }

    Vec3 GetPos(uintptr_t entityaddr) {
        Vec3 v;
        ReadProcessMemory(handle, (void*)(entityaddr), &v, sizeof(v), 0);
        return v;
    }
}

namespace glfw {
    GLFWwindow* window;

    const char* WindowTitle = "RetroWare V2";

    GLFWmonitor* glfwPrimaryMonitor;

    int window_width;
    int window_height;

    int Create_Window() {
        if (!glfwInit())
            return -1;

        glfwPrimaryMonitor = glfwGetPrimaryMonitor();

        window_width = glfwGetVideoMode(glfwPrimaryMonitor)->width;
        window_height = glfwGetVideoMode(glfwPrimaryMonitor)->height;

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

    void SetWindowPS() {
        Memory::WindowSP sp = Memory::GetWindowPos();

        std::cout << "PosX: " << sp.PosX << ", PosY: " << sp.PosY;
        std::cout << " width: " << sp.width << ", height: " << sp.height;
        
        glfwSetWindowPos(window, sp.PosX, sp.PosY);
        glfwSetWindowSize(window, sp.width, sp.height);

        glViewport(0, 0, sp.width, sp.height);
    }

    void Setup() {
        Create_Window();
    }

    void clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void SwapBuffer() {
        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwSwapInterval(0);
    }
}

namespace Menu {
    bool MenuOpen = true;
    bool Menu_Switch_Open = true;

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

    void setup_imgui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(glfw::window, true);
        ImGui_ImplOpenGL2_Init();

        SetupImguiStyle();
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

    bool WindowShouldClose() {
        return glfwWindowShouldClose(glfw::window);
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
            glfwSetWindowAttrib(glfw::window, GLFW_MOUSE_PASSTHROUGH, false);
        } else {
            glfwSetWindowAttrib(glfw::window, GLFW_MOUSE_PASSTHROUGH, true);
        }
    }

    void DrawCheatMenu() {
        ImGui::SetNextWindowSize(ImVec2(700, 400));
        ImGui::Begin("RetroWare V2");

        ImGui::End();
    }

    void update() {
        OpenExitMenu();
        ImguiNewFrame();

        if (MenuOpen) {
            DrawCheatMenu();
        }

        ImguiRender();
    }
}

namespace Cheat_Radar {
    float map(float x, float in_min, float in_max, float out_min, float out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    void loop() {
        glBegin(GL_QUADS);
        glColor3f(0, 0, 0);
        glVertex2f(-0.9, -0.9);
        glVertex2f(-0.6, -0.9);
        glVertex2f(-0.6, -0.4);
        glVertex2f(-0.9, -0.4);
        glEnd();
        
        
        glBegin(GL_LINES);
        glColor3f(0, 1, 0);

        uintptr_t local_ad = Memory::GetLocalPlayer();

        float local_posX = map(Memory::GetPos(local_ad).x, -3247, 2243, -0.9, -0.6);
        float local_posY = map(Memory::GetPos(local_ad).y, -1336, 3895, -0.9, -0.4);

        // glVertex2f(Memory::ReadAddressFloat(local_ad + 0xD60) / 50000 + 0.003, Memory::ReadAddressFloat(local_ad + 0xD60 + 0x4) / 50000);
        // glVertex2f(Memory::ReadAddressFloat(local_ad + 0xD60) / 50000, Memory::ReadAddressFloat(local_ad + 0xD60 + 0x4) / 50000);


        glVertex2f(local_posX, local_posY);
        glVertex2f(0, 0);


        glColor3f(1, 0, 0);
        for (int i = 0; i < 64; i++) {
            uintptr_t ad = Memory::ReadAddressUintptr_t(Memory::client_DLL + 0x0173DBD0 + (i * 0x10));
            
            int Health = Memory::ReadAddressInt(ad + 0x334);

            if (Health > 0 && Health < 101) {

            }
        }

        glEnd();
    }
}

namespace Cheat {
    void update() {
        Cheat_Radar::loop();
    }
}

namespace RW2 {
	void StartUp() {
        glfw::Setup();
        Menu::setup_imgui();
        Memory::GetWindowsAddr();
        glfw::SetWindowPS();
	}

    void loop() {
        while (!Menu::WindowShouldClose()) {
            glfw::clear();
            Menu::update();
            Cheat::update();
            glfw::SwapBuffer();
        }
    }
}


int main() {
    RW2::StartUp();
    RW2::loop();

	return 0;
}

*/