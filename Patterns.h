// this file is for importing pattern definitions into arduino code
// Defined patterns - incrementing numbers
// patterns will loop indefinitely

#define START_OF_MESSAGE 's'
#define END_OF_MESSAGE 'e'
// solid color
#define PATTERN_SOLIDCOLOR '0'
// blink on/off two colors
#define PATTERN_BLINK '1'
// pulse on/off two colors
#define PATTERN_PULSE '2'
// scroll two colors hard cutoff w/ modulus
#define PATTERN_SCROLL '3'
// scroll two colors w/ 2 sin waves
#define PATTERN_SCROLLSMOOTH '4'
// wipe left
#define PATTERN_WIPELEFT '5'
// wipe right
#define PATTERN_WIPERIGHT '6'
// larson scanner
#define PATTERN_LARSON '7'
// random blinking
#define PATTERN_RANDBLINK '8'
// decay of lights (used for actions mostly)
#define PATTERN_DECAY '9'

// define actions - incrementing uppercase letters
// actions will happen as they are called, they are 'drawn' over patterns

// todo have two delay values, one for animation speed and hold time

// set all lights to a color for a specified amount of time
//#define ACTION_SETCOLOR 'A'
//// fade all lights to a color for a specified amount of time
//#define ACTION_FADECOLOR 'B'
//// flash lights
//#define ACTION_FLASH 'C'
//// wipe left
//#define ACTION_WIPELEFT 'D'
//// wipe right
//#define ACTION_WIPERIGHT 'E'
//// center wipe
//#define ACTION_WIPECENTER 'F'

// presets
// custom patterns that follow the pattern
#define PRESET_REDWHITEANDBLUE 'a'

