#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

//// Universal include path, not every compiler supports!
// #include <raylib.h>
#include "raylib/raylib/src/raylib.h"
#include "raylib/raylib/src/raymath.h"

/* SCREEN_FACTOR */
// Remember that to set this value to exact 120 for better
// result in rendering objects and such, because things might
// go weird, especially in the 4th game, when the objects
// placement are based on literal floating-point numbers and
// not from the actual resizing values from the WIDTH and the
// HEIGHT of the screen.
#define SCREEN_FACTOR 120
#define WIDTH         16 * SCREEN_FACTOR
#define HEIGHT        9  * SCREEN_FACTOR

#define FONTSIZE_20  20
#define FONTSIZE_24  24
#define FONTSIZE_28  28
#define FONTSIZE_32  32
#define FONTSIZE_36  36
#define FONTSIZE_40  40

#define BLINKING    30 // Default: 30 -> 1s delay (15 = 0.5s, 60 = 2s, and so on)
#define RESIZER     2.0f
#define SCROLLSPEED 5.0f

void MainMenuWindow(void);
void MovingAroundBallDemoWindow(void);
void BouncingBallDemoWindow(void);

void BouncingBallDemoWindow(void) {
    // InitWindow(WIDTH, HEIGHT, "Bouncing Ball Animation (DVD screenlike)");
    // SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_WINDOW_RESIZABLE);
    // SetWindowFocused();
    // SetTargetFPS(60);

    Vector2 BallPosition = {.x = GetScreenWidth() / 2.0f, .y = GetScreenHeight() / 2.0f};
    Vector2 BallVelocity = {.x = 10.0f, .y = 8.0f};
    float BallRadius = 50.0f;
    
    Color BGColor = {225, 234, 234, 255};
    bool IsFullscreen = false, IsPause = false;
    int FramesCounter = 0, TextBlinkingTime = 0, PressedKey = 0, PressedCombinedKey = 0;
    double ElapsedTime = 0.0f;

    const char* PauseMessage        = "Press [SPACE] again to continue...";
    const char* RadiusResizingError = "The ball's radius CAN'T BE resized furthermore! Resize the ball the otherway around...";
    const char* RestartAgainMessage = "Press [R] to reset all stats...";
    const char* ResizingMessage     = "Scroll [UP] OR [DOWN] (using your mouse wheel/touchpad) to resize the ball!";
    const char* VelocityChangerINC  = "Increase the ball's VELOCITY by holding down [Z]: (x) OR [X]: (y)";
    const char* VelocityChangerDEC  = "Decrease the ball's VELOCITY by holding down [SHIFT][Z]: (x) OR [SHIFT][X]: (y)";

    const char* UpdateVariables01 = "(see for commented lines/codes in the \"void BouncingBallDemoWindow(void)\"";
    const char* UpdateVariables02 = "function to change some of the values internally...)";

    const char* BallPosXYMessage    = "BallPosition (x: px, y: px):";
    const char* BallVelXYMessage    = "BallVelocity (x: px/s, y: px/s):";
    const char* BallRadiusMessage   = "BallRadius:";

    while (!WindowShouldClose()) {
        ElapsedTime += GetFrameTime();

        if (!IsFullscreen && IsKeyPressed(KEY_F)) { ToggleFullscreen(); IsFullscreen = !IsFullscreen; }
        if (IsFullscreen && IsKeyPressed(KEY_F))  { ToggleBorderlessWindowed(); IsFullscreen = !IsFullscreen; }

        if (!IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_Z)) {
            if (BallVelocity.x >= 0) BallVelocity.x += 0.1f;
            if (BallVelocity.x < 0) BallVelocity.x -= 0.1f;
        } if (!IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_X)) {
            if (BallVelocity.y >= 0) BallVelocity.y += 0.1f;
            if (BallVelocity.y < 0) BallVelocity.y -= 0.1f;
        } if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_Z)) {
            if (BallVelocity.x >= 0) BallVelocity.x -= 0.1f;
            if (BallVelocity.x < 0) BallVelocity.x += 0.1f;
        } if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_X)) {
            if (BallVelocity.y >= 0) BallVelocity.y -= 0.1f;
            if (BallVelocity.y < 0) BallVelocity.y += 0.1f;
        }

        if (PressedKey != 0 && PressedCombinedKey != 0) printf("%d, %d = %d\n", PressedKey, PressedCombinedKey, PressedKey + PressedCombinedKey);
        if (IsKeyPressed(KEY_R)) {
            BallPosition.x = GetScreenWidth() / 2.0f; BallPosition.y = GetScreenHeight() / 2.0f;
            BallVelocity.x = 10.0f; BallVelocity.y = 8.0f;
            BallRadius = 50.0f;
            IsPause = false;
            FramesCounter = 0, TextBlinkingTime = 0;
        } if (IsKeyPressed(KEY_SPACE)) IsPause = !IsPause;

        if (!IsPause) {
            BallPosition.x += BallVelocity.x;
            BallPosition.y += BallVelocity.y;

            if ((BallPosition.x >= (GetScreenWidth() - BallRadius)) || (BallPosition.x <= BallRadius)) BallVelocity.x *= -1;
            if ((BallPosition.y >= (GetScreenHeight() - BallRadius)) || (BallPosition.y <= BallRadius)) BallVelocity.y *= -1;
        } else FramesCounter++;

        BeginDrawing();
        ClearBackground(BGColor);
        DrawCircleV(BallPosition, BallRadius, SKYBLUE);

        TextBlinkingTime = FramesCounter / BLINKING;
        if (IsPause && (TextBlinkingTime % 2 == 0)) DrawText(PauseMessage, ((GetScreenWidth()/2.0f) - (MeasureText(PauseMessage, FONTSIZE_32)/2.0f)), ((GetScreenHeight()/2.0f)), FONTSIZE_32, BLACK);
        DrawFPS(20, 20);

        if (BallRadius <= 0.0f) { BallRadius = 0.0f; DrawText(RadiusResizingError, (GetScreenWidth()/2.0f) - (MeasureText(RadiusResizingError, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 60, FONTSIZE_28, RED); }
        if (BallRadius >= (GetScreenHeight() / 2.0f)) { BallRadius = (GetScreenHeight() / 2.0f); DrawText(RadiusResizingError, (GetScreenWidth()/2.0f) - (MeasureText(RadiusResizingError, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 60, FONTSIZE_28, RED); }
        DrawText(TextFormat("%s (%.0fpx, %.0fpx)", BallPosXYMessage, BallPosition.x, BallPosition.y), 20, 50, FONTSIZE_20, RED);
        DrawText(TextFormat("%s (%.1fpx/s, %.1fpx/s)", BallVelXYMessage, BallVelocity.x, BallVelocity.y), 20, 80, FONTSIZE_20, GREEN);
        DrawText(TextFormat("%s %.0fpx", BallRadiusMessage, BallRadius), 20, 110, FONTSIZE_20, BLUE);
        
        DrawText(RestartAgainMessage, (GetScreenWidth()/2.0f) - (MeasureText(RestartAgainMessage, FONTSIZE_24)/2.0f), (GetScreenHeight()/2.0f) + ((GetScreenHeight()/2.0f) - 30), FONTSIZE_24, MAGENTA);
        if (ElapsedTime < 5.0f) DrawText(ResizingMessage, (GetScreenWidth()/2.0f) - (MeasureText(ResizingMessage, FONTSIZE_36)/2.0f), (GetScreenHeight()/2.0f) - 240, FONTSIZE_36, BLACK);
        if (ElapsedTime < 5.0f) DrawText(VelocityChangerINC, (GetScreenWidth()/2.0f) - (MeasureText(VelocityChangerINC, FONTSIZE_36)/2.0f), (GetScreenHeight()/2.0f) - 160, FONTSIZE_36, BLACK);
        if (ElapsedTime < 5.0f) DrawText(VelocityChangerDEC, (GetScreenWidth()/2.0f) - (MeasureText(VelocityChangerDEC, FONTSIZE_36)/2.0f), (GetScreenHeight()/2.0f) - 120, FONTSIZE_36, BLACK);
        if (ElapsedTime < 5.0f) DrawText(UpdateVariables01, (GetScreenWidth()/2.0f) - (MeasureText(UpdateVariables01, FONTSIZE_32)/2.0f), (GetScreenHeight()/2.0f) + (GetScreenHeight()/2.75f), FONTSIZE_32, GRAY);
        if (ElapsedTime < 5.0f) DrawText(UpdateVariables02, (GetScreenWidth()/2.0f) - (MeasureText(UpdateVariables02, FONTSIZE_32)/2.0f), (GetScreenHeight()/2.0f) + (GetScreenHeight()/2.75f) + 30.0f, FONTSIZE_32, GRAY);

        BallRadius -= (GetMouseWheelMove() * (RESIZER * SCROLLSPEED));
        EndDrawing();
    }

    // CloseWindow();
    // MainMenuWindow();
}

void MovingAroundBallDemoWindow(void) {
    // InitWindow(WIDTH, HEIGHT, "Moving Ball Across The Screen (Player 2D-like)");
    // SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_WINDOW_RESIZABLE);
    // SetWindowFocused();
    // SetTargetFPS(60);

    Vector2 BallPosition = {.x = GetScreenWidth() / 2.0f, .y = GetScreenHeight() / 2.0f};
    Vector2 BallVelocity = {.x = 10.0f, .y = 8.0f};
    float BallRadius = 50.0f;

    Color BGColor = {225, 234, 234, 255};
    bool IsFullscreen = false, IsPaused = false;
    int FramesCounter = 0, TextBlinkingTime = 0;
    double ElapsedTime = 0.0f;

    const char* PauseMessage        = "Press [SPACE] again to continue...";
    const char* RadiusResizingError = "The ball's radius CAN'T BE resized furthermore! Resize the ball the otherway around...";
    const char* RestartAgainMessage = "Press [R] to RESET ALL stats...";
    const char* ResizingMessage     = "Scroll [UP] OR [DOWN] (using your mouse wheel/touchpad) to resize the ball!";
    const char* MoveAroundGuide     = "Use [W][A][S][D] OR [^][v][<][>] (arrow keys) to move around...";
    const char* VelocityChangerINC  = "Increase the ball's VELOCITY by holding down [Z]: (x) OR [X]: (y)";
    const char* VelocityChangerDEC  = "Decrease the ball's VELOCITY by holding down [SHIFT][Z]: (x) OR [SHIFT][X]: (y)";

    const char* UpdateVariables01 = "(see for commented lines/codes in the \"void MovingAroundBallDemoWindow(void)\"";
    const char* UpdateVariables02 = "function to change some of the values internally...)";

    const char* BallPosXYMessage    = "BallPosition (x: px, y: px):";
    const char* BallVelXYMessage    = "BallVelocity (x: px/s, y: px/s):";
    const char* BallRadiusMessage   = "BallRadius:";

    while (!WindowShouldClose()) {
        ElapsedTime += GetFrameTime();

        if (!IsFullscreen && IsKeyPressed(KEY_F)) { ToggleFullscreen(); IsFullscreen = !IsFullscreen; }
        if (IsFullscreen && IsKeyPressed(KEY_F))  { ToggleBorderlessWindowed(); IsFullscreen = !IsFullscreen; }

        if (!IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_Z)) {
            if (BallVelocity.x >= 0) BallVelocity.x += 0.1f;
            if (BallVelocity.x < 0) BallVelocity.x -= 0.1f;
        } if (!IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_X)) {
            if (BallVelocity.y >= 0) BallVelocity.y += 0.1f;
            if (BallVelocity.y < 0) BallVelocity.y -= 0.1f;
        } if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_Z)) {
            if (BallVelocity.x >= 0) BallVelocity.x -= 0.1f;
            if (BallVelocity.x < 0) BallVelocity.x += 0.1f;
        } if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_X)) {
            if (BallVelocity.y >= 0) BallVelocity.y -= 0.1f;
            if (BallVelocity.y < 0) BallVelocity.y += 0.1f;
        }
        
        if (IsKeyPressed(KEY_R)) {
            BallPosition.x = GetScreenWidth() / 2.0f; BallPosition.y = GetScreenHeight() / 2.0f;
            BallVelocity.x = 10.0f; BallVelocity.y = 8.0f;
            BallRadius = 50.0f;
            IsPaused = false;
            FramesCounter = 0, TextBlinkingTime = 0;
        } if (IsKeyPressed(KEY_SPACE)) IsPaused = !IsPaused;

        if (!IsPaused) {
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) BallPosition.x += BallVelocity.x;
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  BallPosition.x -= BallVelocity.x;
            if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  BallPosition.y += BallVelocity.y;
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    BallPosition.y -= BallVelocity.y;

            if (BallPosition.x <= BallRadius) BallPosition.x = BallRadius;
            if (BallPosition.y <= BallRadius) BallPosition.y = BallRadius;
            if (BallPosition.x >= (GetScreenWidth() - BallRadius)) BallPosition.x = (GetScreenWidth() - BallRadius);
            if (BallPosition.y >= (GetScreenHeight() - BallRadius)) BallPosition.y = (GetScreenHeight() - BallRadius);
        } else FramesCounter++;

        BeginDrawing();
        ClearBackground(BGColor);
        DrawCircleV(BallPosition, BallRadius, SKYBLUE);

        TextBlinkingTime = FramesCounter / BLINKING;
        if (IsPaused && (TextBlinkingTime % 2 == 0)) DrawText(PauseMessage, ((GetScreenWidth()/2.0f) - (MeasureText(PauseMessage, FONTSIZE_32)/2.0f)), ((GetScreenHeight()/2.0f)), FONTSIZE_32, BLACK);
        DrawFPS(20, 20);

        if (BallRadius <= 0.0f) { BallRadius = 0.0f; DrawText(RadiusResizingError, (GetScreenWidth()/2.0f) - (MeasureText(RadiusResizingError, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 60, FONTSIZE_28, RED); }
        if (BallRadius >= (GetScreenHeight() / 2.0f)) { BallRadius = (GetScreenHeight() / 2.0f); DrawText(RadiusResizingError, (GetScreenWidth()/2.0f) - (MeasureText(RadiusResizingError, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 60, FONTSIZE_28, RED); }
        DrawText(TextFormat("%s (%.0fpx, %.0fpx)", BallPosXYMessage, BallPosition.x, BallPosition.y), 20, 50, FONTSIZE_20, RED);
        DrawText(TextFormat("%s (%.1fpx/s, %.1fpx/s)", BallVelXYMessage, BallVelocity.x, BallVelocity.y), 20, 80, FONTSIZE_20, GREEN);
        DrawText(TextFormat("%s %.0fpx", BallRadiusMessage, BallRadius), 20, 110, FONTSIZE_20, BLUE);
        
        DrawText(RestartAgainMessage, (GetScreenWidth()/2.0f) - (MeasureText(RestartAgainMessage, FONTSIZE_24)/2.0f), (GetScreenHeight()/2.0f) + ((GetScreenHeight()/2.0f) - 30), FONTSIZE_24, MAGENTA);
        if (ElapsedTime < 5.0f) DrawText(MoveAroundGuide, (GetScreenWidth()/2.0f) - (MeasureText(MoveAroundGuide, FONTSIZE_40)/2.0f), (GetScreenHeight()/2.0f) - 320, FONTSIZE_40, BLACK);
        if (ElapsedTime < 5.0f) DrawText(ResizingMessage, (GetScreenWidth()/2.0f) - (MeasureText(ResizingMessage, FONTSIZE_36)/2.0f), (GetScreenHeight()/2.0f) - 240, FONTSIZE_36, BLACK);
        if (ElapsedTime < 5.0f) DrawText(VelocityChangerINC, (GetScreenWidth()/2.0f) - (MeasureText(VelocityChangerINC, FONTSIZE_36)/2.0f), (GetScreenHeight()/2.0f) - 160, FONTSIZE_36, BLACK);
        if (ElapsedTime < 5.0f) DrawText(VelocityChangerDEC, (GetScreenWidth()/2.0f) - (MeasureText(VelocityChangerDEC, FONTSIZE_36)/2.0f), (GetScreenHeight()/2.0f) - 120, FONTSIZE_36, BLACK);
        if (ElapsedTime < 5.0f) DrawText(UpdateVariables01, (GetScreenWidth()/2.0f) - (MeasureText(UpdateVariables01, FONTSIZE_32)/2.0f), (GetScreenHeight()/2.0f) + (GetScreenHeight()/2.75f), FONTSIZE_32, GRAY);
        if (ElapsedTime < 5.0f) DrawText(UpdateVariables02, (GetScreenWidth()/2.0f) - (MeasureText(UpdateVariables02, FONTSIZE_32)/2.0f), (GetScreenHeight()/2.0f) + (GetScreenHeight()/2.75f) + 30.0f, FONTSIZE_32, GRAY);
        
        BallRadius -= (GetMouseWheelMove() * (RESIZER * SCROLLSPEED));
        EndDrawing();
    }

    // CloseWindow();
    // MainMenuWindow();
}

void ChessboardGeneratorDemoWindow(void) {
    // Recommended for better experience:
    //  -> Cells = {16, 16}, CellArea = 50.0f
    Vector2 Cells = {.x = 16, .y = 16};
    Color CellColorWHITE = WHITE, CellColorBLACK = BLACK;
    float CellArea = 50.0f;
    
    bool IsFullscreen = false, CellBoardColorChanger = false;
    char ColorP1[16] = "...", ColorP2[16] = "...";
    double ElapsedTime = 0.0f;

    const char* RestartAgainMessage    = "Press [R] to RESET ALL stats...";
    const char* CellBoardColorSwitcher = "Press [C] to CHANGE the color switch CHANNEL with another cell board...";
    const char* ColorSwitchAccess      = "[0][1][2][3][4][5][6][7][8][9]: Switch to 10 (ten) different colors!";
    const char* ColorChannels          = "Color Channels: {BLACK, RED, ORANGE, GOLD, GREEN, BLUE, PINK, PURPLE, WHITE, BROWN}";

    const char* CellBoardColorChangeP1 = "> [P1]: WHITE -> Color changed into:";
    const char* CellBoardColorChangeP2 = "> [P2]: BLACK -> Color changed into:";

    while (!WindowShouldClose()) {
        ElapsedTime += GetFrameTime();
        
        BeginDrawing();
        ClearBackground(GRAY);

        if (!IsFullscreen && IsKeyPressed(KEY_F)) { ToggleFullscreen(); IsFullscreen = !IsFullscreen; }
        if (IsFullscreen && IsKeyPressed(KEY_F))  { ToggleBorderlessWindowed(); IsFullscreen = !IsFullscreen; }
        if (IsKeyPressed(KEY_C)) { CellBoardColorChanger = !CellBoardColorChanger; }
        if (IsKeyPressed(KEY_R)) { CellColorWHITE = WHITE; CellColorBLACK = BLACK; CellBoardColorChanger = false; strncpy(ColorP1, "WHITE", 16), strncpy(ColorP2, "BLACK", 16); }
        
        // CellBoard -> Defaults to WHITE (changed accordingly)
        if (!CellBoardColorChanger && IsKeyPressed(KEY_ZERO))  { CellColorWHITE = BLACK; strncpy(ColorP1, "BLACK", 16); }
        if (!CellBoardColorChanger && IsKeyPressed(KEY_ONE))   { CellColorWHITE = RED; strncpy(ColorP1, "RED", 16); }
        if (!CellBoardColorChanger && IsKeyPressed(KEY_TWO))   { CellColorWHITE = ORANGE; strncpy(ColorP1, "ORANGE", 16); }
        if (!CellBoardColorChanger && IsKeyPressed(KEY_THREE)) { CellColorWHITE = GOLD; strncpy(ColorP1, "GOLD", 16); }
        if (!CellBoardColorChanger && IsKeyPressed(KEY_FOUR))  { CellColorWHITE = GREEN; strncpy(ColorP1, "GREEN", 16); }
        if (!CellBoardColorChanger && IsKeyPressed(KEY_FIVE))  { CellColorWHITE = BLUE; strncpy(ColorP1, "BLUE", 16); }
        if (!CellBoardColorChanger && IsKeyPressed(KEY_SIX))   { CellColorWHITE = PINK; strncpy(ColorP1, "MAGENTA", 16); }
        if (!CellBoardColorChanger && IsKeyPressed(KEY_SEVEN)) { CellColorWHITE = PURPLE; strncpy(ColorP1, "PURPLE", 16); }
        if (!CellBoardColorChanger && IsKeyPressed(KEY_EIGHT)) { CellColorWHITE = WHITE; strncpy(ColorP1, "WHITE", 16); }
        if (!CellBoardColorChanger && IsKeyPressed(KEY_NINE))  { CellColorWHITE = BROWN; strncpy(ColorP1, "BROWN", 16); }
        
        // CellBoard -> Defaults to BLACK (changed accordingly)
        if (CellBoardColorChanger  && IsKeyPressed(KEY_ZERO))  { CellColorBLACK = BLACK; strncpy(ColorP2, "BLACK", 16); }
        if (CellBoardColorChanger  && IsKeyPressed(KEY_ONE))   { CellColorBLACK = RED; strncpy(ColorP2, "RED", 16); }
        if (CellBoardColorChanger  && IsKeyPressed(KEY_TWO))   { CellColorBLACK = ORANGE; strncpy(ColorP2, "ORANGE", 16); }
        if (CellBoardColorChanger  && IsKeyPressed(KEY_THREE)) { CellColorBLACK = GOLD; strncpy(ColorP2, "GOLD", 16); }
        if (CellBoardColorChanger  && IsKeyPressed(KEY_FOUR))  { CellColorBLACK = GREEN; strncpy(ColorP2, "GREEN", 16); }
        if (CellBoardColorChanger  && IsKeyPressed(KEY_FIVE))  { CellColorBLACK = BLUE; strncpy(ColorP2, "BLUE", 16); }
        if (CellBoardColorChanger  && IsKeyPressed(KEY_SIX))   { CellColorBLACK = PINK; strncpy(ColorP2, "MAGENTA", 16); }
        if (CellBoardColorChanger  && IsKeyPressed(KEY_SEVEN)) { CellColorBLACK = PURPLE; strncpy(ColorP2, "PURPLE", 16); }
        if (CellBoardColorChanger  && IsKeyPressed(KEY_EIGHT)) { CellColorBLACK = WHITE; strncpy(ColorP2, "WHITE", 16); }
        if (CellBoardColorChanger  && IsKeyPressed(KEY_NINE))  { CellColorBLACK = BROWN; strncpy(ColorP2, "BROWN", 16); }

        for (int i = 0; i < Cells.y; i++) {
            for (int j = 0; j < Cells.x; j++) {
                if (i % 2 == 0) {
                    if (j % 2 == 0) DrawRectangle(((GetScreenWidth()/2.0f) - ((CellArea*Cells.x)/1.75f)) + (CellArea*(j + 1)), ((GetScreenHeight()/2.0f) - ((CellArea*Cells.y)/1.75f)) + (CellArea*(i + 1)), CellArea, CellArea, CellColorBLACK);
                    if (j % 2 != 0) DrawRectangle(((GetScreenWidth()/2.0f) - ((CellArea*Cells.x)/1.75f)) + (CellArea*(j + 1)), ((GetScreenHeight()/2.0f) - ((CellArea*Cells.y)/1.75f)) + (CellArea*(i + 1)), CellArea, CellArea, CellColorWHITE);
                } if (i % 2 != 0) {
                    if (j % 2 == 0) DrawRectangle(((GetScreenWidth()/2.0f) - ((CellArea*Cells.x)/1.75f)) + (CellArea*(j + 1)), ((GetScreenHeight()/2.0f) - ((CellArea*Cells.y)/1.75f)) + (CellArea*(i + 1)), CellArea, CellArea, CellColorWHITE);
                    if (j % 2 != 0) DrawRectangle(((GetScreenWidth()/2.0f) - ((CellArea*Cells.x)/1.75f)) + (CellArea*(j + 1)), ((GetScreenHeight()/2.0f) - ((CellArea*Cells.y)/1.75f)) + (CellArea*(i + 1)), CellArea, CellArea, CellColorBLACK);
                }
            }
        }

        if (!CellBoardColorChanger) DrawText(TextFormat("%s %s | [P2]: %s", CellBoardColorChangeP1, ColorP1, ColorP2), (GetScreenWidth()/2.0f) - (MeasureText(TextFormat("%s %s | [P2]: %s", CellBoardColorChangeP1, ColorP1, ColorP2), FONTSIZE_36)/2.0f), (GetScreenHeight()/2.0f) - ((GetScreenHeight()/2.0f) - 40.0f), FONTSIZE_36, CellColorWHITE);
        if (CellBoardColorChanger)  DrawText(TextFormat("%s %s | [P1]: %s", CellBoardColorChangeP2, ColorP2, ColorP1), (GetScreenWidth()/2.0f) - (MeasureText(TextFormat("%s %s | [P1]: %s", CellBoardColorChangeP2, ColorP2, ColorP1), FONTSIZE_36)/2.0f), (GetScreenHeight()/2.0f) - ((GetScreenHeight()/2.0f) - 40.0f), FONTSIZE_36, CellColorBLACK);
        if (ElapsedTime < 5.0f) DrawText(ColorSwitchAccess, (GetScreenWidth()/2.0f) - (MeasureText(ColorSwitchAccess, FONTSIZE_32)/2.0f), (GetScreenHeight()/2.0f) + ((GetScreenHeight()/2.0f) - 115.0f), FONTSIZE_32, WHITE);
        DrawText(ColorChannels, (GetScreenWidth()/2.0f) - (MeasureText(ColorChannels, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) - ((GetScreenHeight()/2.0f) - 80.0f), FONTSIZE_28, LIGHTGRAY);
        DrawText(RestartAgainMessage, (GetScreenWidth()/2.0f) - (MeasureText(RestartAgainMessage, FONTSIZE_24)/2.0f), (GetScreenHeight()/2.0f) + ((GetScreenHeight()/2.0f) - 30.0f), FONTSIZE_24, WHITE);
        DrawText(CellBoardColorSwitcher, (GetScreenWidth()/2.0f) - (MeasureText(CellBoardColorSwitcher, FONTSIZE_24)/2.0f), (GetScreenHeight()/2.0f) + ((GetScreenHeight()/2.0f) - 60.0f), FONTSIZE_24, YELLOW);
        EndDrawing();
    }
}

void Mini2DObstaclesDemoWindow(void) {
    // Spike configuration type: HEIGHT
    // Set either of these values into the most upper point of a triangle (defined in _01).
    float SpikeHeight60Deg = 0.0f, SpikeHeight30Deg = 30.0f;

    // Spike configuration type: WIDTH
    // Set either of these values into the most upper point of a triangle (defined in both _02 and _03).
    float SpikeWidth = 60.0f;
    
    // Spike configuration type: ALPHA
    // Set the alpha channel for type SPIKE (used in DrawText() below).
    float SpikeHitboxAlpha = 0.0f;

    /* DO NOT CHANGE ANY OF THESE "Vector2"" VALUES UNLESS YOU KNOW WHAT YOU'RE DOING! */
    /* DO NOT CHANGE ANY OF THESE "Vector2"" VALUES UNLESS YOU KNOW WHAT YOU'RE DOING! */
    /* DO NOT CHANGE ANY OF THESE "Vector2"" VALUES UNLESS YOU KNOW WHAT YOU'RE DOING! */

    Vector2 PlatformSize              = {.x = (float)GetScreenWidth(), .y = 60.f};
    Vector2 PlatformPosition          = {.x = 0.0f, .y = (float)GetScreenHeight() - 30.0f};
    Vector2 PlatformPosition_ORIGINAL = {.x = 0.0f, .y = (float)GetScreenHeight() - 30.0f};

    Vector2 PlayerSize     = {.x = 75.0f, .y = 75.0f};
    Vector2 PlayerPosition = {.x = ((float)GetScreenWidth() / 2.0f) - (PlayerSize.x / 2.0f), .y = PlatformPosition.y - 75.0f};
    
    Vector2 SpikesSizeV1_01         = {.x = (GetScreenWidth()/4.0f) + 0.0f, .y = ((PlatformPosition.y - 75.0f) + 15.0f) + SpikeHeight60Deg};
    Vector2 SpikesSizeV2_01         = {.x = (GetScreenWidth()/4.0f) - 30.0f, .y = ((PlatformPosition.y - 75.0f) + 15.0f) + SpikeWidth};
    Vector2 SpikesSizeV3_01         = {.x = (GetScreenWidth()/4.0f) + 30.0f, .y = ((PlatformPosition.y - 75.0f) + 15.0f) + SpikeWidth};
    Vector2 SpikesHitboxSize_01     = {.x = SpikeWidth/2.0f, .y = SpikeWidth};
    Vector2 SpikesHitboxPosition_01 = {.x = (SpikesSizeV2_01.x + SpikesHitboxSize_01.x) - (SpikeWidth/4.0f), .y = SpikesSizeV2_01.y - SpikeWidth + 5.0f};

    Vector2 SpikesSizeV1_02         = {.x = (GetScreenWidth()/4.0f + 60.0f) + 0.0f, .y = ((PlatformPosition.y - 75.0f) + 15.0f) + SpikeHeight60Deg};
    Vector2 SpikesSizeV2_02         = {.x = (GetScreenWidth()/4.0f + 60.0f) - 30.0f, .y = ((PlatformPosition.y - 75.0f) + 15.0f) + SpikeWidth};
    Vector2 SpikesSizeV3_02         = {.x = (GetScreenWidth()/4.0f + 60.0f) + 30.0f, .y = ((PlatformPosition.y - 75.0f) + 15.0f) + SpikeWidth};
    Vector2 SpikesHitboxSize_02     = {.x = SpikeWidth/2.0f, .y = SpikeWidth};
    Vector2 SpikesHitboxPosition_02 = {.x = (SpikesSizeV2_02.x + SpikesHitboxSize_02.x) - (SpikeWidth/4.0f), .y = SpikesSizeV2_02.y - SpikeWidth + 5.0f};

    Vector2 SpikesSizeV1_03         = {.x = (GetScreenWidth()/1.5f + 90.0f) + 0.0f, .y = ((PlatformPosition.y - 75.0f) + 15.0f) + SpikeHeight60Deg};
    Vector2 SpikesSizeV2_03         = {.x = (GetScreenWidth()/1.5f + 90.0f) - 30.0f, .y = ((PlatformPosition.y - 75.0f) + 15.0f) + SpikeWidth};
    Vector2 SpikesSizeV3_03         = {.x = (GetScreenWidth()/1.5f + 90.0f) + 30.0f, .y = ((PlatformPosition.y - 75.0f) + 15.0f) + SpikeWidth};
    Vector2 SpikesHitboxSize_03     = {.x = SpikeWidth/2.0f, .y = SpikeWidth};
    Vector2 SpikesHitboxPosition_03 = {.x = (SpikesSizeV2_03.x + SpikesHitboxSize_03.x) - (SpikeWidth/4.0f), .y = SpikesSizeV2_03.y - SpikeWidth + 5.0f};

    Vector2 ObstacleSize_01         = {.x = (float)GetScreenWidth() - (SpikesHitboxPosition_03.x), .y = 120.0f};
    Vector2 ObstacleSize_02         = {.x = ((float)GetScreenWidth() - (SpikesHitboxPosition_03.x) - 360.0f), .y = 180.0f};
    Vector2 ObstacleSize_03         = {.x = 210.0f, .y = 60.0f};
    Vector2 ObstacleSize_04         = {.x = (SpikesHitboxPosition_01.x - (SpikeWidth/2.0f) + 20.0f), .y = 365.0f};
    Vector2 ObstaclePosition_01     = {.x = (SpikesHitboxPosition_03.x + (SpikeWidth/1.5f) + 5.0f), .y = (PlatformPosition.y - ObstacleSize_01.y)};
    Vector2 ObstaclePosition_02     = {.x = (SpikesHitboxPosition_03.x + (SpikeWidth/1.5f) + ObstacleSize_02.x + 120.0f), .y = (PlatformPosition.y - ObstacleSize_02.y)};
    Vector2 ObstaclePosition_03     = {.x = (810.0f), .y = 810.0f};
    Vector2 ObstaclePosition_04     = {.x = ((SpikesHitboxPosition_01.x - (SpikeWidth/1.5f) - ObstacleSize_04.x + 25.0f)), .y = (PlatformPosition.y - ObstacleSize_04.y)};

    /* DO NOT CHANGE ANY OF THESE "Vector2"" VALUES UNLESS YOU KNOW WHAT YOU'RE DOING! */
    /* DO NOT CHANGE ANY OF THESE "Vector2"" VALUES UNLESS YOU KNOW WHAT YOU'RE DOING! */
    /* DO NOT CHANGE ANY OF THESE "Vector2"" VALUES UNLESS YOU KNOW WHAT YOU'RE DOING! */

    /* IMPORTANT VARIABLES AND VALUES FOR THE CORE MECHANICS */
    /* IMPORTANT VARIABLES AND VALUES FOR THE CORE MECHANICS */
    /* IMPORTANT VARIABLES AND VALUES FOR THE CORE MECHANICS */

    int MaxJumpHeight = 175, MaxJumpHeight_ORIGINAL = 175;
    int FramesCounter = 0, TextBlinkingTime = 0;
    float dt = 0.0f, ElapsedTime = 0.0f, FramesJumpCounter = 0.0f;
    float PlayerVelocity = 12.5f, SprintingMultiplier = 0.75f, DisplayPlayerVelocity = 12.5f;
    float PlatformFrictionAcceleration = 50.0f, GravitationalAcceleration = 12.5f;
    bool IsFullscreen = false, IsJumping = false, IsPaused = false, IsCrashed = false, IsVisited = false;

    /* IMPORTANT VARIABLES AND VALUES FOR THE CORE MECHANICS */
    /* IMPORTANT VARIABLES AND VALUES FOR THE CORE MECHANICS */
    /* IMPORTANT VARIABLES AND VALUES FOR THE CORE MECHANICS */

    const char* PauseMessage      = "Press [P] again to continue...";
    const char* MovementGuide01   = "Use [A][D] OR [<][>] (ARROW keys) on your keyboard to MOVE around!";
    const char* MovementGuide02   = "Press/hold down [SPACE] to JUMP, and lastly hold down [SHIFT] to SPRINT!";
    const char* UpdateVariables01 = "(see for commented lines/codes in the \"void Mini2DObstaclesDemoWindow(void)\"";
    const char* UpdateVariables02 = "function to change some of the values internally...)";
    const char* PlayerCrashed     = "You've crashed! Press [ENTER] to try again...";

    const char* PlayerPosMessage  = "PlayerPosition (x: px, y: px):";
    const char* PlayerVelMessage  = "PlayerVelocity (x: px/s):";
    const char* MaxJumpHMessage   = "MaxJumpHeight (x: px):";
    const char* GravitationalACC  = "GravitationalAcceleration (x: px/s^2):";
    const char* PlatFrictionACC   = "PlatformFrictionAcceleration (x: px/s^2):";
    const char* SprintingACC      = "SprintingMultiplier (x: px/s^2):";
    
    while (!WindowShouldClose()) {
        if (!IsVisited && ElapsedTime > 5.0f) IsVisited = true;
        dt = GetFrameTime();
        ElapsedTime += GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!IsFullscreen && IsKeyPressed(KEY_F)) { ToggleFullscreen(); IsFullscreen = !IsFullscreen; }
        if (IsFullscreen && IsKeyPressed(KEY_F))  { ToggleBorderlessWindowed(); IsFullscreen = !IsFullscreen; }

        if (!IsCrashed && IsKeyPressed(KEY_P)) IsPaused = !IsPaused;
        if (IsCrashed && IsKeyPressed(KEY_ENTER)) {
            PlayerSize.x     = 75.0f; PlayerSize.y = 75.0f;
            PlayerPosition.x = ((float)GetScreenWidth() / 2.0f) - (PlayerSize.x / 2.0f); PlayerPosition.y = PlatformPosition.y - 75.0f;
            
            ElapsedTime      = 0.0f;
            IsJumping = false; IsPaused = false; IsCrashed = false;
            FramesCounter = 0; TextBlinkingTime = 0; FramesJumpCounter = 0.0f;
        }

        if (!IsPaused && !IsCrashed) {
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                if (PlayerPosition.x >= PlatformPosition.x) { PlayerPosition.x -= (PlatformFrictionAcceleration * (dt*PlayerVelocity)); DisplayPlayerVelocity = PlayerVelocity; }
            } if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                if (PlayerPosition.x <= ((float)GetScreenWidth() - PlayerSize.x)) { PlayerPosition.x += (PlatformFrictionAcceleration * (dt*PlayerVelocity)); DisplayPlayerVelocity = PlayerVelocity; }
            } if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))) {
                if (PlayerPosition.x >= PlatformPosition.x) { PlayerPosition.x -= ((SprintingMultiplier) * (PlatformFrictionAcceleration * (dt*PlayerVelocity))); DisplayPlayerVelocity = (PlayerVelocity + (PlayerVelocity*SprintingMultiplier)); }
            } if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))) {
                if (PlayerPosition.x <= ((float)GetScreenWidth() - PlayerSize.x)) { PlayerPosition.x += ((SprintingMultiplier) * (PlatformFrictionAcceleration * (dt*PlayerVelocity))); DisplayPlayerVelocity = (PlayerVelocity + (PlayerVelocity*SprintingMultiplier)); }
            } if (IsKeyDown(KEY_SPACE)) {
                if (PlayerPosition.y >= (PlatformPosition.y - 75.0f)) IsJumping = true;
            }

            if (IsJumping) {
                if (FramesJumpCounter <= MaxJumpHeight) { PlayerPosition.y -= (PlatformFrictionAcceleration * (dt*GravitationalAcceleration)); FramesJumpCounter += (PlatformFrictionAcceleration * (dt*GravitationalAcceleration)); }
                if (FramesJumpCounter > MaxJumpHeight)  { FramesJumpCounter = 0.0f; IsJumping = false; }
            } if (!IsJumping) {
                if (PlayerPosition.y < (PlatformPosition.y - 75.0f))  PlayerPosition.y += (PlatformFrictionAcceleration * (dt*GravitationalAcceleration));
                if (PlayerPosition.y >= (PlatformPosition.y - 75.0f)) PlayerPosition.y = (PlatformPosition.y - 75.0f);
            }
        } else FramesCounter++;
        
        //// TODO: Make a falling spikes animation from above using these calculations...
        // if (PlayerPosition.y >= (PlatformPosition.y - 75.0f)) PlayerPosition.y -= (PlayerPosition.y + (PlatformPosition.y - 75.0f));
        
        // Check for crash with spikes.
        if (PlayerPosition.x <= (SpikesHitboxPosition_01.x + (SpikeWidth/4.0f)*2.25f) && PlayerPosition.x >= ((SpikesHitboxPosition_01.x + (SpikeWidth/4.0f)*2.25f) - PlayerSize.x - (SpikeWidth/1.5f)) && \
            (PlayerPosition.y + PlayerSize.y) >= (SpikesHitboxPosition_01.y)) IsCrashed = true;
        if (PlayerPosition.x <= (SpikesHitboxPosition_02.x + (SpikeWidth/4.0f)*2.25f) && PlayerPosition.x >= ((SpikesHitboxPosition_02.x + (SpikeWidth/4.0f)*2.25f) - PlayerSize.x - (SpikeWidth/1.5f)) && \
            (PlayerPosition.y + PlayerSize.y) >= (SpikesHitboxPosition_02.y)) IsCrashed = true;
        if (PlayerPosition.x <= (SpikesHitboxPosition_03.x + (SpikeWidth/4.0f)*2.25f) && PlayerPosition.x >= ((SpikesHitboxPosition_03.x + (SpikeWidth/4.0f)*2.25f) - PlayerSize.x - (SpikeWidth/1.5f)) && \
            (PlayerPosition.y + PlayerSize.y) >= (SpikesHitboxPosition_03.y)) IsCrashed = true;

        // Check for collisions with obstacles.
        // printf("PlayerPosition.x = %f, %f\n", (PlayerPosition.x + PlayerSize.x), (ObstaclePosition_04.x + ObstacleSize_04.x + 75.0f));
        // printf("PlayerPosition.y = %f, %f, %f, %f\n", (PlayerPosition.y - PlayerSize.y), (ObstaclePosition_01.y - ObstacleSize_01.y - 30.0f), (ObstaclePosition_02.y - ObstacleSize_02.y + 30.0f), (ObstaclePosition_03.y - ObstacleSize_03.y));
        if ((PlayerPosition.x + PlayerSize.x) >= (ObstaclePosition_01.x) && (PlayerPosition.x + PlayerSize.x) <= (ObstaclePosition_02.x) && (PlayerPosition.y - PlayerSize.y) <= (ObstaclePosition_01.y)) {
            PlatformPosition.y = (ObstaclePosition_01.y);
        } else if ((PlayerPosition.x + PlayerSize.x) > (ObstaclePosition_02.x)) {
            if ((PlayerPosition.y - PlayerSize.y) > ObstaclePosition_02.y - ObstacleSize_02.y + 30.0f) PlayerPosition.x = (ObstaclePosition_02.x - PlayerSize.x);
            else PlatformPosition.y = (ObstaclePosition_02.y);
        } else if ((PlayerPosition.x + PlayerSize.x) > (ObstaclePosition_03.x) && (PlayerPosition.x + PlayerSize.x) < (ObstaclePosition_03.x + ObstacleSize_03.x + PlayerSize.x)) {
            if ((PlayerPosition.y - PlayerSize.y) <= (ObstaclePosition_03.y - ObstacleSize_03.y - 30.0f)) PlatformPosition.y = (ObstaclePosition_03.y);
            else if ((PlayerPosition.y - PlayerSize.y) > (ObstaclePosition_03.y - ObstacleSize_03.y + 30.0f)) {
                MaxJumpHeight = (PlayerPosition.y - (ObstaclePosition_03.y - ObstacleSize_03.y));
            }
        } else if ((PlayerPosition.x + PlayerSize.x) < (ObstaclePosition_04.x + ObstacleSize_04.x + 75.0f)) {
            // PlatformPosition.x = (ObstaclePosition_04.x + ObstacleSize_04.x + 75.0f);
            if ((PlayerPosition.y + PlayerSize.y) > (ObstaclePosition_04.y)) PlayerPosition.x = (ObstaclePosition_04.x + ObstacleSize_04.x);
            else if ((PlayerPosition.y + PlayerSize.y) < ObstaclePosition_04.y) {
                PlatformPosition.y = (ObstaclePosition_04.y);
            }
        } else { PlatformPosition.x = PlatformPosition_ORIGINAL.x; PlatformPosition.y = PlatformPosition_ORIGINAL.y; MaxJumpHeight = MaxJumpHeight_ORIGINAL; }

        // printf("%d\n", MaxJumpHeight);
        DrawRectangleV(PlatformPosition_ORIGINAL, PlatformSize, DARKBLUE);
        DrawRectangleV(PlayerPosition, PlayerSize, SKYBLUE);

        // Draw the spikes sample.
        DrawTriangle(SpikesSizeV1_01, SpikesSizeV2_01, SpikesSizeV3_01, MAROON);
        DrawTriangle(SpikesSizeV1_02, SpikesSizeV2_02, SpikesSizeV3_02, MAROON);
        DrawTriangle(SpikesSizeV1_03, SpikesSizeV2_03, SpikesSizeV3_03, MAROON);

        // Draw the hitboxes of each spikes (find better way to check the collision).
        DrawRectangleV(SpikesHitboxPosition_01, SpikesHitboxSize_01, ColorAlpha(RED, SpikeHitboxAlpha));
        DrawRectangleV(SpikesHitboxPosition_02, SpikesHitboxSize_02, ColorAlpha(RED, SpikeHitboxAlpha));
        DrawRectangleV(SpikesHitboxPosition_03, SpikesHitboxSize_03, ColorAlpha(RED, SpikeHitboxAlpha));

        // Draw the obstacles.
        DrawRectangleV(ObstaclePosition_01, ObstacleSize_01, DARKBLUE);
        DrawRectangleV(ObstaclePosition_02, ObstacleSize_02, DARKBLUE);
        DrawRectangleV(ObstaclePosition_03, ObstacleSize_03, DARKBLUE);
        DrawRectangleV(ObstaclePosition_04, ObstacleSize_04, DARKBLUE);
        
        TextBlinkingTime = FramesCounter / BLINKING;
        if (IsCrashed) DrawText(PlayerCrashed, ((GetScreenWidth()/2.0f) - (MeasureText(PlayerCrashed, FONTSIZE_36)/2.0f)), ((GetScreenHeight()/2.0f)), FONTSIZE_36, RED);
        if (IsPaused && (TextBlinkingTime % 2 == 0)) DrawText(PauseMessage, ((GetScreenWidth()/2.0f) - (MeasureText(PauseMessage, FONTSIZE_32)/2.0f)), ((GetScreenHeight()/2.0f)), FONTSIZE_32, BLACK);
        DrawFPS(20, 20);

        DrawText(TextFormat("%s (%.0fpx, %.0fpx)", PlayerPosMessage, PlayerPosition.x, PlayerPosition.y), 20, 50, FONTSIZE_20, RED);
        DrawText(TextFormat("%s %.2fpx/s", PlayerVelMessage, DisplayPlayerVelocity), 20, 80, FONTSIZE_20, GREEN);
        DrawText(TextFormat("%s %dpx", MaxJumpHMessage, MaxJumpHeight), 20, 110, FONTSIZE_20, BLUE);
        
        DrawText(TextFormat("%s %.2fpx/s^2", GravitationalACC, GravitationalAcceleration), 20, 170, FONTSIZE_20, RED);
        DrawText(TextFormat("%s %.2fpx/s^2", PlatFrictionACC, PlatformFrictionAcceleration), 20, 200, FONTSIZE_20, GREEN);
        DrawText(TextFormat("%s %.2fpx/s^2", SprintingACC, SprintingMultiplier), 20, 230, FONTSIZE_20, BLUE);

        if (!IsVisited && ElapsedTime < 5.0f) DrawText(MovementGuide01, (GetScreenWidth()/2.0f) - (MeasureText(MovementGuide01, FONTSIZE_40)/2.0f), (GetScreenHeight()/2.0f) - 240, FONTSIZE_40, BLACK);
        if (!IsVisited && ElapsedTime < 5.0f) DrawText(MovementGuide02, (GetScreenWidth()/2.0f) - (MeasureText(MovementGuide02, FONTSIZE_40)/2.0f), (GetScreenHeight()/2.0f) - 200, FONTSIZE_40, BLACK);
        if (!IsVisited && ElapsedTime < 5.0f) DrawText(UpdateVariables01, (GetScreenWidth()/2.0f) - (MeasureText(UpdateVariables01, FONTSIZE_32)/2.0f), (GetScreenHeight()/2.0f) - 100, FONTSIZE_32, GRAY);
        if (!IsVisited && ElapsedTime < 5.0f) DrawText(UpdateVariables02, (GetScreenWidth()/2.0f) - (MeasureText(UpdateVariables02, FONTSIZE_32)/2.0f), (GetScreenHeight()/2.0f) - 60, FONTSIZE_32, GRAY);

        EndDrawing();
    }
}

