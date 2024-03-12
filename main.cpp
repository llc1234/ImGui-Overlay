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
