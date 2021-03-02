#pragma once

namespace PRE
{
	namespace InputModule
	{
		class KeyCode
		{
		public:
			static const unsigned int UNKNOWN = 0;
			static const unsigned int A = 4;
			static const unsigned int B = 5;
			static const unsigned int C = 6;
			static const unsigned int D = 7;
			static const unsigned int E = 8;
			static const unsigned int F = 9;
			static const unsigned int G = 10;
			static const unsigned int H = 11;
			static const unsigned int I = 12;
			static const unsigned int J = 13;
			static const unsigned int K = 14;
			static const unsigned int L = 15;
			static const unsigned int M = 16;
			static const unsigned int N = 17;
			static const unsigned int O = 18;
			static const unsigned int P = 19;
			static const unsigned int Q = 20;
			static const unsigned int R = 21;
			static const unsigned int S = 22;
			static const unsigned int T = 23;
			static const unsigned int U = 24;
			static const unsigned int V = 25;
			static const unsigned int W = 26;
			static const unsigned int X = 27;
			static const unsigned int Y = 28;
			static const unsigned int Z = 29;
			static const unsigned int NUM_1 = 30;
			static const unsigned int NUM_2 = 31;
			static const unsigned int NUM_3 = 32;
			static const unsigned int NUM_4 = 33;
			static const unsigned int NUM_5 = 34;
			static const unsigned int NUM_6 = 35;
			static const unsigned int NUM_7 = 36;
			static const unsigned int NUM_8 = 37;
			static const unsigned int NUM_9 = 38;
			static const unsigned int NUM_0 = 39;
			static const unsigned int RETURN = 40;
			static const unsigned int ESCAPE = 41;
			static const unsigned int BACKSPACE = 42;
			static const unsigned int TAB = 43;
			static const unsigned int SPACE = 44;
			static const unsigned int MINUS = 45;
			static const unsigned int EQUALS = 46;
			static const unsigned int LEFTBRACKET = 47;
			static const unsigned int RIGHTBRACKET = 48;
			static const unsigned int BACKSLASH = 49;
			static const unsigned int NONUSHASH = 50;
			static const unsigned int SEMICOLON = 51;
			static const unsigned int APOSTROPHE = 52;
			static const unsigned int GRAVE = 53;
			static const unsigned int COMMA = 54;
			static const unsigned int PERIOD = 55;
			static const unsigned int SLASH = 56;
			static const unsigned int CAPSLOCK = 57;
			static const unsigned int F1 = 58;
			static const unsigned int F2 = 59;
			static const unsigned int F3 = 60;
			static const unsigned int F4 = 61;
			static const unsigned int F5 = 62;
			static const unsigned int F6 = 63;
			static const unsigned int F7 = 64;
			static const unsigned int F8 = 65;
			static const unsigned int F9 = 66;
			static const unsigned int F10 = 67;
			static const unsigned int F11 = 68;
			static const unsigned int F12 = 69;
			static const unsigned int PRINTSCREEN = 70;
			static const unsigned int SCROLLLOCK = 71;
			static const unsigned int PAUSE = 72;
			static const unsigned int INSERT = 73;
			static const unsigned int HOME = 74;
			static const unsigned int PAGEUP = 75;
			static const unsigned int DELETE = 76;
			static const unsigned int END = 77;
			static const unsigned int PAGEDOWN = 78;
			static const unsigned int RIGHT = 79;
			static const unsigned int LEFT = 80;
			static const unsigned int DOWN = 81;
			static const unsigned int UP = 82;
			static const unsigned int NUMLOCKCLEAR = 83;
			static const unsigned int KP_DIVIDE = 84;
			static const unsigned int KP_MULTIPLY = 85;
			static const unsigned int KP_MINUS = 86;
			static const unsigned int KP_PLUS = 87;
			static const unsigned int KP_ENTER = 88;
			static const unsigned int KP_1 = 89;
			static const unsigned int KP_2 = 90;
			static const unsigned int KP_3 = 91;
			static const unsigned int KP_4 = 92;
			static const unsigned int KP_5 = 93;
			static const unsigned int KP_6 = 94;
			static const unsigned int KP_7 = 95;
			static const unsigned int KP_8 = 96;
			static const unsigned int KP_9 = 97;
			static const unsigned int KP_0 = 98;
			static const unsigned int KP_PERIOD = 99;
			static const unsigned int NONUSBACKSLASH = 100;
			static const unsigned int APPLICATION = 101;
			static const unsigned int POWER = 102;
			static const unsigned int KP_EQUALS = 103;
			static const unsigned int F13 = 104;
			static const unsigned int F14 = 105;
			static const unsigned int F15 = 106;
			static const unsigned int F16 = 107;
			static const unsigned int F17 = 108;
			static const unsigned int F18 = 109;
			static const unsigned int F19 = 110;
			static const unsigned int F20 = 111;
			static const unsigned int F21 = 112;
			static const unsigned int F22 = 113;
			static const unsigned int F23 = 114;
			static const unsigned int F24 = 115;
			static const unsigned int EXECUTE = 116;
			static const unsigned int HELP = 117;
			static const unsigned int MENU = 118;
			static const unsigned int SELECT = 119;
			static const unsigned int STOP = 120;
			static const unsigned int AGAIN = 121;
			static const unsigned int UNDO = 122;
			static const unsigned int CUT = 123;
			static const unsigned int COPY = 124;
			static const unsigned int PASTE = 125;
			static const unsigned int FIND = 126;
			static const unsigned int MUTE = 127;
			static const unsigned int VOLUMEUP = 128;
			static const unsigned int VOLUMEDOWN = 129;
			static const unsigned int KP_COMMA = 133;
			static const unsigned int KP_EQUALSAS400 = 134;
			static const unsigned int INTERNATIONAL1 = 135;
			static const unsigned int INTERNATIONAL2 = 136;
			static const unsigned int INTERNATIONAL3 = 137;
			static const unsigned int INTERNATIONAL4 = 138;
			static const unsigned int INTERNATIONAL5 = 139;
			static const unsigned int INTERNATIONAL6 = 140;
			static const unsigned int INTERNATIONAL7 = 141;
			static const unsigned int INTERNATIONAL8 = 142;
			static const unsigned int INTERNATIONAL9 = 143;
			static const unsigned int LANG1 = 144;
			static const unsigned int LANG2 = 145;
			static const unsigned int LANG3 = 146;
			static const unsigned int LANG4 = 147;
			static const unsigned int LANG5 = 148;
			static const unsigned int LANG6 = 149;
			static const unsigned int LANG7 = 150;
			static const unsigned int LANG8 = 151;
			static const unsigned int LANG9 = 152;
			static const unsigned int ALTERASE = 153;
			static const unsigned int SYSREQ = 154;
			static const unsigned int CANCEL = 155;
			static const unsigned int CLEAR = 156;
			static const unsigned int PRIOR = 157;
			static const unsigned int RETURN2 = 158;
			static const unsigned int SEPARATOR = 159;
			static const unsigned int OUT = 160;
			static const unsigned int OPER = 161;
			static const unsigned int CLEARAGAIN = 162;
			static const unsigned int CRSEL = 163;
			static const unsigned int EXSEL = 164;
			static const unsigned int KP_00 = 176;
			static const unsigned int KP_000 = 177;
			static const unsigned int THOUSANDSSEPARATOR = 178;
			static const unsigned int DECIMALSEPARATOR = 179;
			static const unsigned int CURRENCYUNIT = 180;
			static const unsigned int CURRENCYSUBUNIT = 181;
			static const unsigned int KP_LEFTPAREN = 182;
			static const unsigned int KP_RIGHTPAREN = 183;
			static const unsigned int KP_LEFTBRACE = 184;
			static const unsigned int KP_RIGHTBRACE = 185;
			static const unsigned int KP_TAB = 186;
			static const unsigned int KP_BACKSPACE = 187;
			static const unsigned int KP_A = 188;
			static const unsigned int KP_B = 189;
			static const unsigned int KP_C = 190;
			static const unsigned int KP_D = 191;
			static const unsigned int KP_E = 192;
			static const unsigned int KP_F = 193;
			static const unsigned int KP_XOR = 194;
			static const unsigned int KP_POWER = 195;
			static const unsigned int KP_PERCENT = 196;
			static const unsigned int KP_LESS = 197;
			static const unsigned int KP_GREATER = 198;
			static const unsigned int KP_AMPERSAND = 199;
			static const unsigned int KP_DBLAMPERSAND = 200;
			static const unsigned int KP_VERTICALBAR = 201;
			static const unsigned int KP_DBLVERTICALBAR = 202;
			static const unsigned int KP_COLON = 203;
			static const unsigned int KP_HASH = 204;
			static const unsigned int KP_SPACE = 205;
			static const unsigned int KP_AT = 206;
			static const unsigned int KP_EXCLAM = 207;
			static const unsigned int KP_MEMSTORE = 208;
			static const unsigned int KP_MEMRECALL = 209;
			static const unsigned int KP_MEMCLEAR = 210;
			static const unsigned int KP_MEMADD = 211;
			static const unsigned int KP_MEMSUBTRACT = 212;
			static const unsigned int KP_MEMMULTIPLY = 213;
			static const unsigned int KP_MEMDIVIDE = 214;
			static const unsigned int KP_PLUSMINUS = 215;
			static const unsigned int KP_CLEAR = 216;
			static const unsigned int KP_CLEARENTRY = 217;
			static const unsigned int KP_BINARY = 218;
			static const unsigned int KP_OCTAL = 219;
			static const unsigned int KP_DECIMAL = 220;
			static const unsigned int KP_HEXADECIMAL = 221;
			static const unsigned int LCTRL = 224;
			static const unsigned int LSHIFT = 225;
			static const unsigned int LALT = 226;
			static const unsigned int LGUI = 227;
			static const unsigned int RCTRL = 228;
			static const unsigned int RSHIFT = 229;
			static const unsigned int RALT = 230;
			static const unsigned int RGUI = 231;
			static const unsigned int MODE = 257;
			static const unsigned int AUDIONEXT = 258;
			static const unsigned int AUDIOPREV = 259;
			static const unsigned int AUDIOSTOP = 260;
			static const unsigned int AUDIOPLAY = 261;
			static const unsigned int AUDIOMUTE = 262;
			static const unsigned int MEDIASELECT = 263;
			static const unsigned int WWW = 264;
			static const unsigned int MAIL = 265;
			static const unsigned int CALCULATOR = 266;
			static const unsigned int COMPUTER = 267;
			static const unsigned int AC_SEARCH = 268;
			static const unsigned int AC_HOME = 269;
			static const unsigned int AC_BACK = 270;
			static const unsigned int AC_FORWARD = 271;
			static const unsigned int AC_STOP = 272;
			static const unsigned int AC_REFRESH = 273;
			static const unsigned int AC_BOOKMARKS = 274;
			static const unsigned int BRIGHTNESSDOWN = 275;
			static const unsigned int BRIGHTNESSUP = 276;
			static const unsigned int DISPLAYSWITCH = 277;
			static const unsigned int KBDILLUMTOGGLE = 278;
			static const unsigned int KBDILLUMDOWN = 279;
			static const unsigned int KBDILLUMUP = 280;
			static const unsigned int EJECT = 281;
			static const unsigned int SLEEP = 282;
			static const unsigned int APP1 = 283;
			static const unsigned int APP2 = 284;
			static const unsigned int AUDIOREWIND = 285;
			static const unsigned int AUDIOFASTFORWARD = 286;

		private:
			KeyCode();
			~KeyCode();
		};
	} // namespace InputModule
} // namespace PRE