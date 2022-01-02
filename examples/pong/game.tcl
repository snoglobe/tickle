set WindowTitle "pnog"

set ballX 128
set ballY 128
set ballXV 1
set ballYV 1

set score 0

set paddleY 128

# Run when the game starts (put initalization here)
proc begin {} {
    LoadSound "./click.wav" 0
    LoadSound "./synth.wav" 1
    LoadSound "./wall.wav" 2
}

# Run before Draw (put your logic here)
proc step {} {
    global paddleY
    global ballX
    global ballY
    global ballXV
    global ballYV
    global score

    if {[KeyDown 265] == 1} {
        set paddleY [expr {$paddleY - 2}]
    } elseif {[KeyDown 264] == 1} {
        set paddleY [expr {$paddleY + 2}]
    }

    if {$ballY == 255} {
        set ballYV [expr {0 - $ballYV}]
        PlaySound 2
    }

    if {$ballX == 255} {
        set ballXV [expr {0 - $ballXV}]
        PlaySound 2
    }

    if {$ballY == 1} {
        set ballYV [expr {0 - $ballYV}]
        PlaySound 2
    }

    if {$ballX == 1} {
        set ballX 128
        set ballY 128
        incr score
        PlaySound 1
    }

    if {$ballX > 5 && $ballX < 13 && $ballY > $paddleY && $ballY < [expr {$paddleY + 64}]} {
        set ballXV [expr {0 - $ballXV}]
        PlaySound 0
    }

    set ballX [expr {$ballX + $ballXV}]
    set ballY [expr {$ballY + $ballYV}]

}

# Run during Draw (put your drawing code here)
proc draw {} {
    global Black
    global White
    global paddleY
    global ballX
    global ballY
    global score

    Clear $Black
    Rect 5 $paddleY 8 64 $White
    Rect $ballX $ballY 4 4 $White
    Text 128 64 $score $White
}

# Run when the game exits (do cleanup, save, etc.)
proc end {} {

}