#include "sys.h"

/**
 * @brief LED初始化
 * 
 */
int led_init(void);

/**
 * @brief LED控制
 * 
 * @param bright 亮度 1lsb = 1%
 * @param color 色温 1lsb = 1%  大于50%时偏冷，小于50%时偏暖
 */
int led_ctrl(short bright, short color);


























