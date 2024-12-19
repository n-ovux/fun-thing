#include <GL/glew.h>
#include <raylib.h>
#include <rlgl.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "../styles/dark/style_dark.h"

int main()
{
  const int WIDTH  = 1600;
  const int HEIGHT = 900;

  SetTraceLogLevel(LOG_WARNING);
  InitWindow(WIDTH, HEIGHT, "fun");
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
  GuiLoadStyleDark();

  if (glewInit() != GLEW_OK)
    exit(1);

  Camera3D camera   = {0};
  camera.position   = (Vector3){0.0f, 10.0f, 10.0f};
  camera.target     = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up         = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy       = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  Vector3 cube_position = (Vector3){0.0f, 0.0f, 0.0f};

  float factor                         = 0.1;
  RenderTexture2D visualization_screen = LoadRenderTexture(WIDTH, HEIGHT);

  while (!WindowShouldClose())
    {
      UpdateCamera(&camera, CAMERA_ORBITAL);

      BeginDrawing();
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      GuiPanel((Rectangle){factor * WIDTH, factor * HEIGHT, (1 - 2 * factor) * WIDTH, (1 - 2 * factor) * HEIGHT}, "Visualization");

      BeginTextureMode(visualization_screen);
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
      BeginMode3D(camera);

      DrawCube(cube_position, 2.0f, 2.0f, 2.0f, WHITE);
      DrawCubeWires(cube_position, 2.0f, 2.0f, 2.0f, MAROON);
      DrawGrid(10, 1.0f);

      EndMode3D();
      EndTextureMode();

      DrawTextureRec(visualization_screen.texture,
                     (Rectangle){factor * WIDTH, factor * HEIGHT, (1 - 2 * factor) * WIDTH - 2, -(1 - 2 * factor) * HEIGHT + 25},
                     (Vector2){factor * WIDTH + 1, factor * HEIGHT + 24}, WHITE);

      EndDrawing();
    }
  return 0;
}
