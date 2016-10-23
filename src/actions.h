//#define NO_ACTION b00000000
//#define HOLD bxxxx0000
//#define TOUCH_ALL b1111xxxx
//#define TOUCH_LEFT b1111xxxx
//#define TOUCH_RIGHT
//#define UP_ALL
//#define UP_LEFT
//#define UP_RIGHT

/*

Top
|Bottom
||Left
|||Right
|||| - if both, then both
0000 0000
     |||| - otherwise none of the above
     |||Direction Right
     ||Direction Left
     |Direction Down
     Direction Up
 */