#pragma once

#ifdef GAME_EXPORTS
#define GAME __declspec(dllexport)
#else
#define GAME __declspec(dllimport)
#endif
