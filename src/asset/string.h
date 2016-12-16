#pragma once
#include "types.h"

namespace VI
{

namespace Asset
{
	namespace String
	{
		const s32 count = 407;
		const AssetID _default = 0;
		const AssetID ability_spawn_cost = 1;
		const AssetID attack = 2;
		const AssetID back = 3;
		const AssetID battery_captured = 4;
		const AssetID binding = 5;
		const AssetID bolter = 6;
		const AssetID btn_A = 7;
		const AssetID btn_B = 8;
		const AssetID btn_Back = 9;
		const AssetID btn_LeftClick = 10;
		const AssetID btn_LeftShoulder = 11;
		const AssetID btn_LeftTrigger = 12;
		const AssetID btn_None = 13;
		const AssetID btn_RightClick = 14;
		const AssetID btn_RightShoulder = 15;
		const AssetID btn_RightTrigger = 16;
		const AssetID btn_Start = 17;
		const AssetID btn_X = 18;
		const AssetID btn_Y = 19;
		const AssetID buy_period = 20;
		const AssetID buying = 21;
		const AssetID canceling_capture = 22;
		const AssetID capture_bonus = 23;
		const AssetID close = 24;
		const AssetID confirm_quit = 25;
		const AssetID confirm_spend = 26;
		const AssetID connecting = 27;
		const AssetID contact_ivory = 28;
		const AssetID contact_meursault = 29;
		const AssetID contact_seven = 30;
		const AssetID containment_field = 31;
		const AssetID containment_field_destroyed = 32;
		const AssetID control_point_captured = 33;
		const AssetID control_point_lost = 34;
		const AssetID conversation_done = 35;
		const AssetID danger = 36;
		const AssetID day = 37;
		const AssetID deaths = 38;
		const AssetID decoy = 39;
		const AssetID defeat = 40;
		const AssetID defend = 41;
		const AssetID deploy_timer = 42;
		const AssetID deploying = 43;
		const AssetID description_bolter = 44;
		const AssetID description_containment_field = 45;
		const AssetID description_decoy = 46;
		const AssetID description_grenade = 47;
		const AssetID description_minion = 48;
		const AssetID description_rocket = 49;
		const AssetID description_sensor = 50;
		const AssetID description_sniper = 51;
		const AssetID draw = 52;
		const AssetID drones = 53;
		const AssetID drones_remaining = 54;
		const AssetID enemy_tracking = 55;
		const AssetID energy = 56;
		const AssetID energy_generation = 57;
		const AssetID energy_generation_group = 58;
		const AssetID energy_generation_total = 59;
		const AssetID error_hostile_zone = 60;
		const AssetID error_inside_tram = 61;
		const AssetID error_locked_zone = 62;
		const AssetID error_zone_under_attack = 63;
		const AssetID exit = 64;
		const AssetID forfeit_network_error = 65;
		const AssetID forfeit_opponent_quit = 66;
		const AssetID futifs = 67;
		const AssetID grenade = 68;
		const AssetID group_required = 69;
		const AssetID hack_complete = 70;
		const AssetID hack_kits = 71;
		const AssetID hacking = 72;
		const AssetID hour = 73;
		const AssetID in_group_queue = 74;
		const AssetID incoming_message = 75;
		const AssetID insufficient_resource = 76;
		const AssetID intro = 77;
		const AssetID invert_y = 78;
		const AssetID key_A = 79;
		const AssetID key_ACBack = 80;
		const AssetID key_ACBookmarks = 81;
		const AssetID key_ACForward = 82;
		const AssetID key_ACHome = 83;
		const AssetID key_ACRefresh = 84;
		const AssetID key_ACSearch = 85;
		const AssetID key_ACStop = 86;
		const AssetID key_Again = 87;
		const AssetID key_AltErase = 88;
		const AssetID key_Apostrophe = 89;
		const AssetID key_Application = 90;
		const AssetID key_AudioMute = 91;
		const AssetID key_AudioNext = 92;
		const AssetID key_AudioPlay = 93;
		const AssetID key_AudioPrev = 94;
		const AssetID key_AudioStop = 95;
		const AssetID key_B = 96;
		const AssetID key_Backslash = 97;
		const AssetID key_Backspace = 98;
		const AssetID key_BrightnessDown = 99;
		const AssetID key_BrightnessUp = 100;
		const AssetID key_C = 101;
		const AssetID key_Calculator = 102;
		const AssetID key_Cancel = 103;
		const AssetID key_Capslock = 104;
		const AssetID key_Clear = 105;
		const AssetID key_ClearAgain = 106;
		const AssetID key_Comma = 107;
		const AssetID key_Computer = 108;
		const AssetID key_Copy = 109;
		const AssetID key_CrSel = 110;
		const AssetID key_CurrencySubunit = 111;
		const AssetID key_CurrencyUnit = 112;
		const AssetID key_Cut = 113;
		const AssetID key_D = 114;
		const AssetID key_D0 = 115;
		const AssetID key_D1 = 116;
		const AssetID key_D2 = 117;
		const AssetID key_D3 = 118;
		const AssetID key_D4 = 119;
		const AssetID key_D5 = 120;
		const AssetID key_D6 = 121;
		const AssetID key_D7 = 122;
		const AssetID key_D8 = 123;
		const AssetID key_D9 = 124;
		const AssetID key_DecimalSeparator = 125;
		const AssetID key_Delete = 126;
		const AssetID key_DisplaySwitch = 127;
		const AssetID key_Down = 128;
		const AssetID key_E = 129;
		const AssetID key_Eject = 130;
		const AssetID key_End = 131;
		const AssetID key_Equals = 132;
		const AssetID key_Escape = 133;
		const AssetID key_ExSel = 134;
		const AssetID key_Execute = 135;
		const AssetID key_F = 136;
		const AssetID key_F1 = 137;
		const AssetID key_F10 = 138;
		const AssetID key_F11 = 139;
		const AssetID key_F12 = 140;
		const AssetID key_F13 = 141;
		const AssetID key_F14 = 142;
		const AssetID key_F15 = 143;
		const AssetID key_F16 = 144;
		const AssetID key_F17 = 145;
		const AssetID key_F18 = 146;
		const AssetID key_F19 = 147;
		const AssetID key_F2 = 148;
		const AssetID key_F20 = 149;
		const AssetID key_F21 = 150;
		const AssetID key_F22 = 151;
		const AssetID key_F23 = 152;
		const AssetID key_F24 = 153;
		const AssetID key_F3 = 154;
		const AssetID key_F4 = 155;
		const AssetID key_F5 = 156;
		const AssetID key_F6 = 157;
		const AssetID key_F7 = 158;
		const AssetID key_F8 = 159;
		const AssetID key_F9 = 160;
		const AssetID key_Find = 161;
		const AssetID key_G = 162;
		const AssetID key_Grave = 163;
		const AssetID key_H = 164;
		const AssetID key_Help = 165;
		const AssetID key_Home = 166;
		const AssetID key_I = 167;
		const AssetID key_Insert = 168;
		const AssetID key_J = 169;
		const AssetID key_K = 170;
		const AssetID key_KbDillumDown = 171;
		const AssetID key_KbDillumToggle = 172;
		const AssetID key_KbDillumUp = 173;
		const AssetID key_Keypad0 = 174;
		const AssetID key_Keypad00 = 175;
		const AssetID key_Keypad000 = 176;
		const AssetID key_Keypad1 = 177;
		const AssetID key_Keypad2 = 178;
		const AssetID key_Keypad3 = 179;
		const AssetID key_Keypad4 = 180;
		const AssetID key_Keypad5 = 181;
		const AssetID key_Keypad6 = 182;
		const AssetID key_Keypad7 = 183;
		const AssetID key_Keypad8 = 184;
		const AssetID key_Keypad9 = 185;
		const AssetID key_KeypadA = 186;
		const AssetID key_KeypadAmpersand = 187;
		const AssetID key_KeypadAt = 188;
		const AssetID key_KeypadB = 189;
		const AssetID key_KeypadBackspace = 190;
		const AssetID key_KeypadBinary = 191;
		const AssetID key_KeypadC = 192;
		const AssetID key_KeypadClear = 193;
		const AssetID key_KeypadClearEntry = 194;
		const AssetID key_KeypadColon = 195;
		const AssetID key_KeypadComma = 196;
		const AssetID key_KeypadD = 197;
		const AssetID key_KeypadDblAmpersand = 198;
		const AssetID key_KeypadDblVerticalBar = 199;
		const AssetID key_KeypadDecimal = 200;
		const AssetID key_KeypadDivide = 201;
		const AssetID key_KeypadE = 202;
		const AssetID key_KeypadEnter = 203;
		const AssetID key_KeypadEquals = 204;
		const AssetID key_KeypadEqualsAS400 = 205;
		const AssetID key_KeypadExclam = 206;
		const AssetID key_KeypadF = 207;
		const AssetID key_KeypadGreater = 208;
		const AssetID key_KeypadHash = 209;
		const AssetID key_KeypadHexadecimal = 210;
		const AssetID key_KeypadLeftBrace = 211;
		const AssetID key_KeypadLeftParen = 212;
		const AssetID key_KeypadLess = 213;
		const AssetID key_KeypadMemAdd = 214;
		const AssetID key_KeypadMemClear = 215;
		const AssetID key_KeypadMemDivide = 216;
		const AssetID key_KeypadMemMultiply = 217;
		const AssetID key_KeypadMemRecall = 218;
		const AssetID key_KeypadMemStore = 219;
		const AssetID key_KeypadMemSubtract = 220;
		const AssetID key_KeypadMinus = 221;
		const AssetID key_KeypadMultiply = 222;
		const AssetID key_KeypadOctal = 223;
		const AssetID key_KeypadPercent = 224;
		const AssetID key_KeypadPeriod = 225;
		const AssetID key_KeypadPlus = 226;
		const AssetID key_KeypadPlusMinus = 227;
		const AssetID key_KeypadPower = 228;
		const AssetID key_KeypadRightBrace = 229;
		const AssetID key_KeypadRightParen = 230;
		const AssetID key_KeypadSpace = 231;
		const AssetID key_KeypadTab = 232;
		const AssetID key_KeypadVerticalBar = 233;
		const AssetID key_KeypadXor = 234;
		const AssetID key_L = 235;
		const AssetID key_LAlt = 236;
		const AssetID key_LCtrl = 237;
		const AssetID key_LGui = 238;
		const AssetID key_LShift = 239;
		const AssetID key_Left = 240;
		const AssetID key_LeftBracket = 241;
		const AssetID key_M = 242;
		const AssetID key_Mail = 243;
		const AssetID key_MediaSelect = 244;
		const AssetID key_Menu = 245;
		const AssetID key_Minus = 246;
		const AssetID key_Mode = 247;
		const AssetID key_MouseLeft = 248;
		const AssetID key_MouseMiddle = 249;
		const AssetID key_MouseRight = 250;
		const AssetID key_Mute = 251;
		const AssetID key_N = 252;
		const AssetID key_None = 253;
		const AssetID key_NumlockClear = 254;
		const AssetID key_O = 255;
		const AssetID key_Oper = 256;
		const AssetID key_Out = 257;
		const AssetID key_P = 258;
		const AssetID key_PageDown = 259;
		const AssetID key_PageUp = 260;
		const AssetID key_Paste = 261;
		const AssetID key_Pause = 262;
		const AssetID key_Period = 263;
		const AssetID key_Power = 264;
		const AssetID key_Printscreen = 265;
		const AssetID key_Prior = 266;
		const AssetID key_Q = 267;
		const AssetID key_R = 268;
		const AssetID key_RAlt = 269;
		const AssetID key_RCtrl = 270;
		const AssetID key_RGui = 271;
		const AssetID key_RShift = 272;
		const AssetID key_Return = 273;
		const AssetID key_Return2 = 274;
		const AssetID key_Right = 275;
		const AssetID key_RightBracket = 276;
		const AssetID key_S = 277;
		const AssetID key_Scrolllock = 278;
		const AssetID key_Select = 279;
		const AssetID key_Semicolon = 280;
		const AssetID key_Separator = 281;
		const AssetID key_Slash = 282;
		const AssetID key_Sleep = 283;
		const AssetID key_Space = 284;
		const AssetID key_Stop = 285;
		const AssetID key_SysReq = 286;
		const AssetID key_T = 287;
		const AssetID key_Tab = 288;
		const AssetID key_ThousandsSeparator = 289;
		const AssetID key_U = 290;
		const AssetID key_Undo = 291;
		const AssetID key_Up = 292;
		const AssetID key_V = 293;
		const AssetID key_VolumeDown = 294;
		const AssetID key_VolumeUp = 295;
		const AssetID key_W = 296;
		const AssetID key_Www = 297;
		const AssetID key_X = 298;
		const AssetID key_Y = 299;
		const AssetID key_Z = 300;
		const AssetID keyboard_movement = 301;
		const AssetID kills = 302;
		const AssetID left_joystick = 303;
		const AssetID leftover_energy = 304;
		const AssetID loading = 305;
		const AssetID losing = 306;
		const AssetID main_menu = 307;
		const AssetID match_searching = 308;
		const AssetID match_starting = 309;
		const AssetID member_of_group = 310;
		const AssetID minion = 311;
		const AssetID minion_killed = 312;
		const AssetID minute = 313;
		const AssetID msg_ivory_intro = 314;
		const AssetID msg_meursault_intro = 315;
		const AssetID music = 316;
		const AssetID no = 317;
		const AssetID no_effect = 318;
		const AssetID none = 319;
		const AssetID now = 320;
		const AssetID online = 321;
		const AssetID options = 322;
		const AssetID overshield = 323;
		const AssetID play = 324;
		const AssetID player = 325;
		const AssetID player_ascended = 326;
		const AssetID player_joined = 327;
		const AssetID player_killed = 328;
		const AssetID player_left = 329;
		const AssetID prompt_accept = 330;
		const AssetID prompt_back = 331;
		const AssetID prompt_buy = 332;
		const AssetID prompt_buy_more = 333;
		const AssetID prompt_cancel = 334;
		const AssetID prompt_cancel_ability = 335;
		const AssetID prompt_cancel_hack = 336;
		const AssetID prompt_capture = 337;
		const AssetID prompt_defend = 338;
		const AssetID prompt_deploy = 339;
		const AssetID prompt_hack = 340;
		const AssetID prompt_interact = 341;
		const AssetID prompt_splitscreen = 342;
		const AssetID prompt_splitscreen_ready = 343;
		const AssetID prompt_sudoku_place = 344;
		const AssetID prompt_upgrade = 345;
		const AssetID prompt_zone_defend = 346;
		const AssetID quit = 347;
		const AssetID resource_collected = 348;
		const AssetID right_joystick = 349;
		const AssetID rocket = 350;
		const AssetID rocket_incoming = 351;
		const AssetID sensitivity = 352;
		const AssetID sensor = 353;
		const AssetID sensor_destroyed = 354;
		const AssetID sfx = 355;
		const AssetID shield = 356;
		const AssetID shield_down = 357;
		const AssetID sniper = 358;
		const AssetID spectating = 359;
		const AssetID splitscreen = 360;
		const AssetID starting_capture = 361;
		const AssetID stealth = 362;
		const AssetID tab_inventory = 363;
		const AssetID tab_map = 364;
		const AssetID tab_messages = 365;
		const AssetID target_shield_down = 366;
		const AssetID team_a = 367;
		const AssetID team_b = 368;
		const AssetID team_c = 369;
		const AssetID team_d = 370;
		const AssetID terminal_locked = 371;
		const AssetID timer = 372;
		const AssetID tip = 373;
		const AssetID tip_0 = 374;
		const AssetID tip_1 = 375;
		const AssetID tip_10 = 376;
		const AssetID tip_11 = 377;
		const AssetID tip_2 = 378;
		const AssetID tip_3 = 379;
		const AssetID tip_4 = 380;
		const AssetID tip_5 = 381;
		const AssetID tip_7 = 382;
		const AssetID tip_8 = 383;
		const AssetID tip_9 = 384;
		const AssetID tut_ability = 385;
		const AssetID tut_capture = 386;
		const AssetID tut_climb_jump = 387;
		const AssetID tut_slide = 388;
		const AssetID tut_start = 389;
		const AssetID tut_upgrade = 390;
		const AssetID tut_wallrun = 391;
		const AssetID unknown = 392;
		const AssetID upgrade_description = 393;
		const AssetID upgrade_notification = 394;
		const AssetID upgrading = 395;
		const AssetID victory = 396;
		const AssetID waiting = 397;
		const AssetID yes = 398;
		const AssetID zodiak = 399;
		const AssetID zone_already_captured = 400;
		const AssetID zone_defense_expired = 401;
		const AssetID zone_unlocked = 402;
		const AssetID zones_captured = 403;
		const AssetID zones_hostile = 404;
		const AssetID zones_locked = 405;
		const AssetID zones_under_attack = 406;
	}
}

}