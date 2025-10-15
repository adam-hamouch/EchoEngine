#pragma once
#include "imgui.h"

enum class Keycode
{
    A = ImGuiKey_A,
    B = ImGuiKey_B,
    C = ImGuiKey_C,
    D = ImGuiKey_D,
    E = ImGuiKey_E,
    F = ImGuiKey_F,
    G = ImGuiKey_G,
    H = ImGuiKey_H,
    I = ImGuiKey_I,
    J = ImGuiKey_J,
    K = ImGuiKey_K,
    L = ImGuiKey_L,
    M = ImGuiKey_M,
    N = ImGuiKey_N,
    O = ImGuiKey_O,
    P = ImGuiKey_P,
    Q = ImGuiKey_Q,
    R = ImGuiKey_R,
    S = ImGuiKey_S,
    T = ImGuiKey_T,
    U = ImGuiKey_U,
    V = ImGuiKey_V,
    W = ImGuiKey_W,
    X = ImGuiKey_X,
    Y = ImGuiKey_Y,
    Z = ImGuiKey_Z,

    Num0 = ImGuiKey_0,
    Num1 = ImGuiKey_1,
    Num2 = ImGuiKey_2,
    Num3 = ImGuiKey_3,
    Num4 = ImGuiKey_4,
    Num5 = ImGuiKey_5,
    Num6 = ImGuiKey_6,
    Num7 = ImGuiKey_7,
    Num8 = ImGuiKey_8,
    Num9 = ImGuiKey_9,

    UpArrow = ImGuiKey_UpArrow,
    DownArrow = ImGuiKey_DownArrow,
    LeftArrow = ImGuiKey_LeftArrow,
    RightArrow = ImGuiKey_RightArrow,

    Space = ImGuiKey_Space,
    Enter = ImGuiKey_Enter,
    Escape = ImGuiKey_Escape,
    Tab = ImGuiKey_Tab,
    Backspace = ImGuiKey_Backspace,
    Insert = ImGuiKey_Insert,
    Delete = ImGuiKey_Delete,
    Home = ImGuiKey_Home,
    End = ImGuiKey_End,
    PageUp = ImGuiKey_PageUp,
    PageDown = ImGuiKey_PageDown,

    Shift = ImGuiKey_LeftShift,
    LeftShift = ImGuiKey_LeftShift,
    RightShift = ImGuiKey_RightShift,
    Ctrl = ImGuiKey_LeftCtrl,
    LeftCtrl = ImGuiKey_LeftCtrl,
    RightCtrl = ImGuiKey_RightCtrl,
    Alt = ImGuiKey_LeftAlt,
    LeftAlt = ImGuiKey_LeftAlt,
    RightAlt = ImGuiKey_RightAlt,
    Super = ImGuiKey_LeftSuper, 
    LeftSuper = ImGuiKey_LeftSuper,
    RightSuper = ImGuiKey_RightSuper,
};

enum class MouseCode
{
    MouseButton1 = 0, // GLFW_MOUSE_BUTTON_1
    MouseButton2 = 1, // GLFW_MOUSE_BUTTON_2
    MouseButton3 = 2, // GLFW_MOUSE_BUTTON_3
    MouseButton4 = 3, // GLFW_MOUSE_BUTTON_4
    MouseButton5 = 4, // GLFW_MOUSE_BUTTON_5
    MouseButton6 = 5, // GLFW_MOUSE_BUTTON_6
    MouseButton7 = 6, // GLFW_MOUSE_BUTTON_7
    MouseButton8 = 7, // GLFW_MOUSE_BUTTON_8

    MouseButtonLast = MouseButton8,
    MouseButtonLeft = MouseButton1,
    MouseButtonRight = MouseButton2,
    MouseButtonMiddle = MouseButton3
};