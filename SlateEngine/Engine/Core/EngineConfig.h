#pragma once
constexpr int BUILD_NUMBER		= 0x0;
constexpr int MIN_WIN_HEIGHT	= 800;
constexpr int MIN_WIN_WIDTH		= 600;

namespace {
	//Its for releasing game, Excluding debug features etc.
	bool IS_COOKED     = false;

	//Max FPS for VSYNC
	int MAX_FPS        = 60;

	bool VSYNC_ENABLED = true;
}