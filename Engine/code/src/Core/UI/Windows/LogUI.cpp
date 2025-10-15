#include "Core/UI/Windows/LogUI.h"
#include "Core/Engine.h"
#include "GameBehaviour/ScriptsManager.h"

#include <imgui.h>

LogUI::LogUI()
{
    scriptManager = Engine::GetInstance()->GetScriptsManager();
}

void LogUI::Render()
{
    ImGui::Begin("Log Console");

    if (ImGui::Button("Clear"))
    {
        scriptManager->compileLog.clear();
    }

    ImGui::SameLine();
    ImGui::Checkbox("Auto-scroll", &autoScroll);

    ImGui::Separator();

    ImGui::Text("Compilation Log:");
	ImGui::BeginChild("CompileLogRegion", ImVec2(0, ImGui::GetTextLineHeight() * 20), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::TextUnformatted(scriptManager->compileLog.c_str());

    if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();

    ImGui::End();
}


void LogUI::AddLog(const std::string& msg)
{
    logs.push_back(msg);
}

void LogUI::Clear()
{
    logs.clear();
}
