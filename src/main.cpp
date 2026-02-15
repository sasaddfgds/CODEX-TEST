#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace {

struct UiConfig {
    float accentHue = 0.58f;
    float frameRounding = 12.0f;
    float windowRounding = 14.0f;
    float tabRounding = 10.0f;
    float itemSpacing = 10.0f;
    bool compactMode = false;
    bool showDemoWindow = false;
};

void setupCyrillicFont() {
    ImGuiIO& io = ImGui::GetIO();
    const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();

    const char* fontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    if (io.Fonts->AddFontFromFileTTF(fontPath, 20.0f, nullptr, ranges) == nullptr) {
        io.Fonts->AddFontDefault();
        std::cerr << "Не удалось загрузить шрифт с кириллицей: " << fontPath << '\n';
    }
}

ImVec4 hsv(float h, float s, float v, float a = 1.0f) {
    ImVec4 out;
    ImGui::ColorConvertHSVtoRGB(h, s, v, out.x, out.y, out.z);
    out.w = a;
    return out;
}

void applyModernStyle(const UiConfig& cfg) {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = cfg.windowRounding;
    style.FrameRounding = cfg.frameRounding;
    style.TabRounding = cfg.tabRounding;
    style.GrabRounding = cfg.frameRounding;
    style.ScrollbarRounding = cfg.frameRounding;
    style.ChildRounding = cfg.frameRounding;
    style.PopupRounding = cfg.frameRounding;

    style.WindowPadding = cfg.compactMode ? ImVec2(10, 10) : ImVec2(16, 14);
    style.FramePadding = cfg.compactMode ? ImVec2(8, 5) : ImVec2(11, 7);
    style.ItemSpacing = ImVec2(cfg.itemSpacing, cfg.itemSpacing * 0.7f);
    style.ItemInnerSpacing = ImVec2(8, 6);

    ImVec4* colors = style.Colors;
    const ImVec4 bg0 = hsv(cfg.accentHue, 0.18f, 0.10f);
    const ImVec4 bg1 = hsv(cfg.accentHue, 0.20f, 0.14f);
    const ImVec4 bg2 = hsv(cfg.accentHue, 0.22f, 0.18f);
    const ImVec4 accent = hsv(cfg.accentHue, 0.70f, 0.95f);
    const ImVec4 accentMuted = hsv(cfg.accentHue, 0.50f, 0.65f);

    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.97f, 0.99f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.58f, 0.63f, 0.69f, 1.00f);
    colors[ImGuiCol_WindowBg] = bg0;
    colors[ImGuiCol_ChildBg] = bg1;
    colors[ImGuiCol_PopupBg] = bg1;
    colors[ImGuiCol_Border] = ImVec4(0.24f, 0.29f, 0.38f, 0.70f);
    colors[ImGuiCol_FrameBg] = bg1;
    colors[ImGuiCol_FrameBgHovered] = bg2;
    colors[ImGuiCol_FrameBgActive] = hsv(cfg.accentHue, 0.30f, 0.30f);

    colors[ImGuiCol_TitleBg] = bg1;
    colors[ImGuiCol_TitleBgActive] = bg2;
    colors[ImGuiCol_MenuBarBg] = bg1;

    colors[ImGuiCol_Header] = hsv(cfg.accentHue, 0.45f, 0.38f, 0.85f);
    colors[ImGuiCol_HeaderHovered] = hsv(cfg.accentHue, 0.62f, 0.58f, 0.90f);
    colors[ImGuiCol_HeaderActive] = accentMuted;

    colors[ImGuiCol_Button] = hsv(cfg.accentHue, 0.52f, 0.44f, 0.90f);
    colors[ImGuiCol_ButtonHovered] = hsv(cfg.accentHue, 0.70f, 0.62f, 0.95f);
    colors[ImGuiCol_ButtonActive] = accent;

    colors[ImGuiCol_Tab] = hsv(cfg.accentHue, 0.40f, 0.25f, 1.0f);
    colors[ImGuiCol_TabHovered] = hsv(cfg.accentHue, 0.67f, 0.58f, 0.95f);
    colors[ImGuiCol_TabSelected] = hsv(cfg.accentHue, 0.72f, 0.58f, 0.88f);
    colors[ImGuiCol_TabSelectedOverline] = accent;
    colors[ImGuiCol_TabDimmed] = hsv(cfg.accentHue, 0.28f, 0.20f, 0.95f);
    colors[ImGuiCol_TabDimmedSelected] = hsv(cfg.accentHue, 0.50f, 0.42f, 0.88f);

    colors[ImGuiCol_CheckMark] = accent;
    colors[ImGuiCol_SliderGrab] = accentMuted;
    colors[ImGuiCol_SliderGrabActive] = accent;
    colors[ImGuiCol_ResizeGrip] = accentMuted;
    colors[ImGuiCol_ResizeGripHovered] = accent;
    colors[ImGuiCol_ResizeGripActive] = accent;
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.34f, 0.42f, 0.8f);
}

}  // namespace

