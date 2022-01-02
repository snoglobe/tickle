# Tickle - Fulltest Example
# Tests WindowTitle, Pixel, Line, Rect, Sprite, Text, Input, and Color. 
# By: shanepaton

set WindowTitle "Tickle - FullTest"

proc begin {} {
    LoadSprite "./test.png" 0
}

proc step {} {

}

proc draw {} {
    global Black
    global White
    global Purple
    global Orange
    global Gold
    global Yellow
    global Brown
    global LightBrown
    global Blue
    global LightBlue
    global Green
    global LightGreen
    global Maroon
    global Red
    global Pink
    global Beige
    
    # Set Clear Color to Black
    Clear $Black

    # Draw Text with all Colors
    Text 0 0 "Greetings from Tickle!" $White
    Text 2 8 "Greetings from Tickle!" $Purple
    Text 4 16 "Greetings from Tickle!" $Orange
    Text 6 24 "Greetings from Tickle!" $Gold
    Text 8 32 "Greetings from Tickle!" $Yellow
    Text 10 40 "Greetings from Tickle!" $Brown
    Text 12 48 "Greetings from Tickle!" $LightBrown
    Text 14 56 "Greetings from Tickle!" $Blue
    Text 16 64 "Greetings from Tickle!" $LightBlue
    Text 18 72 "Greetings from Tickle!" $Green
    Text 20 80 "Greetings from Tickle!" $LightGreen
    Text 22 88 "Greetings from Tickle!" $Maroon
    Text 24 96 "Greetings from Tickle!" $Red
    Text 26 104 "Greetings from Tickle!" $Pink
    Text 28 112 "Greetings from Tickle!" $Beige

    # Draw Sprite
    Sprite 0 [expr {256-16}] [expr {256-16}]

    # Draw Lines
    Line 0 128 64 128 $White
    Line 0 136 64 144 $White

    # Draw Pixels
    Pixel 0 152 $White
    Pixel 2 152 $White
    Pixel 4 152 $White
    Pixel 6 152 $White
    Pixel 8 152 $White
    Pixel 10 152 $White
    Pixel 12 152 $White
    Pixel 14 152 $White

    # Visual arrow key inputs
    if {[expr [KeyDown 263]] == 1} {
        Rect 0 240 16 16 $Blue
        Rect 4 244 8 8 $White
    } else {
        Rect 0 240 16 16 $White
        Rect 4 244 8 8 $Blue
    }

    if {[expr [KeyDown 265]] == 1} {
        Rect 18 222 16 16 $Blue
        Rect 22 226 8 8 $White

    } else {
        Rect 18 222 16 16 $White
        Rect 22 226 8 8 $Blue
    }

    if {[expr [KeyDown 264]] == 1} {
        Rect 18 240 16 16 $Blue
        Rect 22 244 8 8 $White
    } else {
        Rect 18 240 16 16 $White
        Rect 22 244 8 8 $Blue
    }
    
    if {[expr [KeyDown 262]] == 1} {
        Rect 36 240 16 16 $Blue
        Rect 40 244 8 8 $White
    } else {
        Rect 36 240 16 16 $White
        Rect 40 244 8 8 $Blue
    }
}