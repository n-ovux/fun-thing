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
  camera.position   = (Vector3){0.0f, 0.0f, 0.0f};
  camera.target     = (Vector3){0.0f, 0.0f, 0.0f};
  camera.up         = (Vector3){0.0f, 0.0f, 1.0f};
  camera.fovy       = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  float zoom        = 10.0f;
  Vector2 position  = (Vector2){PI / 4, PI / 4};

  Vector3 cube_position = (Vector3){0.0f, 0.0f, 0.0f};

  float factor                         = 0.1;
  RenderTexture2D visualization_screen = LoadRenderTexture(WIDTH, HEIGHT);

  while (!WindowShouldClose())
    {
      float sensitivity = 3.0f;
      zoom              = (zoom >= 1) ? zoom - GetMouseWheelMove() : 1;
      if (IsKeyDown(KEY_D))
        position.x += sensitivity * GetFrameTime();
      else if (IsKeyDown(KEY_A))
        position.x -= sensitivity * GetFrameTime();
      if (IsKeyDown(KEY_W))
        position.y = (position.y > PI / 2) ? position.y = PI / 2 : position.y + sensitivity * GetFrameTime();
      else if (IsKeyDown(KEY_S))
        position.y = (position.y < -PI / 2) ? position.y = -PI / 2 : position.y - sensitivity * GetFrameTime();
      camera.position = (Vector3){zoom * cos(position.x) * cos(position.y), zoom * sin(position.x) * cos(position.y), zoom * sin(position.y)};

      BeginDrawing();
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      BeginTextureMode(visualization_screen);
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
      BeginMode3D(camera);

      DrawCube(cube_position, 2.0f, 2.0f, 2.0f, WHITE);
      DrawCubeWires(cube_position, 2.0f, 2.0f, 2.0f, MAROON);
      rlPushMatrix();
      rlRotatef(90, 1, 0, 0);
      DrawGrid(10, 1.0f);
      rlPopMatrix();

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