int main() {
    std::cout << "привет" << std::endl;

    if (!glfwInit()) {
        throw std::runtime_error("Не удалось инициализировать GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1024, 700, "ImGui: привет", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Не удалось создать окно GLFW");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    setupCyrillicFont();

    UiConfig ui;
    applyModernStyle(ui);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Вид")) {
                ImGui::Checkbox("Компактный режим", &ui.compactMode);
                ImGui::Checkbox("Окно демо ImGui", &ui.showDemoWindow);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Тема")) {
                const float previousHue = ui.accentHue;
                ImGui::SliderFloat("Акцент", &ui.accentHue, 0.0f, 1.0f);
                if (std::fabs(previousHue - ui.accentHue) > 0.0001f) {
                    applyModernStyle(ui);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        applyModernStyle(ui);

        ImGui::SetNextWindowPos(ImVec2(18, 40), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(860, 560), ImGuiCond_Once);

        ImGui::Begin("Современный интерфейс", nullptr,
                     ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

        if (ImGui::BeginMenuBar()) {
            ImGui::TextUnformatted("Навигация");
            ImGui::EndMenuBar();
        }

        if (ImGui::BeginTabBar("MainTabs", ImGuiTabBarFlags_Reorderable)) {
            if (ImGui::BeginTabItem("Главная")) {
                ImGui::Spacing();
                ImGui::TextUnformatted(u8"привет");
                ImGui::TextWrapped("Это уже не вкладки из 91-го: добавлены мягкие скругления, современная палитра и акцентные цвета.");
                ImGui::Dummy(ImVec2(0, 6));
                ImGui::Button("Нормальная кнопка", ImVec2(210, 0));
                ImGui::SameLine();
                ImGui::Button("Акцентная кнопка", ImVec2(210, 0));
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Параметры")) {
                bool changed = false;
                changed |= ImGui::SliderFloat("Скругление окна", &ui.windowRounding, 0.0f, 22.0f);
                changed |= ImGui::SliderFloat("Скругление элементов", &ui.frameRounding, 0.0f, 22.0f);
                changed |= ImGui::SliderFloat("Скругление вкладок", &ui.tabRounding, 0.0f, 22.0f);
                changed |= ImGui::SliderFloat("Интервалы", &ui.itemSpacing, 4.0f, 20.0f);
                changed |= ImGui::Checkbox("Компактный режим", &ui.compactMode);

                if (changed) {
                    applyModernStyle(ui);
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("О программе")) {
                ImGui::TextWrapped("Пример приложения на C++ и Dear ImGui со стилизацией вкладок и меню в более современном виде.");
                ImGui::BulletText("Тема меняется через акцентный цвет.");
                ImGui::BulletText("Размеры и скругления регулируются в реальном времени.");
                ImGui::BulletText("Слово \"привет\" выводится в UI и в stdout.");
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();

        if (ui.showDemoWindow) {
            ImGui::ShowDemoWindow(&ui.showDemoWindow);
        }

        ImGui::Render();

        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.05f, 0.07f, 0.09f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
