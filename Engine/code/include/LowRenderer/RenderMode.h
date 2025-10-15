#pragma once

#include <cstdint>

enum class RenderMode : uint32_t
{
	None = 0,
	Toon = 1 << 0,
	Outline = 1 << 1,
};

#include "RenderMode.inl" 