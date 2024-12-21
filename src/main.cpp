#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

#include <raylib.h>
#include <rlgl.h>
#include <serialib.h>
#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <renderers/raylib/clay_renderer_raylib.c>

serialib serial1;
serialib serial2;

void error(std::string text, int code)
{
  std::cerr << text << std::endl;
  exit(code);
}

void button_click(Clay_ElementId id, Clay_PointerData mouse, intptr_t user_data)
{
  if (mouse.state == CLAY_POINTER_DATA_RELEASED_THIS_FRAME)
    {
      switch (user_data)
        {
        case 1:
          serial1.writeChar(49);
          break;
        case 2:
          serial1.writeChar(50);
          break;
        case 3:
          serial1.writeChar(51);
          break;
        }
    }
}

int main()
{
  const Clay_Color COLOR_SURFACE1 = {18, 18, 18, 255};
  const Clay_Color COLOR_SURFACE2 = {40, 40, 40, 255};
  const Clay_Color COLOR_SURFACE3 = {63, 63, 63, 255};
  const Clay_Color COLOR_SURFACE4 = {87, 87, 87, 255};
  const Clay_Color COLOR_SURFACE5 = {113, 113, 113, 255};
  const Clay_Color COLOR_SURFACE6 = {139, 139, 139, 255};

  const Clay_Color COLOR_ACCENT1 = {97, 51, 19, 255};
  const Clay_Color COLOR_ACCENT2 = {118, 73, 198, 255};
  const Clay_Color COLOR_ACCENT3 = {137, 95, 206, 255};
  const Clay_Color COLOR_ACCENT4 = {155, 117, 213, 255};
  const Clay_Color COLOR_ACCENT5 = {172, 139, 220, 255};
  const Clay_Color COLOR_ACCENT6 = {189, 162, 228, 255};

  const Clay_Color COLOR_BLACK = {0, 0, 0, 255};
  const Clay_Color COLOR_WHITE = {255, 255, 255, 255};

  const int WIDTH  = 1600;
  const int HEIGHT = 900;

  SetTraceLogLevel(LOG_WARNING);
  /*SetConfigFlags(FLAG_WINDOW_RESIZABLE);*/
  InitWindow(WIDTH, HEIGHT, "fun");
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

  uint64_t clay_memory_size = Clay_MinMemorySize();
  Clay_Arena clay_memory =
      Clay_Arena{.label = CLAY_STRING("Clay Memory Area"), .capacity = clay_memory_size, .memory = (char *)malloc(clay_memory_size)};
  Clay_Initialize(clay_memory, Clay_Dimensions{WIDTH, HEIGHT});
  Clay_SetMeasureTextFunction(Raylib_MeasureText);
  const int FONT_ID_BODY16     = 0;
  Raylib_fonts[FONT_ID_BODY16] = (Raylib_Font){.fontId = FONT_ID_BODY16, .font = LoadFontEx("/usr/share/fonts/noto/NotoSans-Medium.ttf", 48, 0, 400)};

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

  float value1 = 0.0f;
  float value2 = 0.0f;

  if (serial1.openDevice("/dev/ttyACM0", 9600) != 1)
    error("failed to open /dev/ttyACM0", 2);
  if (serial2.openDevice("/dev/ttyACM1", 9600) != 1)
    error("failed to open /dev/ttyACM1", 2);

  char *code = (char *)calloc(sizeof(char), 3);
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
      if (IsKeyDown(KEY_ZERO))
        {
          position.x = PI / 4;
          position.y = PI / 4;
          zoom       = 10;
        }
      camera.position = (Vector3){zoom * cos(position.x) * cos(position.y), zoom * sin(position.x) * cos(position.y), zoom * sin(position.y)};

      BeginDrawing();
      ClearBackground(BLACK);

      BeginTextureMode(visualization_screen);
      ClearBackground(BLACK);
      BeginMode3D(camera);

      DrawCube(cube_position, 2.0f, 2.0f, 2.0f, WHITE);
      DrawCubeWires(cube_position, 2.0f, 2.0f, 2.0f, MAROON);
      rlPushMatrix();
      rlRotatef(90, 1, 0, 0);
      DrawGrid(10, 1.0f);
      rlPopMatrix();

      EndMode3D();
      EndTextureMode();

      Clay_SetLayoutDimensions((Clay_Dimensions){
          .width  = GetScreenWidth(),
          .height = GetScreenHeight(),
      });

      Clay_SetPointerState({GetMouseX(), GetMouseY()}, IsMouseButtonDown(0));

      Clay_BeginLayout();

      // clang-format off
      CLAY(
          CLAY_ID("Main"), 
          CLAY_RECTANGLE({.color = COLOR_SURFACE1}),
          CLAY_LAYOUT({
               .sizing          = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
               .padding         = {16, 16},
               .childGap        = 16,
               .layoutDirection = CLAY_LEFT_TO_RIGHT,
          }))
      {
      // Microcontroller Section
      CLAY(
          CLAY_ID("Microcontrollers"), 
          CLAY_RECTANGLE({
            .color = COLOR_SURFACE2, 
            .cornerRadius = {20}
            }),
          CLAY_LAYOUT({
            .sizing =
               {
                   .width   = CLAY_SIZING_PERCENT(0.5f),
                   .height  = CLAY_SIZING_GROW(),
               },
           .padding         = {16, 16},
           .childGap        = 16,
           .layoutDirection = CLAY_TOP_TO_BOTTOM}))
       {
       // Microcontroller 1
       CLAY(
           CLAY_ID("Microcontroller 1"), 
           CLAY_RECTANGLE({
             .color = COLOR_SURFACE3, 
             .cornerRadius = {20}
             }),
          CLAY_TEXT(
               CLAY_STRING("Microcontroller 1"), 
               CLAY_TEXT_CONFIG({
                 .textColor = COLOR_WHITE,
                 .fontId    = FONT_ID_BODY16,
               .fontSize  = 50,
               })),
           CLAY_LAYOUT({
             .sizing =
             {
               .width  = CLAY_SIZING_GROW(),
               .height = CLAY_SIZING_PERCENT(0.5f),
             }, 
             .layoutDirection = CLAY_LEFT_TO_RIGHT
             })
          )
       {
         CLAY(
             CLAY_ID("LED1"),
             CLAY_RECTANGLE({
               .color = COLOR_SURFACE4,
               .cornerRadius = {8}
               }),
             CLAY_TEXT(
               CLAY_STRING("LED 1"), 
               CLAY_TEXT_CONFIG({
                 .textColor = COLOR_WHITE,
                 .fontId    = FONT_ID_BODY16,
               .fontSize  = 50,
             })),
             Clay_OnHover(button_click, 1)
             );
         CLAY(
             CLAY_ID("LED2"),
             CLAY_RECTANGLE({
               .color = COLOR_SURFACE4,
               .cornerRadius = {8}
               }),
             CLAY_TEXT(
               CLAY_STRING("LED 2"), 
               CLAY_TEXT_CONFIG({
                 .textColor = COLOR_WHITE,
                 .fontId    = FONT_ID_BODY16,
               .fontSize  = 50,
             })),
             Clay_OnHover(button_click, 2)
             );
         CLAY(
             CLAY_ID("LED3"),
             CLAY_RECTANGLE({
               .color = COLOR_SURFACE4,
               .cornerRadius = {8}
               }),
             CLAY_TEXT(
               CLAY_STRING("LED 3"), 
               CLAY_TEXT_CONFIG({
                 .textColor = COLOR_WHITE,
                 .fontId    = FONT_ID_BODY16,
               .fontSize  = 50,
             })),
             Clay_OnHover(button_click, 3)
             );
      } 
      // Microcontroller 2
      CLAY(
          CLAY_ID("Microcontroller 2"), 
          CLAY_RECTANGLE({
            .color = COLOR_SURFACE3, 
            .cornerRadius = {20}
          }),
          CLAY_LAYOUT({
            .sizing = {
            .width = CLAY_SIZING_GROW(),
            .height = CLAY_SIZING_PERCENT(0.5f)}
            }))
      {}}
// Visualization Section
      CLAY(
          CLAY_ID("Visualization"), 
          CLAY_RECTANGLE({
            .color = COLOR_SURFACE4, 
            .cornerRadius = {20}
          }),
          CLAY_LAYOUT({
            .sizing = {
            .width = CLAY_SIZING_PERCENT(0.5f), 
            .height = CLAY_SIZING_GROW()
          }}))
      {}};
// clang-format on

Clay_Raylib_Render(Clay_EndLayout());
/*DrawRectangle(0, 0, 50, 25, CLAY_COLOR_TO_RAYLIB_COLOR(COLOR_SURFACE1));*/
/*DrawRectangle(0, 25, 50, 25, CLAY_COLOR_TO_RAYLIB_COLOR(COLOR_SURFACE2));*/
/*DrawRectangle(0, 50, 50, 25, CLAY_COLOR_TO_RAYLIB_COLOR(COLOR_SURFACE3));*/
/*DrawRectangle(0, 75, 50, 25, CLAY_COLOR_TO_RAYLIB_COLOR(COLOR_SURFACE4));*/
EndDrawing();
}

CloseWindow();
serial1.closeDevice();
serial2.closeDevice();

return 0;
}
