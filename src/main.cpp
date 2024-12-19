#include <GL/glew.h>
#include <cmath>
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
  camera.position   = (Vector3){0.0f, 10.0f, 0.0f};
  camera.target     = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up         = (Vector3){0.0f, 0.0f, 1.0f};
  camera.fovy       = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  Vector2 position  = (Vector2){0.0f, 0.0f};

  Vector3 cube_position = (Vector3){0.0f, 0.0f, 0.0f};

  float factor                         = 0.1;
  RenderTexture2D visualization_screen = LoadRenderTexture(WIDTH, HEIGHT);

  while (!WindowShouldClose())
    {
      camera.position = (Vector3){10 * cos(position.x) * cos(position.y), 10 * sin(position.x) * cos(position.y), 10 * sin(position.y)};
      if (IsKeyDown(KEY_D))
        {
          position.x += 5.0f * GetFrameTime();
        }
      else if (IsKeyDown(KEY_A))
        {
          position.x -= 5.0f * GetFrameTime();
        }
      if (IsKeyDown(KEY_W))
        {
          position.y = (position.y > PI / 2) ? position.y = PI / 2 : position.y + 5.0f * GetFrameTime();
        }
      else if (IsKeyDown(KEY_S))
        {
          position.y = (position.y < -PI / 2) ? position.y = -PI / 2 : position.y - 5.0f * GetFrameTime();
        }

      BeginDrawing();
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      BeginTextureMode(visualization_screen);
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
      BeginMode3D(camera);

      DrawCube(cube_position, 2.0f, 2.0f, 2.0f, WHITE);
      DrawCubeWires(cube_position, 2.0f, 2.0f, 2.0f, MAROON);
      DrawPoint3D((Vector3){1.0f, 0.0f, 0.0f}, RED);
      DrawPoint3D((Vector3){0.0f, 1.0f, 0.0f}, BLUE);
      DrawPoint3D((Vector3){0.0f, 0.0f, 1.0f}, GREEN);

      EndMode3D();
      EndTextureMode();

      GuiPanel((Rectangle){factor * WIDTH, factor * HEIGHT, (1 - 2 * factor) * WIDTH, (1 - 2 * factor) * HEIGHT}, "Visualization");
      DrawTextureRec(visualization_screen.texture,
                     (Rectangle){factor * WIDTH, factor * HEIGHT, (1 - 2 * factor) * WIDTH - 2, -(1 - 2 * factor) * HEIGHT + 25},
                     (Vector2){factor * WIDTH + 1, factor * HEIGHT + 24}, WHITE);

      EndDrawing();
    }
  return 0;
}
