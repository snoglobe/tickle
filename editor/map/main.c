#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.c"
#include "../../palette.h"
#include <stdio.h>
#include <stdint.h>

static inline int GetPColor(int c) {
    switch(c){
        case 0:
            return (TBLACK); // black
        case 1: 
            return (TWHITE); // white
        case 2:
            return (TRED); // red
        case 3:
            return (TMAROON); // maroon
        case 4:
            return (TORANGE); // orange
        case 5:
            return (TGOLD); // gold
        case 6:  
            return (TYELLOW); // yellow
        case 7:
            return (TLGREEN); // light green
        case 8:
            return (TGREEN); // green
        case 9:
            return (TLBLUE); // light blue
        case 10:
            return (TBLUE); // blue
        case 11:
            return (TDPURPLE); // purple
        case 12:
            return (TPINK); // pink
        case 13:
            return (TBEIGE); // beige
        case 14:
            return (TLGREY); // light brown
        case 15:
            return (TDGREY); // brown
        case 17:
            return (0x00000000); // transparent
        default: 
            printf("invalid color %d", c);
            exit(EXIT_FAILURE);
    }
}

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
        ClearBackground(GetColor(GetPColor(0)));
        for(int x = 0; x < 8; x++) {
            for(int y = 0; y < 2; y++) {
                if(GuiButton((Rectangle){280 + (x * 25), 20 + (y * 25), 24, 24}, "")) {
                    pickedColor = y > 0 ? x + 8 : x;
                }
            }        
        }
        for(int x = 0; x < 8; x++) {
            for(int y = 0; y < 2; y++) {
                GuiDrawRectangle((Rectangle){280 + (x * 25), 20 + (y * 25), 24, 24}, 1, WHITE, GetColor(GetPColor(y > 0 ? x + 8 : x)));
            }        
        }
        GuiTextBox((Rectangle){20, 50, 130, 20}, filename, 255, true);
        GuiDrawRectangle((Rectangle){0, 502, 512, 10}, 1, GetColor(GetPColor(pickedColor)), GetColor(GetPColor(pickedColor)));
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
                GuiDrawRectangle((Rectangle){55 + (x * 25), 90 + (y * 25), 25, 25}, 0, GetColor(GetPColor(sprite[x][y])), GetColor(GetPColor(sprite[x][y])));
            }        
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}