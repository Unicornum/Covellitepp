
#pragma once

enum
{
  APP_CMD_DESTROY = 1709201109,
  APP_CMD_GAINED_FOCUS = 1709201110,
  APP_CMD_LOST_FOCUS = 1709201111,
  APP_CMD_INIT_WINDOW = 1709201112,
  APP_CMD_TERM_WINDOW = 1709201113,
  APP_CMD_WINDOW_REDRAW_NEEDED = 1709201114,
  APP_CMD_STOP = 1709201115,
  APP_CMD_PAUSE = 1709201116,
  APP_CMD_RESUME = 1709201117,

  AINPUT_EVENT_TYPE_MOTION = 1709201119,
  AINPUT_EVENT_TYPE_KEY = 1709201107,

  AKEY_EVENT_ACTION_UP = 1709201108,
  AKEY_EVENT_ACTION_DOWN = 1711041303,
};

enum
{
  AKEYCODE_UNKNOWN = 0,
  AKEYCODE_BACK = 1709201118,
  AKEYCODE_MENU = 1807311222,
  AKEYCODE_ENTER = 1710091200,
  AKEYCODE_DEL = 1710091201,
};

enum
{
  AMOTION_EVENT_ACTION_MASK = 0xff,
  AMOTION_EVENT_ACTION_POINTER_INDEX_MASK = 0xff00,
  AMOTION_EVENT_ACTION_DOWN = 0,
  AMOTION_EVENT_ACTION_UP = 1,
  AMOTION_EVENT_ACTION_MOVE = 2,
  AMOTION_EVENT_ACTION_CANCEL = 3,
  AMOTION_EVENT_ACTION_OUTSIDE = 4,
  AMOTION_EVENT_ACTION_POINTER_DOWN = 5,
  AMOTION_EVENT_ACTION_POINTER_UP = 6,
  AMOTION_EVENT_ACTION_HOVER_MOVE = 7,
  AMOTION_EVENT_ACTION_SCROLL = 8,
  AMOTION_EVENT_ACTION_HOVER_ENTER = 9,
  AMOTION_EVENT_ACTION_HOVER_EXIT = 10,
};