void MainMenuWindow(void) {
    InitWindow(WIDTH, HEIGHT, "EintsWaveX's Raylib!");
    // SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowFocused();
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetTargetFPS(60);

    Color BGColor = {225, 234, 234, 255};
    int GameChoice = 1, FeaturedGameDemo = 4;
    bool IsFullscreen = false;
    
    const char* TitleMessage    = "Welcome to EintsWaveX's Raylib Demo!";
    const char* TMDescription01 = "Here are some demo projects of small game creations";
    const char* TMDescription02 = "in Raylib using pure C! Hope you enjoyed...";
    const char* GameDemo01      = "Bouncing Ball Animation (DVD screenlike)";
    const char* GameDemo02      = "Moving Ball Across The Screen (Player 2.5D-like)";
    const char* GameDemo03      = "Chessboard Generator (Color customization)";
    const char* GameDemo04      = "Mini 2D Obstacles (Spikes, Platforms, etc)";
    const char* FullscreenHelp  = "Press [F] to toggle the fullscreen mode (and vice versa)...";

    while (!WindowShouldClose()) {
        if (!IsFullscreen && IsKeyPressed(KEY_F)) { ToggleFullscreen(); IsFullscreen = !IsFullscreen; }
        if (IsFullscreen && IsKeyPressed(KEY_F))  { ToggleBorderlessWindowed(); IsFullscreen = !IsFullscreen; }

        BeginDrawing();
        ClearBackground(BGColor);
        
        DrawText(TitleMessage, (GetScreenWidth()/2.0f) - (MeasureText(TitleMessage, FONTSIZE_40)/2.0f), (GetScreenHeight()/2.0f) - ((GetScreenHeight()/2.0f) - 100), FONTSIZE_40, BLACK);
        DrawText(TMDescription01, (GetScreenWidth()/2.0f) - (MeasureText(TMDescription01, FONTSIZE_32)/2.0f), (GetScreenHeight()/2.0f) - ((GetScreenHeight()/2.0f) - 150), FONTSIZE_32, GRAY);
        DrawText(TMDescription02, (GetScreenWidth()/2.0f) - (MeasureText(TMDescription02, FONTSIZE_32)/2.0f), (GetScreenHeight()/2.0f) - ((GetScreenHeight()/2.0f) - 180), FONTSIZE_32, GRAY);
        DrawText(FullscreenHelp, (GetScreenWidth()/2.0f) - (MeasureText(FullscreenHelp, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + ((GetScreenHeight()/2.0f) - 30), FONTSIZE_28, GRAY);

        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && GameChoice - 1 != 0) GameChoice--;
        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && GameChoice + 1 != (FeaturedGameDemo + 1)) GameChoice++;
        
        switch (GameChoice) {
            case 1: {
                DrawText(TextFormat("> %s <", GameDemo01), (GetScreenWidth()/2.0f) - (MeasureText(TextFormat("> %s <", GameDemo01), FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f), FONTSIZE_28, RED);
                DrawText(TextFormat("%s", GameDemo02), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo02, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 30, FONTSIZE_28, BLACK);
                DrawText(TextFormat("%s", GameDemo03), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo03, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 60, FONTSIZE_28, BLACK);
                DrawText(TextFormat("%s", GameDemo04), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo04, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 90, FONTSIZE_28, BLACK);
                break;
            } case 2: {
                DrawText(TextFormat("%s", GameDemo01), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo01, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f), FONTSIZE_28, BLACK);
                DrawText(TextFormat("> %s <", GameDemo02), (GetScreenWidth()/2.0f) - (MeasureText(TextFormat("> %s <", GameDemo02), FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 30, FONTSIZE_28, ORANGE);
                DrawText(TextFormat("%s", GameDemo03), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo03, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 60, FONTSIZE_28, BLACK);
                DrawText(TextFormat("%s", GameDemo04), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo04, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 90, FONTSIZE_28, BLACK);
                break;
            } case 3: {
                DrawText(TextFormat("%s", GameDemo01), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo01, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f), FONTSIZE_28, BLACK);
                DrawText(TextFormat("%s", GameDemo02), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo02, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 30, FONTSIZE_28, BLACK);
                DrawText(TextFormat("> %s <", GameDemo03), (GetScreenWidth()/2.0f) - (MeasureText(TextFormat("> %s <", GameDemo03), FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 60, FONTSIZE_28, GOLD);
                DrawText(TextFormat("%s", GameDemo04), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo04, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 90, FONTSIZE_28, BLACK);
                break;
            } case 4: {
                DrawText(TextFormat("%s", GameDemo01), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo01, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f), FONTSIZE_28, BLACK);
                DrawText(TextFormat("%s", GameDemo02), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo02, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 30, FONTSIZE_28, BLACK);
                DrawText(TextFormat("%s", GameDemo03), (GetScreenWidth()/2.0f) - (MeasureText(GameDemo03, FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 60, FONTSIZE_28, BLACK);
                DrawText(TextFormat("> %s <", GameDemo04), (GetScreenWidth()/2.0f) - (MeasureText(TextFormat("> %s <", GameDemo04), FONTSIZE_28)/2.0f), (GetScreenHeight()/2.0f) + 90, FONTSIZE_28, LIME);
                break;
            } default: {
                break;
            }
        }

        if (IsKeyPressed(KEY_ENTER)) {
            switch (GameChoice) {
                case 1: { BouncingBallDemoWindow(); break; }
                case 2: { MovingAroundBallDemoWindow(); break; }
                case 3: { ChessboardGeneratorDemoWindow(); break; }
                case 4: { Mini2DObstaclesDemoWindow(); break; }
                default: break;
            }
        }
        
        if (IsKeyPressed(KEY_ESCAPE)) {

        }
        EndDrawing();
    } CloseWindow();
}

void ConfirmExitWindow(void) {

}

int main(void) {
    MainMenuWindow();
    exit(0);
    return 0;
}