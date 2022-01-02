#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    assert(x < XSIZE);
    assert(y < YSIZE);
    assert(col < 0xFFFFFFFF);

    DrawPixel((int)x, (int)y, GetColor((int)col));
    return (JIM_OK);
}   

static int Line(Jim_Interp *interp, int argc, Jim_Obj *const *argv) {
    long x1;
    long y1;
    long x2;
    long y2;
    long col;

    if(argc != 6) {
        Jim_WrongNumArgs(interp, 6, argv, "int, int, int, int, int");
        return (JIM_ERR);
    }

    Jim_GetLong(interp, argv[1], &x1);
    Jim_GetLong(interp, argv[2], &y1);
    Jim_GetLong(interp, argv[3], &x2);
    Jim_GetLong(interp, argv[4], &y2);
    Jim_GetLong(interp, argv[5], &col);

    assert(x1 < XSIZE);
    assert(y1 < YSIZE);
    assert(x2 < XSIZE);
    assert(y2 < YSIZE);

    DrawLine((int)x1, (int)y1, (int)x2, (int)y2, GetColor((int)col));
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

    assert(x < XSIZE);
    assert(y < YSIZE);
    assert(w < XSIZE);
    assert(h < YSIZE);

    DrawRectangle((int)x, (int)y, (int)w, (int)h, GetColor((int)col));
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

    DrawText(text, (int)x, (int)y, 8, GetColor((int)col));
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

    Sprites[slot] = LoadTexture(path);
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

    ClearBackground(GetColor(col));
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

    Jim_SetGlobalVariableStr(interp, "White",      Jim_NewIntObj(interp, 0xf2f2f9ff));
    Jim_SetGlobalVariableStr(interp, "Black",      Jim_NewIntObj(interp, 0x161423ff));
    Jim_SetGlobalVariableStr(interp, "Purple",     Jim_NewIntObj(interp, 0x6a5fa0ff));
    Jim_SetGlobalVariableStr(interp, "Orange",     Jim_NewIntObj(interp, 0xe76d14ff));
    Jim_SetGlobalVariableStr(interp, "Gold",       Jim_NewIntObj(interp, 0xedb329ff));
    Jim_SetGlobalVariableStr(interp, "Yellow",     Jim_NewIntObj(interp, 0xf7e26cff));
    Jim_SetGlobalVariableStr(interp, "Brown",      Jim_NewIntObj(interp, 0x6c251eff));
    Jim_SetGlobalVariableStr(interp, "LightBrown", Jim_NewIntObj(interp, 0x934226ff));
    Jim_SetGlobalVariableStr(interp, "Blue",       Jim_NewIntObj(interp, 0x1867a0ff));
    Jim_SetGlobalVariableStr(interp, "LightBlue",  Jim_NewIntObj(interp, 0x26ddddff));
    Jim_SetGlobalVariableStr(interp, "Green",      Jim_NewIntObj(interp, 0x126d30ff));
    Jim_SetGlobalVariableStr(interp, "LightGreen", Jim_NewIntObj(interp, 0x1fcb23ff));
    Jim_SetGlobalVariableStr(interp, "Maroon",     Jim_NewIntObj(interp, 0x98183cff));
    Jim_SetGlobalVariableStr(interp, "Red",        Jim_NewIntObj(interp, 0xd82323ff));
    Jim_SetGlobalVariableStr(interp, "Pink",       Jim_NewIntObj(interp, 0xe98472ff));
    Jim_SetGlobalVariableStr(interp, "Beige",      Jim_NewIntObj(interp, 0xf2c0a2ff));

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
    InitWindow(XSIZE, YSIZE, name);
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
        Jim_Eval(interp, "step");
        BeginDrawing();
        error = Jim_Eval(interp, "draw");
        EndDrawing();
        if(error == JIM_ERR) {
            Jim_MakeErrorMessage(interp);
            fprintf(stderr, "%s\n", Jim_GetString(Jim_GetResult(interp), NULL));
		    Jim_FreeInterp(interp);
		    exit(EXIT_FAILURE);
        }
    }

    Jim_Eval(interp, "end");
    CloseWindow();
    CloseAudioDevice();
    return 0;
}