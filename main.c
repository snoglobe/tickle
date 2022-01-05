#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "palette.h"
#include <stdint.h>
#include <inttypes.h>

#define JIM_EMBEDDED
#include <jim.h>
#include <raylib.h>

#define XSIZE 256
#define YSIZE 256

#define SPRITE_MAX 256
#define AUDIO_MAX 256

#define SPRITE_X 16
#define SPRITE_Y 16

#define FPS 60

Texture2D Sprites[SPRITE_MAX];
Sound Sounds[AUDIO_MAX];

#define max(a, b) ((a)>(b)? (a) : (b))
#define min(a, b) ((a)<(b)? (a) : (b))

Vector2 ClampValue(Vector2 value, Vector2 min, Vector2 max)
{
    Vector2 result = value;
    result.x = (result.x > max.x)? max.x : result.x;
    result.x = (result.x < min.x)? min.x : result.x;
    result.y = (result.y > max.y)? max.y : result.y;
    result.y = (result.y < min.y)? min.y : result.y;
    return result;
}


static inline Color GetPColor(int c) {
    switch(c){
        case 0:
            return GetColor(TBLACK); // black
        case 1: 
            return GetColor(TWHITE); // white
        case 2:
            return GetColor(TRED); // red
        case 3:
            return GetColor(TMAROON); // maroon
        case 4:
            return GetColor(TORANGE); // orange
        case 5:
            return GetColor(TGOLD); // gold
        case 6:  
            return GetColor(TYELLOW); // yellow
        case 7:
            return GetColor(TLGREEN); // light green
        case 8:
            return GetColor(TGREEN); // green
        case 9:
            return GetColor(TLBLUE); // light blue
        case 10:
            return GetColor(TBLUE); // blue
        case 11:
            return GetColor(TDPURPLE); // purple
        case 12:
            return GetColor(TPINK); // pink
        case 13:
            return GetColor(TBEIGE); // beige
        case 14:
            return GetColor(TLGREY); // light grey
        case 15:
            return GetColor(TDGREY); // dark grey
        case 17:
            return GetColor(0x00000000);
        default: 
            printf("invalid color %d", c);
            exit(EXIT_FAILURE);
    }
}

