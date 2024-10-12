#pragma once
constexpr int BUILD_NUMBER		= 0x0;
constexpr int MIN_WIN_HEIGHT	= 800;
constexpr int MIN_WIN_WIDTH		= 600;

//Its for releasing game, Excluding debug features etc.
constexpr bool IS_COOKED     = true;

//Max FPS for VSYNC
constexpr int MAX_FPS        = 144;

constexpr bool VSYNC_ENABLED = true;

constexpr const char* META_FILE_EXT = ".smeta";

constexpr int PHYSX_VD_DEFAULT_PORT = 5425;
constexpr const char* PHYSX_VD_DEFAULT_HOST = "127.0.0.1";

constexpr const float GRAVITY_SCALE = -9.81f;