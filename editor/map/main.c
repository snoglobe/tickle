#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.c"
#include "../../palette.h"
#include <stdio.h>
#include <stdint.h>

uint8_t sprite[16][16];

int main(int argc, char* argv[])
{    
    int screenWidth = 512;
    int screenHeight = 512;

    InitWindow(screenWidth, screenHeight, "Tixel");

    SetTargetFPS(60);
    char* filename = calloc(sizeof(int), 256);
    strcpy(filename, "Filename");
    int pickedColor = 0;
    int tool = 0; // 0 = pencil, 1 = eraser

    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            sprite[x][y] = 17;
        }
    }

    GuiLoadStyle("tickle.rgs");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GetColor(IGetPColor(0)));
        for(int x = 0; x < 8; x++) {
            for(int y = 0; y < 2; y++) {
                if(GuiButton((Rectangle){280 + (x * 25), 20 + (y * 25), 24, 24}, "")) {
                    pickedColor = y > 0 ? x + 8 : x;
                }
            }        
        }
        for(int x = 0; x < 8; x++) {
            for(int y = 0; y < 2; y++) {
                GuiDrawRectangle((Rectangle){280 + (x * 25), 20 + (y * 25), 24, 24}, 1, WHITE, GetColor(IGetPColor(y > 0 ? x + 8 : x)));
            }        
        }
        GuiTextBox((Rectangle){20, 50, 130, 20}, filename, 255, true);
        GuiDrawRectangle((Rectangle){0, 502, 512, 10}, 1, GetColor(IGetPColor(pickedColor)), GetColor(IGetPColor(pickedColor)));
        if ( GuiButton( (Rectangle){ 20, 20, 60, 20 }, "#01#Load" ) ){
            FILE *f = fopen(filename, "rb");
            for(int x,i = 0; x < 16; x++){
                for(int y = 0; y < 16; y++){
                    fseek(f, i++, SEEK_SET);
                    sprite[x][y] = fgetc(f);
                }
            }
            fclose(f);
        }
        if ( GuiButton( (Rectangle){ 90, 20, 60, 20 }, "#02#Save" ) ){
            FILE *f = fopen(filename, "wb");
            fwrite(sprite, sizeof(char), sizeof(sprite), f);
            fclose(f);
        }
        if ( GuiButton( (Rectangle){ 160, 20, 50, 50 }, "#22#" ) ){
            tool = 0;
        }
        if ( GuiButton( (Rectangle){ 220, 20, 50, 50 }, "#28#" ) ){
            tool = 1;
        }
        for(int x = 0; x < 16; x++) {
            for(int y = 0; y < 16; y++) {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, (Rectangle){55 + (x * 25), 90 + (y * 25), 24, 24}))
                {
                    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                        if(!tool){
                            sprite[x][y] = pickedColor;
                        } else {
                            sprite[x][y] = 17;
                        }
                    }
                }
                GuiDrawRectangle((Rectangle){55 + (x * 25), 90 + (y * 25), 25, 25}, 0, (x + y) % 2 == 0 ? GRAY : WHITE, (x + y) % 2 == 0 ? GRAY : WHITE);
                GuiDrawRectangle((Rectangle){55 + (x * 25), 90 + (y * 25), 25, 25}, 0, GetColor(IGetPColor(sprite[x][y])), GetColor(IGetPColor(sprite[x][y])));
            }        
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}