set WindowTitle "Tickle - FullTest"

proc begin {} {
    LoadSprite "./test.png" 0
    LoadSound "./examples/fulltest/test.wav" 0
}

proc step {} {

}

proc draw {} {
    
    # Set Clear Color to Black
    Clear $::Black

    Rect 0 0 114 8 1
    for {set i 0} {$i < 15} {incr i} {
         Text [expr {$i * 2}] [expr {$i * 8}] "Greetings from Tickle!" $i
    }

    # Draw Sprite
    Sprite 0 [expr {256-16}] [expr {256-16}]

    # Draw Lines
    Line 0 128 128 128 1 $::White
    Line 0 136 128 144 2 $::White

    Text 240 0 [GetFPS] $::White

    Circle 16 172 16 $::White
    Rect 36 156 32 32 $::White

    # Draw Pixels
    for {set x 0} { $x < 128 } { incr x; incr x } {
        Pixel $x 152 $::White
    }

    # Visual arrow key inputs
    if {[expr [KeyDown 263]] == 1 || [expr [KeyDown 65]] == 1} {
        Rect 0 240 16 16 $::Blue
        Rect 4 244 8 8 $::White
    } else {
        Rect 0 240 16 16 $::White
        Rect 4 244 8 8 $::Blue
    }

    if {[expr [KeyDown 265]] == 1 || [expr [KeyDown 87]] == 1} {
        Rect 18 222 16 16 $::Blue
        Rect 22 226 8 8 $::White

    } else {
        Rect 18 222 16 16 $::White
        Rect 22 226 8 8 $::Blue
    }

    if {[expr [KeyDown 264]] == 1 || [expr [KeyDown 83]] == 1} {
        Rect 18 240 16 16 $::Blue
        Rect 22 244 8 8 $::White
    } else {
        Rect 18 240 16 16 $::White
        Rect 22 244 8 8 $::Blue
    }
    
    if {[expr [KeyDown 262]] == 1 || [expr [KeyDown 68]] == 1} {
        Rect 36 240 16 16 $::Blue
        Rect 40 244 8 8 $::White
    } else {
        Rect 36 240 16 16 $::White
        Rect 40 244 8 8 $::Blue
    }

    Text 64 246 "Press space to play sound" $::White
    if {[expr [KeyPress 32]] == 1} {
        PlaySound 0
    }
}