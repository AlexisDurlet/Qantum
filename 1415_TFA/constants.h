#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include "flib_vec2.h"

//XML
const int MAX_XML_DATA_SIZE = 63;

//Physics consts
const float GRAVITY = 100 * 32;
const float GROUND_INTERFACE_THICKNESS = 0.5;
const float GROUND_INTERFACE_LENGHT_TOLERANCE = 9;
const int BOX_SIDE_PROBING_STEP_LENGTH = 5;

//Texture consts
const int TEXTURE_NUMBER = 2;
const int TILE_SIZE = 64;
const float SCALE = 0.5f;

//Input consts
const int PRESSED_TIME_LIMIT = 100;
const int DOUBLE_TAP_TIMING = 200;
const float SWIPE_MAX_AXIS_FACTOR = 1;
const float SWIPE_MIN_DISTANCE = 20;

const TGfxVec2 BUTTON_EXIT_CUTOUT = TGfxVec2(12 * 64, 2 * 64);

//Animation consts
const int TIME_TOLERANCE = 10;

//Player consts
const TGfxVec2 PLAYER_RADIUS = TGfxVec2(16, 32);
const TGfxVec2 PLAYER_RADIUS_BLINK = TGfxVec2(3, 3);
const TGfxVec2 PLAYER_WALK_SPEED = TGfxVec2(100, 0);
const TGfxVec2 PLAYER_INITIAL_CUTOUT = TGfxVec2(0, 0);
const TGfxVec2 PLAYER_INITIAL_CUTOUT_SIZE = TGfxVec2(64, 128);
const TGfxVec2 PLAYER_SCALE = TGfxVec2(0.5f, 0.5f);

const TGfxVec2 FALLING_CUTOUT = TGfxVec2(8 * 64, 0);
const TGfxVec2 FALLING_OPTIONAL_CUTOUT = TGfxVec2(8 * 64, 128);

const TGfxVec2 IDLE_ANIM_INITIAL_CUTOUT = TGfxVec2(0, 0);
const TGfxVec2 IDLE_ANIM_OPTIONAL_CUTOUT = TGfxVec2(0, 128);
const int IDLE_ANIM_TIME_PER_FRAME = 1000;
const int IDLE_ANIM_FRAME_NUMBER = 2;

const TGfxVec2 RUNNING_ANIM_INITIAL_CUTOUT = TGfxVec2(128, 0);
const TGfxVec2 RUNNING_ANIM_OPTIONAL_CUTOUT = TGfxVec2(128, 128);
const int RUNNING_ANIM_TIME_PER_FRAME = 100;
const int RUNNING_ANIM_FRAME_NUMBER = 6;

const TGfxVec2 PORTALTP_ANIM_INITIAL_CUTOUT = TGfxVec2(4*64, 2*128);
const int PORTALTP_ANIM_TIME_PER_FRAME = 100;
const int PORTALTP_ANIM_FRAME_NUMBER = 5;

const TGfxVec2 BLINKTP_ANIM_INITIAL_CUTOUT = TGfxVec2(0, 3 * 128);
const int BLINKTP_ANIM_TIME_PER_FRAME = 50;
const int BLINKTP_ANIM_FRAME_NUMBER = 6;

const TGfxVec2 DEATH_ANIM_INITIAL_CUTOUT = TGfxVec2(0, 4 * 128);
const int DEATH_ANIM_TIME_PER_FRAME = 100;
const int DEATH_ANIM_FRAME_NUMBER = 7;

const TGfxVec2 PORTAL_INITIAL_CUTOUT = TGfxVec2(0, 2 * 128);
const TGfxVec2 PORTAL_INITIAL_CUTOUT_SIZE = PLAYER_INITIAL_CUTOUT_SIZE;
const TGfxVec2 PORTAL_SCALE = PLAYER_SCALE;
const int PORTAL_ANIM_TIME_PER_FRAME = 200;
const int PORTAL_ANIM_FRAME_NUMBER = 4;

const float BLINK_STEP_LENGHT = 5;
const float BLINK_SPEED = 1000;
const float BLINK_SPEED_SUBDIVISION = 50;

const float CAMERA_DEATH_TRAVELING_SPEED = 300;


//platforms consts
const TGfxVec2 PLATFORM_CUTOUT = TGfxVec2(0, 15 * 64);

//traps consts
const TGfxVec2 TRAP_BASE_RADIUS = TGfxVec2(16, 16);
const TGfxVec2 TRAP_BASE_CUTOUT = TGfxVec2(10 * 64, 0 * 64);
const TGfxVec2 TRAP_BASE_CUTOUT_SIZE = TGfxVec2(64, 64);

const float TRAP_LASER_WIDTH = 11;
const TGfxVec2 TRAP_LASER_CUTOUT = TGfxVec2(15 * 64, 0 * 64);
const float TRAP_LASER_CUTOUT_SIZE_X = 64;

const TGfxVec2 TRAP_LASER_END_INITIAL_CUTOUT = TGfxVec2(10 * 64, 2 * 64);
const TGfxVec2 TRAP_LASER_END_CUTOUT_SIZE = TGfxVec2(64, 64);
const int TRAP_LASER_END_TIME_PER_FRAME = 100;
const int TRAP_LASER_END_FRAME_NUMBER = 2;

//check points consts
const TGfxVec2 CHECKPOINT_INITIAL_CUTOUT = TGfxVec2(7 * 64, 6 * 64);
const TGfxVec2 CHECKPOINT_ACTIVATED_CUTOUT = TGfxVec2(9 * 64, 6 * 64);
const TGfxVec2 CHECKPOINT_CUTOUT_SIZE = TGfxVec2(128, 128);
const TGfxVec2 CHECKPOINT_RADIUS = TGfxVec2(32, 32);
const TGfxVec2 CHECKPOINT_RESPAWN_CUTOUT = TGfxVec2(7 * 64, 8 * 64);
const int CHECKPOINT_RESPAWN_TIME_PER_FRAME = 50;
const int CHECKPOINT_RESPAWN_FRAME_NUMBER = 2;


#endif
