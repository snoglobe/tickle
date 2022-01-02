set WindowTitle "pnog"

set ballX 128
set ballY 128
set ballXV 1
set ballYV 1

set score 0

set paddleY 128

# Run when the game starts (put initalization here)
proc begin {} {

}

# Run before Draw (put your logic here)
proc step {} {
    global paddleY
    global ballX
    global ballY
    global ballXV
    global ballYV

    if {[KeyDown 265] == 1} {
        set paddleY [expr {$paddleY - 2}]
    } elseif {[KeyDown 264] == 1} {
        set paddleY [expr {$paddleY + 2}]
    }

    if {$ballY == 255} {
        set ballYV [expr {0 - $ballYV}]
    }

    if {$ballX == 255} {
        set ballXV [expr {0 - $ballXV}]
    }

    if {$ballY == 1} {
        set ballYV [expr {0 - $ballYV}]
    }

    if {$ballX == 1} {
        set ballX 128
        set ballY 128
        set score [expr {$score + 1}]
    }

    if {$ballX > 5 && $ballX < 13 && $ballY < $paddleY && $ballY > [expr {$paddleY - 64}]} {
        # hit

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

    Clear $Black
    Rect 5 $paddleY 8 64 $White
    Rect $ballX $ballY 4 4 $White
}

# Run when the game exits (do cleanup, save, etc.)
proc end {} {

}