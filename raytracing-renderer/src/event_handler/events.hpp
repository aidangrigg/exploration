namespace events {

// based on SDL Keycodes: https://wiki.libsdl.org/SDL3/SDL_Scancode
enum KeyCode {
  KC_UNKNOWN = 0,

  KC_A = 4,
  KC_B = 5,
  KC_C = 6,
  KC_D = 7,
  KC_E = 8,
  KC_F = 9,
  KC_G = 10,
  KC_H = 11,
  KC_I = 12,
  KC_J = 13,
  KC_K = 14,
  KC_L = 15,
  KC_M = 16,
  KC_N = 17,
  KC_O = 18,
  KC_P = 19,
  KC_Q = 20,
  KC_R = 21,
  KC_S = 22,
  KC_T = 23,
  KC_U = 24,
  KC_V = 25,
  KC_W = 26,
  KC_X = 27,
  KC_Y = 28,
  KC_Z = 29,

  KC_1 = 30,
  KC_2 = 31,
  KC_3 = 32,
  KC_4 = 33,
  KC_5 = 34,
  KC_6 = 35,
  KC_7 = 36,
  KC_8 = 37,
  KC_9 = 38,
  KC_0 = 39,

  KC_RETURN = 40,
  KC_ESCAPE = 41,
  KC_BACKSPACE = 42,
  KC_TAB = 43,
  KC_SPACE = 44,

  KC_MINUS = 45,
  KC_EQUALS = 46,
  KC_LEFTBRACKET = 47,
  KC_RIGHTBRACKET = 48,
  KC_BACKSLASH = 49, /**< Located at the lower left of the return
                      *   key on ISO keyboards and at the right end
                      *   of the QWERTY row on ANSI keyboards.
                      *   Produces REVERSE SOLIDUS (backslash) and
                      *   VERTICAL LINE in a US layout, REVERSE
                      *   SOLIDUS and VERTICAL LINE in a UK Mac
                      *   layout, NUMBER SIGN and TILDE in a UK
                      *   Windows layout, DOLLAR SIGN and POUND SIGN
                      *   in a Swiss German layout, NUMBER SIGN and
                      *   APOSTROPHE in a German layout, GRAVE
                      *   ACCENT and POUND SIGN in a French Mac
                      *   layout, and ASTERISK and MICRO SIGN in a
                      *   French Windows layout.
                      */
  KC_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                      *   instead of 49 for the same key, but all
                      *   OSes I've seen treat the two codes
                      *   identically. So, as an implementer, unless
                      *   your keyboard generates both of those
                      *   codes and your OS treats them differently,
                      *   you should generate KC_BACKSLASH
                      *   instead of this code. As a user, you
                      *   should not rely on this code because SDL
                      *   will never generate it with most (all?)
                      *   keyboards.
                      */
  KC_SEMICOLON = 51,
  KC_APOSTROPHE = 52,
  KC_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                  *   and ISO keyboards). Produces GRAVE ACCENT and
                  *   TILDE in a US Windows layout and in US and UK
                  *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                  *   and NOT SIGN in a UK Windows layout, SECTION
                  *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                  *   layouts on ISO keyboards, SECTION SIGN and
                  *   DEGREE SIGN in a Swiss German layout (Mac:
                  *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                  *   DEGREE SIGN in a German layout (Mac: only on
                  *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                  *   French Windows layout, COMMERCIAL AT and
                  *   NUMBER SIGN in a French Mac layout on ISO
                  *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                  *   SIGN in a Swiss German, German, or French Mac
                  *   layout on ANSI keyboards.
                  */
  KC_COMMA = 54,
  KC_PERIOD = 55,
  KC_SLASH = 56,

  KC_CAPSLOCK = 57,

  KC_F1 = 58,
  KC_F2 = 59,
  KC_F3 = 60,
  KC_F4 = 61,
  KC_F5 = 62,
  KC_F6 = 63,
  KC_F7 = 64,
  KC_F8 = 65,
  KC_F9 = 66,
  KC_F10 = 67,
  KC_F11 = 68,
  KC_F12 = 69,

  KC_PRINTSCREEN = 70,
  KC_SCROLLLOCK = 71,
  KC_PAUSE = 72,
  KC_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                 does send code 73, not 117) */
  KC_HOME = 74,
  KC_PAGEUP = 75,
  KC_DELETE = 76,
  KC_END = 77,
  KC_PAGEDOWN = 78,
  KC_RIGHT = 79,
  KC_LEFT = 80,
  KC_DOWN = 81,
  KC_UP = 82,

  KC_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                         */
  KC_KP_DIVIDE = 84,
  KC_KP_MULTIPLY = 85,
  KC_KP_MINUS = 86,
  KC_KP_PLUS = 87,
  KC_KP_ENTER = 88,
  KC_KP_1 = 89,
  KC_KP_2 = 90,
  KC_KP_3 = 91,
  KC_KP_4 = 92,
  KC_KP_5 = 93,
  KC_KP_6 = 94,
  KC_KP_7 = 95,
  KC_KP_8 = 96,
  KC_KP_9 = 97,
  KC_KP_0 = 98,
  KC_KP_PERIOD = 99,

  KC_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                            *   keyboards have over ANSI ones,
                            *   located between left shift and Z.
                            *   Produces GRAVE ACCENT and TILDE in a
                            *   US or UK Mac layout, REVERSE SOLIDUS
                            *   (backslash) and VERTICAL LINE in a
                            *   US or UK Windows layout, and
                            *   LESS-THAN SIGN and GREATER-THAN SIGN
                            *   in a Swiss German, German, or French
                            *   layout. */
  KC_APPLICATION = 101,    /**< windows contextual menu, compose */
  KC_POWER = 102,          /**< The USB document says this is a status flag,
                            *   not a physical key - but some Mac keyboards
                            *   do have a power key. */
  KC_KP_EQUALS = 103,
  KC_F13 = 104,
  KC_F14 = 105,
  KC_F15 = 106,
  KC_F16 = 107,
  KC_F17 = 108,
  KC_F18 = 109,
  KC_F19 = 110,
  KC_F20 = 111,
  KC_F21 = 112,
  KC_F22 = 113,
  KC_F23 = 114,
  KC_F24 = 115,
  KC_EXECUTE = 116,
  KC_HELP = 117, /**< AL Integrated Help Center */
  KC_MENU = 118, /**< Menu (show menu) */
  KC_SELECT = 119,
  KC_STOP = 120,  /**< AC Stop */
  KC_AGAIN = 121, /**< AC Redo/Repeat */
  KC_UNDO = 122,  /**< AC Undo */
  KC_CUT = 123,   /**< AC Cut */
  KC_COPY = 124,  /**< AC Copy */
  KC_PASTE = 125, /**< AC Paste */
  KC_FIND = 126,  /**< AC Find */
  KC_MUTE = 127,
  KC_VOLUMEUP = 128,
  KC_VOLUMEDOWN = 129,
  /* not sure whether there's a reason to enable these */
  /*     KC_LOCKINGCAPSLOCK = 130,  */
  /*     KC_LOCKINGNUMLOCK = 131, */
  /*     KC_LOCKINGSCROLLLOCK = 132, */
  KC_KP_COMMA = 133,
  KC_KP_EQUALSAS400 = 134,

  KC_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                          footnotes in USB doc */
  KC_INTERNATIONAL2 = 136,
  KC_INTERNATIONAL3 = 137, /**< Yen */
  KC_INTERNATIONAL4 = 138,
  KC_INTERNATIONAL5 = 139,
  KC_INTERNATIONAL6 = 140,
  KC_INTERNATIONAL7 = 141,
  KC_INTERNATIONAL8 = 142,
  KC_INTERNATIONAL9 = 143,
  KC_LANG1 = 144, /**< Hangul/English toggle */
  KC_LANG2 = 145, /**< Hanja conversion */
  KC_LANG3 = 146, /**< Katakana */
  KC_LANG4 = 147, /**< Hiragana */
  KC_LANG5 = 148, /**< Zenkaku/Hankaku */
  KC_LANG6 = 149, /**< reserved */
  KC_LANG7 = 150, /**< reserved */
  KC_LANG8 = 151, /**< reserved */
  KC_LANG9 = 152, /**< reserved */

  KC_ALTERASE = 153, /**< Erase-Eaze */
  KC_SYSREQ = 154,
  KC_CANCEL = 155, /**< AC Cancel */
  KC_CLEAR = 156,
  KC_PRIOR = 157,
  KC_RETURN2 = 158,
  KC_SEPARATOR = 159,
  KC_OUT = 160,
  KC_OPER = 161,
  KC_CLEARAGAIN = 162,
  KC_CRSEL = 163,
  KC_EXSEL = 164,

  KC_KP_00 = 176,
  KC_KP_000 = 177,
  KC_THOUSANDSSEPARATOR = 178,
  KC_DECIMALSEPARATOR = 179,
  KC_CURRENCYUNIT = 180,
  KC_CURRENCYSUBUNIT = 181,
  KC_KP_LEFTPAREN = 182,
  KC_KP_RIGHTPAREN = 183,
  KC_KP_LEFTBRACE = 184,
  KC_KP_RIGHTBRACE = 185,
  KC_KP_TAB = 186,
  KC_KP_BACKSPACE = 187,
  KC_KP_A = 188,
  KC_KP_B = 189,
  KC_KP_C = 190,
  KC_KP_D = 191,
  KC_KP_E = 192,
  KC_KP_F = 193,
  KC_KP_XOR = 194,
  KC_KP_POWER = 195,
  KC_KP_PERCENT = 196,
  KC_KP_LESS = 197,
  KC_KP_GREATER = 198,
  KC_KP_AMPERSAND = 199,
  KC_KP_DBLAMPERSAND = 200,
  KC_KP_VERTICALBAR = 201,
  KC_KP_DBLVERTICALBAR = 202,
  KC_KP_COLON = 203,
  KC_KP_HASH = 204,
  KC_KP_SPACE = 205,
  KC_KP_AT = 206,
  KC_KP_EXCLAM = 207,
  KC_KP_MEMSTORE = 208,
  KC_KP_MEMRECALL = 209,
  KC_KP_MEMCLEAR = 210,
  KC_KP_MEMADD = 211,
  KC_KP_MEMSUBTRACT = 212,
  KC_KP_MEMMULTIPLY = 213,
  KC_KP_MEMDIVIDE = 214,
  KC_KP_PLUSMINUS = 215,
  KC_KP_CLEAR = 216,
  KC_KP_CLEARENTRY = 217,
  KC_KP_BINARY = 218,
  KC_KP_OCTAL = 219,
  KC_KP_DECIMAL = 220,
  KC_KP_HEXADECIMAL = 221,

  KC_LCTRL = 224,
  KC_LSHIFT = 225,
  KC_LALT = 226, /**< alt, option */
  KC_LGUI = 227, /**< windows, command (apple), meta */
  KC_RCTRL = 228,
  KC_RSHIFT = 229,
  KC_RALT = 230, /**< alt gr, option */
  KC_RGUI = 231, /**< windows, command (apple), meta */

  KC_MODE = 257, /**< I'm not sure if this is really not covered
                  *   by any of the above, but since there's a
                  *   special SDL_KMOD_MODE for it I'm adding it here
                  */

  /* @} */ /* Usage page 0x07 */

  /**
   *  \name Usage page 0x0C
   *
   *  These values are mapped from usage page 0x0C (USB consumer page).
   *
   *  There are way more keys in the spec than we can represent in the
   *  current scancode range, so pick the ones that commonly come up in
   *  real world usage.
   */
  /* @{ */

  KC_SLEEP = 258, /**< Sleep */
  KC_WAKE = 259,  /**< Wake */

  KC_CHANNEL_INCREMENT = 260, /**< Channel Increment */
  KC_CHANNEL_DECREMENT = 261, /**< Channel Decrement */

  KC_MEDIA_PLAY = 262,           /**< Play */
  KC_MEDIA_PAUSE = 263,          /**< Pause */
  KC_MEDIA_RECORD = 264,         /**< Record */
  KC_MEDIA_FAST_FORWARD = 265,   /**< Fast Forward */
  KC_MEDIA_REWIND = 266,         /**< Rewind */
  KC_MEDIA_NEXT_TRACK = 267,     /**< Next Track */
  KC_MEDIA_PREVIOUS_TRACK = 268, /**< Previous Track */
  KC_MEDIA_STOP = 269,           /**< Stop */
  KC_MEDIA_EJECT = 270,          /**< Eject */
  KC_MEDIA_PLAY_PAUSE = 271,     /**< Play / Pause */
  KC_MEDIA_SELECT = 272,         /* Media Select */

  KC_AC_NEW = 273,        /**< AC New */
  KC_AC_OPEN = 274,       /**< AC Open */
  KC_AC_CLOSE = 275,      /**< AC Close */
  KC_AC_EXIT = 276,       /**< AC Exit */
  KC_AC_SAVE = 277,       /**< AC Save */
  KC_AC_PRINT = 278,      /**< AC Print */
  KC_AC_PROPERTIES = 279, /**< AC Properties */

  KC_AC_SEARCH = 280,    /**< AC Search */
  KC_AC_HOME = 281,      /**< AC Home */
  KC_AC_BACK = 282,      /**< AC Back */
  KC_AC_FORWARD = 283,   /**< AC Forward */
  KC_AC_STOP = 284,      /**< AC Stop */
  KC_AC_REFRESH = 285,   /**< AC Refresh */
  KC_AC_BOOKMARKS = 286, /**< AC Bookmarks */

  /* @} */ /* Usage page 0x0C */

  /**
   *  \name Mobile keys
   *
   *  These are values that are often used on mobile phones.
   */
  /* @{ */

  KC_SOFTLEFT = 287,  /**< Usually situated below the display on phones and
                         used as a multi-function feature key for selecting
                         a software defined function shown on the bottom left
                         of the display. */
  KC_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                           used as a multi-function feature key for selecting
                           a software defined function shown on the bottom right
                           of the display. */
  KC_CALL = 289,      /**< Used for accepting phone calls. */
  KC_ENDCALL = 290,   /**< Used for rejecting phone calls. */

  /* @} */ /* Mobile keys */

  /* Add any other keys here. */

  KC_RESERVED = 400, /**< 400-500 reserved for dynamic keycodes */

  KC_COUNT = 512 /**< not a key, just marks the number of scancodes for array bounds */
};

enum MouseClick { LEFT, RIGHT, MIDDLE };

}; // namespace events
