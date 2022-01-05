set WindowTitle "Example - Collision"

set x 80
set y 80

proc begin {} {}
proc step {} {
    if {[KeyDown 265] == 1} {
        set ::y [expr {$::y - 2}]
    } elseif {[KeyDown 264] == 1} {
        set ::y [expr {$::y + 2}]
    }

    if {[KeyDown 263] == 1} {
        set ::x [expr {$::x - 2}]
    } elseif {[KeyDown 262] == 1} {
        set ::x [expr {$::x + 2}]
    }
}
proc draw {} {
    Clear $::Black
    Circle 20 20 10 $::White
    Text 60 60 "move the square onto the circle" $::White
    if {[PointIn $::x $::y 10 20 20]} {
        Rect $::x $::y 5 5 $::Green
    } else {
        Rect $::x $::y 5 5 $::White
    }
}