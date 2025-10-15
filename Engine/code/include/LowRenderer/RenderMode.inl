#pragma once

inline RenderMode operator|(RenderMode a, RenderMode b)
{
	return static_cast<RenderMode>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline RenderMode operator&(RenderMode a, RenderMode b)
{
	return static_cast<RenderMode>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline RenderMode& operator|=(RenderMode& a, RenderMode b)
{
	a = a | b;
	return a;
}

inline bool HasFlag(RenderMode value, RenderMode flag)
{
	return (value & flag) == flag;
}
