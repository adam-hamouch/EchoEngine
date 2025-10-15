#pragma once
#include <vector>
#include <string>

class ScriptManager; 

class LogUI
{
public:
    LogUI(); 

    void Render();
    void AddLog(const std::string& msg);
    void Clear();

private:
    std::vector<std::string> logs;
    bool autoScroll = true;

    ScriptManager* scriptManager; 
};