static int Pixel(Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
    long x;
    long y;
    long col;

    if(argc != 4) {
        Jim_WrongNumArgs(interp, 3, argv, "int, int, int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &x);
    Jim_GetLong(interp, argv[2], &y);
    Jim_GetLong(interp, argv[3], &col);

    assert(col < 0xFFFFFFFF);

    DrawPixel((int)x, (int)y, GetPColor((int)col));
    return (JIM_OK);
}   

static int Line(Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
    long x1;
    long y1;
    long x2;
    long y2;
    long t;
    long col;

    if(argc != 7) {
        Jim_WrongNumArgs(interp, 7, argv, "int, int, int, int, int, int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &x1);
    Jim_GetLong(interp, argv[2], &y1);
    Jim_GetLong(interp, argv[3], &x2);
    Jim_GetLong(interp, argv[4], &y2);
    Jim_GetLong(interp, argv[5], &t);
    Jim_GetLong(interp, argv[6], &col);

    Vector2 v1 = {(float)x1, (float)y1};
    Vector2 v2 = {(float)x2, (float)y2};

    DrawLineEx(v1, v2, (float)t + 0.0f, GetPColor((int)col));
    return (JIM_OK);
}

static int Rect(Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
    long x;
    long y;
    long w;
    long h;
    long col;

    if(argc != 6) {
        Jim_WrongNumArgs(interp, 6, argv, "int, int, int, int, int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &x);
    Jim_GetLong(interp, argv[2], &y);
    Jim_GetLong(interp, argv[3], &w);
    Jim_GetLong(interp, argv[4], &h);
    Jim_GetLong(interp, argv[5], &col);

    DrawRectangle((int)x, (int)y, (int)w, (int)h, GetPColor((int)col));
    return (JIM_OK);
}

static int Circle(Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
    long x;
    long y;
    long r;
    long col;

    if(argc != 5) {
        Jim_WrongNumArgs(interp, 5, argv, "int, int, int, int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &x);
    Jim_GetLong(interp, argv[2], &y);
    Jim_GetLong(interp, argv[3], &r);
    Jim_GetLong(interp, argv[4], &col);

    DrawCircle((int)x, (int)y, (int)r, GetPColor((int)col));
    return (JIM_OK);
}

static int Text(Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
    long x;
    long y;
    const char *text;
    int len;
    long col;

    if(argc != 5) {
        Jim_WrongNumArgs(interp, 4, argv, "int, int, string, int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &x);
    Jim_GetLong(interp, argv[2], &y);
    text = Jim_GetString(argv[3], &len);
    Jim_GetLong(interp, argv[4], &col);

    DrawText(text, (int)x, (int)y, 8, GetPColor((int)col));
    return (JIM_OK);
}

static int LoadSprite(Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
    const char *path;
    int len;
    long slot;

    if(argc != 3) {
        Jim_WrongNumArgs(interp, 3, argv, "string, int");
        return (JIM_ERR);
    }

    path = Jim_GetString(argv[1], &len);
    Jim_GetLong(interp, argv[2], &slot);

    unsigned int sprite[16][16];
    FILE *f = fopen(path, "rb");
    for(int x,i = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            fseek(f, i++, SEEK_SET);
            sprite[y][x] = __builtin_bswap32(IGetPColor(fgetc(f)));
        }
    }
    fclose(f);

    Image t = (Image){sprite, 16, 16, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8};

    Sprites[slot] = LoadTextureFromImage(t);
    return (JIM_OK);
}

static int KeyDown(Jim_Interp *interp, int argc, Jim_Obj *const *argv){
    long key;

    if(argc != 2) {
        Jim_WrongNumArgs(interp, 2, argv, "int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &key);

    Jim_SetResultBool(interp, IsKeyDown(key));
    return (JIM_OK);
}

static int PointIn(Jim_Interp *interp, int argc, Jim_Obj *const *argv){
    long x, y, radius, x2, y2;

    if(argc != 6) {
        Jim_WrongNumArgs(interp, 6, argv, "int, int, int, int, int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &x);
    Jim_GetLong(interp, argv[2], &y);
    Jim_GetLong(interp, argv[3], &radius);
    Jim_GetLong(interp, argv[4], &x2);
    Jim_GetLong(interp, argv[5], &y2);

    Jim_SetResultBool(interp, CheckCollisionPointCircle((Vector2){x2, y2}, (Vector2){x, y}, radius));
    return (JIM_OK);
}

static int KeyPress(Jim_Interp *interp, int argc, Jim_Obj *const *argv){
    long key;

    if(argc != 2) {
        Jim_WrongNumArgs(interp, 2, argv, "int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &key);

    Jim_SetResultBool(interp, IsKeyPressed(key));
    return (JIM_OK);
}

static int Sprite(Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
    long slot;
    long x;
    long y;

    if(argc != 4) {
        Jim_WrongNumArgs(interp, 4, argv, "int, int, int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &slot);
    Jim_GetLong(interp, argv[2], &x);
    Jim_GetLong(interp, argv[3], &y);

    DrawTexture(Sprites[slot], (int)x, (int)y, WHITE);
    return (JIM_OK);
}

static int Clear(Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
    long col;

    if(argc != 2) {
        Jim_WrongNumArgs(interp, 2, argv, "int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &col);

    ClearBackground(GetPColor(col));
    return (JIM_OK);
}

static int LoadSoundF(Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
    const char *path;
    int len;
    long slot;

    if(argc != 3) {
        Jim_WrongNumArgs(interp, 3, argv, "string, int");
        return (JIM_ERR);
    }

    path = Jim_GetString(argv[1], &len);
    Jim_GetLong(interp, argv[2], &slot);

    Sounds[slot] = LoadSound(path);
    return (JIM_OK);
}

static int PlaySoundF(Jim_Interp *interp, int argc, Jim_Obj *const *argv){
    long slot;

    if(argc != 2) {
        Jim_WrongNumArgs(interp, 3, argv, "int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &slot);
    PlaySound(Sounds[slot]);

    return (JIM_OK);
}

static int StopSoundF(Jim_Interp *interp, int argc, Jim_Obj *const *argv){
    long slot;

    if(argc != 2) {
        Jim_WrongNumArgs(interp, 3, argv, "int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &slot);
    StopSound(Sounds[slot]);

    return (JIM_OK);
}

static int GetFps(Jim_Interp *interp, int argc, Jim_Obj *const *argv){
    if(argc != 1){
        Jim_WrongNumArgs(interp, 1, argv, "");
        return (JIM_ERR);
    }

    Jim_SetResult(interp, Jim_NewIntObj(interp, GetFPS()));
    return (JIM_OK);
}

int main(void)
{
    Jim_Interp *interp;
    int error;

    interp = Jim_CreateInterp();
    assert(interp != NULL && "FATAL: Unable to create runtime.");
    
    Jim_RegisterCoreCommands(interp);
    Jim_InitStaticExtensions(interp);
    
    Jim_CreateCommand(interp, "Pixel",      Pixel,      NULL, NULL);
    Jim_CreateCommand(interp, "Line",       Line,       NULL, NULL);
    Jim_CreateCommand(interp, "Rect",       Rect,       NULL, NULL);
    Jim_CreateCommand(interp, "Circle",     Circle,     NULL, NULL);
    Jim_CreateCommand(interp, "Text",       Text,       NULL, NULL);
    Jim_CreateCommand(interp, "LoadSprite", LoadSprite, NULL, NULL);
    Jim_CreateCommand(interp, "LoadSound",  LoadSoundF, NULL, NULL);
    Jim_CreateCommand(interp, "PlaySound" , PlaySoundF, NULL, NULL);
    Jim_CreateCommand(interp, "StopSound",  StopSoundF, NULL, NULL);
    Jim_CreateCommand(interp, "Sprite",     Sprite,     NULL, NULL);
    Jim_CreateCommand(interp, "KeyDown",    KeyDown,    NULL, NULL);
    Jim_CreateCommand(interp, "KeyPress",   KeyPress,   NULL, NULL);
    Jim_CreateCommand(interp, "Clear",      Clear,      NULL, NULL);
    Jim_CreateCommand(interp, "GetFPS",     GetFps,     NULL, NULL);
    Jim_CreateCommand(interp, "PointIn",    PointIn,    NULL, NULL);

    Jim_SetGlobalVariableStr(interp, "White",      Jim_NewIntObj(interp, 1)); // 
    Jim_SetGlobalVariableStr(interp, "Black",      Jim_NewIntObj(interp, 0)); // 
    Jim_SetGlobalVariableStr(interp, "Purple",     Jim_NewIntObj(interp, 11)); //
    Jim_SetGlobalVariableStr(interp, "Orange",     Jim_NewIntObj(interp, 4)); //
    Jim_SetGlobalVariableStr(interp, "Gold",       Jim_NewIntObj(interp, 5)); //
    Jim_SetGlobalVariableStr(interp, "Yellow",     Jim_NewIntObj(interp, 6)); //
    Jim_SetGlobalVariableStr(interp, "Brown",      Jim_NewIntObj(interp, 15)); //
    Jim_SetGlobalVariableStr(interp, "LightBrown", Jim_NewIntObj(interp, 14)); //
    Jim_SetGlobalVariableStr(interp, "Blue",       Jim_NewIntObj(interp, 10)); //
    Jim_SetGlobalVariableStr(interp, "LightBlue",  Jim_NewIntObj(interp, 9)); //
    Jim_SetGlobalVariableStr(interp, "Green",      Jim_NewIntObj(interp, 8)); //
    Jim_SetGlobalVariableStr(interp, "LightGreen", Jim_NewIntObj(interp, 7)); //
    Jim_SetGlobalVariableStr(interp, "Maroon",     Jim_NewIntObj(interp, 3)); //
    Jim_SetGlobalVariableStr(interp, "Red",        Jim_NewIntObj(interp, 2)); //
    Jim_SetGlobalVariableStr(interp, "Pink",       Jim_NewIntObj(interp, 12)); //
    Jim_SetGlobalVariableStr(interp, "Beige",      Jim_NewIntObj(interp, 13)); //

    error = Jim_EvalFile(interp, "game.tcl");
    if(error == JIM_ERR) {
        Jim_MakeErrorMessage(interp);
        fprintf(stderr, "%s\n", Jim_GetString(Jim_GetResult(interp), NULL));
		Jim_FreeInterp(interp);
		exit(EXIT_FAILURE);
    }

    int len;
    const char* name = Jim_GetString(Jim_GetVariableStr(interp, "WindowTitle", 0), &len);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(512, 512, name);
    SetWindowMinSize(XSIZE, YSIZE);
    Image logo = LoadImage("logo.png");
    SetWindowIcon(logo);
    RenderTexture2D target = LoadRenderTexture(XSIZE, YSIZE);
    SetTextureFilter(target.texture, 0);
    SetTargetFPS(60);
    InitAudioDevice();

    error = Jim_Eval(interp, "begin");
    if(error == JIM_ERR) {
        Jim_MakeErrorMessage(interp);
        fprintf(stderr, "%s\n", Jim_GetString(Jim_GetResult(interp), NULL));
		Jim_FreeInterp(interp);
		exit(EXIT_FAILURE);
    }

    while(!WindowShouldClose())
    {
        ClearBackground(BLACK);
        float scale = min((float)GetScreenWidth()/XSIZE, (float)GetScreenHeight()/YSIZE);
        Jim_Eval(interp, "step");
        BeginDrawing();
        BeginTextureMode(target);
        error = Jim_Eval(interp, "draw");
        EndTextureMode();
        DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)XSIZE*scale))*0.5f, (GetScreenHeight() - ((float)YSIZE*scale))*0.5f,
                           (float)XSIZE*scale, (float)YSIZE*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
        if(error == JIM_ERR) {
            Jim_MakeErrorMessage(interp);
            fprintf(stderr, "%s\n", Jim_GetString(Jim_GetResult(interp), NULL));
		    Jim_FreeInterp(interp);
		    exit(EXIT_FAILURE);
        }
    }

    Jim_Eval(interp, "end");
    UnloadRenderTexture(target);
    CloseWindow();
    CloseAudioDevice();
    return 0;
}