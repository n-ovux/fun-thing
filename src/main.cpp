#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main() {
  const int WIDTH = 1600;
  const int HEIGHT = 900;

  SetTraceLogLevel(LOG_WARNING);
  InitWindow(WIDTH, HEIGHT, "fun");

  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

    EndDrawing();
  }
  return 0;
}
