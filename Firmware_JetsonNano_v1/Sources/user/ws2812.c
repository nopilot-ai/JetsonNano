#include "stm32f10x.h"
#include "ws2812.h"


uint16_t led_ws2812[ARRAY_LEN] = {0};

void led_wheel(uint8_t led, uint8_t r ,uint8_t g, uint8_t b)
{
  if (led < LED_COUNT)
  {
    for (uint8_t bits = 0; bits < 8; bits++)
      led_ws2812[DELAY_LEN + 24*led + bits] = led_pwm[g][bits];
    for (uint8_t bits = 0; bits < 8; bits++)
      led_ws2812[DELAY_LEN + 24*led + bits + 8] = led_pwm[r][bits];
    for (uint8_t bits = 0; bits < 8; bits++)
      led_ws2812[DELAY_LEN + 24*led + bits + 16] = led_pwm[b][bits];
  }
}

uint8_t tempo_cnt = 0;
uint8_t tempo_cnt1 = 1;
uint8_t tempo_cnt2 = 2;
int8_t tempo_blink = 1;
int8_t tempo_cnt_blink = 1;
uint8_t tempo_cnt_temp = 4;
void led_loop(uint8_t type)
{
  switch (type)
  {
  case 0:   //none control source without internet
    if (tempo_cnt < (LED_COUNT - 1)) tempo_cnt++;
    else tempo_cnt = 0;
    if (tempo_cnt1 < (LED_COUNT - 1)) tempo_cnt1++;
    else tempo_cnt1 = 0;
    if (tempo_cnt2 < (LED_COUNT - 1)) tempo_cnt2++;
    else tempo_cnt2 = 0;
    for (uint8_t led_num = 0;led_num < LED_COUNT; led_num++)
    {
      if (tempo_cnt == led_num) led_wheel(led_num, 127, 127, 0);
      else if ((tempo_cnt1 == led_num) || (tempo_cnt2 == led_num)) led_wheel(led_num, 64, 64, 0); //yellow
      else led_wheel(led_num, 0, 0, 0);
    }
    break;
  case 1://wait source conrol with internet
    if (tempo_cnt < (LED_COUNT - 1)) tempo_cnt++;
    else tempo_cnt = 0;
    if (tempo_cnt1 < (LED_COUNT - 1)) tempo_cnt1++;
    else tempo_cnt1 = 0;
    if (tempo_cnt2 < (LED_COUNT - 1)) tempo_cnt2++;
    else tempo_cnt2 = 0;
    for (uint8_t led_num = 0;led_num < LED_COUNT; led_num++)
    {
      if (tempo_cnt == led_num) led_wheel(led_num, 127, 127, 127);
      else if ((tempo_cnt1 == led_num) || (tempo_cnt2 == led_num)) led_wheel(led_num, 64, 64, 64); //orAnge
      else led_wheel(led_num, 0, 0, 0);
    }
    break;
  case 2://web page control
    if (tempo_cnt < (LED_COUNT - 1)) tempo_cnt++;
    else tempo_cnt = 0;
    if (tempo_cnt1 < (LED_COUNT - 1)) tempo_cnt1++;
    else tempo_cnt1 = 0;
    if (tempo_cnt2 < (LED_COUNT - 1)) tempo_cnt2++;
    else tempo_cnt2 = 0;
    for (uint8_t led_num = 0;led_num < LED_COUNT; led_num++)
    {
      if (tempo_cnt == led_num) led_wheel(led_num, 127, 0, 0);
      else if ((tempo_cnt1 == led_num) || (tempo_cnt2 == led_num)) led_wheel(led_num, 64, 0, 0); 
      else led_wheel(led_num, 0, 0, 0);
    }
    break;
  case 3://joypad control
    if (tempo_cnt < (LED_COUNT - 1)) tempo_cnt++;
    else tempo_cnt = 0;
    if (tempo_cnt1 < (LED_COUNT - 1)) tempo_cnt1++;
    else tempo_cnt1 = 0;
    if (tempo_cnt2 < (LED_COUNT - 1)) tempo_cnt2++;
    else tempo_cnt2 = 0;
    for (uint8_t led_num = 0;led_num < LED_COUNT; led_num++)
    {
      if (tempo_cnt == led_num) led_wheel(led_num, 127, 0, 127);
      else if ((tempo_cnt1 == led_num) || (tempo_cnt2 == led_num)) led_wheel(led_num, 64, 0, 64); //fiolent
      else led_wheel(led_num, 0, 0, 0);
    }
    break;
  case 4://1-4 led blink red
    if (tempo_cnt_blink == 120)
      tempo_blink = -4;
    if (tempo_cnt_blink == 0)
      tempo_blink = 4;
    
    tempo_cnt_blink += tempo_blink;
    led_wheel(0, tempo_cnt_blink, 0, 0);
    led_wheel(1, tempo_cnt_blink, 0, 0);
    led_wheel(2, tempo_cnt_blink, 0, 0);
    led_wheel(3, tempo_cnt_blink, 0, 0);
    led_wheel(4, 0, 0, 0);led_wheel(5, 0, 0, 0); led_wheel(6, 0, 0, 0);led_wheel(7, 0, 0, 0);
    led_wheel(8, 0, 0, 0);led_wheel(9, 0, 0, 0);led_wheel(10, 0, 0, 0);led_wheel(11, 0, 0, 0);
    break;
  case 5://5-8 led blink red
    if (tempo_cnt_blink == 120)
      tempo_blink = -4;
    if (tempo_cnt_blink == 0)
      tempo_blink = 4;
    
    tempo_cnt_blink += tempo_blink;
    led_wheel(0, 0, 120, 0);led_wheel(1, 0, 120, 0);led_wheel(2, 0, 120, 0);led_wheel(3, 0, 120, 0);
    led_wheel(4, tempo_cnt_blink, 0, 0);
    led_wheel(5, tempo_cnt_blink, 0, 0);
    led_wheel(6, tempo_cnt_blink, 0, 0);
    led_wheel(7, tempo_cnt_blink, 0, 0);
    led_wheel(8, 0, 0, 0);led_wheel(9, 0, 0, 0);led_wheel(10, 0, 0, 0);led_wheel(11, 0, 0, 0);
    break;
  case 6://9-12 led blink red
    if (tempo_cnt_blink == 120)
      tempo_blink = -tempo_cnt_temp;
    if (tempo_cnt_blink == 0)
      tempo_blink = tempo_cnt_temp;
    
    tempo_cnt_blink += tempo_blink;
    led_wheel(0, 0, 120, 0);led_wheel(1, 0, 120, 0);led_wheel(2, 0, 120, 0);led_wheel(3, 0, 120, 0);
    led_wheel(4, 0, 120, 0);led_wheel(5, 0, 120, 0);led_wheel(6, 0, 120, 0);led_wheel(7, 0, 120, 0);
    led_wheel(8, tempo_cnt_blink, 0, 0);
    led_wheel(9, tempo_cnt_blink, 0, 0);
    led_wheel(10, tempo_cnt_blink, 0, 0);
    led_wheel(11, tempo_cnt_blink, 0, 0);
    break;
  case 7://all led blink red
    if (tempo_cnt_blink == 120)
      tempo_blink = -tempo_cnt_temp;
    if (tempo_cnt_blink == 0)
      tempo_blink = tempo_cnt_temp;
    
    tempo_cnt_blink += tempo_blink;
    led_wheel(0, tempo_cnt_blink, 0, 0);
    led_wheel(1, tempo_cnt_blink, 0, 0);
    led_wheel(2, tempo_cnt_blink, 0, 0);
    led_wheel(3, tempo_cnt_blink, 0, 0);
    led_wheel(4, tempo_cnt_blink, 0, 0);
    led_wheel(5, tempo_cnt_blink, 0, 0);
    led_wheel(6, tempo_cnt_blink, 0, 0);
    led_wheel(7, tempo_cnt_blink, 0, 0);    
    led_wheel(8, tempo_cnt_blink, 0, 0);
    led_wheel(9, tempo_cnt_blink, 0, 0);
    led_wheel(10, tempo_cnt_blink, 0, 0);
    led_wheel(11, tempo_cnt_blink, 0, 0);
    break;
  case 8://all led red
    led_wheel(0, 120, 0, 0);
    led_wheel(1, 120, 0, 0);
    led_wheel(2, 120, 0, 0);
    led_wheel(3, 120, 0, 0);
    led_wheel(4, 120, 0, 0);
    led_wheel(5, 120, 0, 0);
    led_wheel(6, 120, 0, 0);
    led_wheel(7, 120, 0, 0);    
    led_wheel(8, 120, 0, 0);
    led_wheel(9, 120, 0, 0);
    led_wheel(10, 120, 0, 0);
    led_wheel(11, 120, 0, 0);
    break;
  case 9://all led blink yellow
    if (tempo_cnt_blink == 120)
      tempo_blink = -tempo_cnt_temp;
    if (tempo_cnt_blink == 0)
      tempo_blink = tempo_cnt_temp;
    
    tempo_cnt_blink += tempo_blink;
    led_wheel(0, tempo_cnt_blink, tempo_cnt_blink, 0);
    led_wheel(1, tempo_cnt_blink, 0, 0);
    led_wheel(2, tempo_cnt_blink, tempo_cnt_blink, 0);
    led_wheel(3, tempo_cnt_blink, 0, 0);
    led_wheel(4, tempo_cnt_blink, tempo_cnt_blink, 0);
    led_wheel(5, tempo_cnt_blink, 0, 0);
    led_wheel(6, tempo_cnt_blink, tempo_cnt_blink, 0);
    led_wheel(7, tempo_cnt_blink, 0, 0);
    led_wheel(8, tempo_cnt_blink, tempo_cnt_blink, 0);
    led_wheel(9, tempo_cnt_blink, 0, 0);
    led_wheel(10, tempo_cnt_blink, tempo_cnt_blink, 0);
    led_wheel(11, tempo_cnt_blink, 0, 0);
    break;
  }
  
}
/*
void led_wheel(uint8_t led, uint8_t r ,uint8_t g, uint8_t b)
{
  
  for (uint16_t clr = DELAY_LEN; clr < ARRAY_LEN; )
  {   
    for (uint8_t bits = 0; bits < 8; bits++)
    {
      led_ws2812[clr] = led_pwm[r][cnt];
      clr++;
    }
    for (uint8_t bits = 0; bits < 8; bits++)
    {
      led_ws2812[clr] = led_pwm[g][cnt];
      clr++;
    }
    for (uint8_t bits = 0; bits < 8; bits++)
    {
      led_ws2812[clr] = led_pwm[b][cnt];
      clr++;
    }    
  } 
}*/

const uint16_t led_pwm[128][8] = 
{
24,24,24,24,24,24,24,24,
24,24,24,24,24,24,24,44, 
24,24,24,24,24,24,44, 24,
24,24,24,24,24,24,44, 44, 
24,24,24,24,24,44, 24,24,
24,24,24,24,24,44, 24,44, 
24,24,24,24,24,44, 44, 24,
24,24,24,24,24,44, 44, 44, 
24,24,24,24,44, 24,24,24,
24,24,24,24,44, 24,24,44, 
24,24,24,24,44, 24,44, 24,
24,24,24,24,44, 24,44, 44, 
24,24,24,24,44, 44, 24,24,
24,24,24,24,44, 44, 24,44, 
24,24,24,24,44, 44, 44, 24,
24,24,24,24,44, 44, 44, 44, 
24,24,24,44, 24,24,24,24,
24,24,24,44, 24,24,24,44, 
24,24,24,44, 24,24,44, 24,
24,24,24,44, 24,24,44, 44, 
24,24,24,44, 24,44, 24,24,
24,24,24,44, 24,44, 24,44, 
24,24,24,44, 24,44, 44, 24,
24,24,24,44, 24,44, 44, 44, 
24,24,24,44, 44, 24,24,24,
24,24,24,44, 44, 24,24,44, 
24,24,24,44, 44, 24,44, 24,
24,24,24,44, 44, 24,44, 44, 
24,24,24,44, 44, 44, 24,24,
24,24,24,44, 44, 44, 24,44, 
24,24,24,44, 44, 44, 44, 24,
24,24,24,44, 44, 44, 44, 44, 
24,24,44, 24,24,24,24,24,
24,24,44, 24,24,24,24,44, 
24,24,44, 24,24,24,44, 24,
24,24,44, 24,24,24,44, 44, 
24,24,44, 24,24,44, 24,24,
24,24,44, 24,24,44, 24,44, 
24,24,44, 24,24,44, 44, 24,
24,24,44, 24,24,44, 44, 44, 
24,24,44, 24,44, 24,24,24,
24,24,44, 24,44, 24,24,44, 
24,24,44, 24,44, 24,44, 24,
24,24,44, 24,44, 24,44, 44, 
24,24,44, 24,44, 44, 24,24,
24,24,44, 24,44, 44, 24,44, 
24,24,44, 24,44, 44, 44, 24,
24,24,44, 24,44, 44, 44, 44, 
24,24,44, 44, 24,24,24,24,
24,24,44, 44, 24,24,24,44, 
24,24,44, 44, 24,24,44, 24,
24,24,44, 44, 24,24,44, 44, 
24,24,44, 44, 24,44, 24,24,
24,24,44, 44, 24,44, 24,44, 
24,24,44, 44, 24,44, 44, 24,
24,24,44, 44, 24,44, 44, 44, 
24,24,44, 44, 44, 24,24,24,
24,24,44, 44, 44, 24,24,44, 
24,24,44, 44, 44, 24,44, 24,
24,24,44, 44, 44, 24,44, 44, 
24,24,44, 44, 44, 44, 24,24,
24,24,44, 44, 44, 44, 24,44, 
24,24,44, 44, 44, 44, 44, 24,
24,24,44, 44, 44, 44, 44, 44, 
24,44, 24,24,24,24,24,24,
24,44, 24,24,24,24,24,44, 
24,44, 24,24,24,24,44, 24,
24,44, 24,24,24,24,44, 44, 
24,44, 24,24,24,44, 24,24,
24,44, 24,24,24,44, 24,44, 
24,44, 24,24,24,44, 44, 24,
24,44, 24,24,24,44, 44, 44, 
24,44, 24,24,44, 24,24,24,
24,44, 24,24,44, 24,24,44, 
24,44, 24,24,44, 24,44, 24,
24,44, 24,24,44, 24,44, 44, 
24,44, 24,24,44, 44, 24,24,
24,44, 24,24,44, 44, 24,44, 
24,44, 24,24,44, 44, 44, 24,
24,44, 24,24,44, 44, 44, 44, 
24,44, 24,44, 24,24,24,24,
24,44, 24,44, 24,24,24,44, 
24,44, 24,44, 24,24,44, 24,
24,44, 24,44, 24,24,44, 44, 
24,44, 24,44, 24,44, 24,24,
24,44, 24,44, 24,44, 24,44, 
24,44, 24,44, 24,44, 44, 24,
24,44, 24,44, 24,44, 44, 44, 
24,44, 24,44, 44, 24,24,24,
24,44, 24,44, 44, 24,24,44, 
24,44, 24,44, 44, 24,44, 24,
24,44, 24,44, 44, 24,44, 44, 
24,44, 24,44, 44, 44, 24,24,
24,44, 24,44, 44, 44, 24,44, 
24,44, 24,44, 44, 44, 44, 24,
24,44, 24,44, 44, 44, 44, 44, 
24,44, 44, 24,24,24,24,24,
24,44, 44, 24,24,24,24,44, 
24,44, 44, 24,24,24,44, 24,
24,44, 44, 24,24,24,44, 44, 
24,44, 44, 24,24,44, 24,24,
24,44, 44, 24,24,44, 24,44, 
24,44, 44, 24,24,44, 44, 24,
24,44, 44, 24,24,44, 44, 44, 
24,44, 44, 24,44, 24,24,24,
24,44, 44, 24,44, 24,24,44, 
24,44, 44, 24,44, 24,44, 24,
24,44, 44, 24,44, 24,44, 44, 
24,44, 44, 24,44, 44, 24,24,
24,44, 44, 24,44, 44, 24,44, 
24,44, 44, 24,44, 44, 44, 24,
24,44, 44, 24,44, 44, 44, 44, 
24,44, 44, 44, 24,24,24,24,
24,44, 44, 44, 24,24,24,44, 
24,44, 44, 44, 24,24,44, 24,
24,44, 44, 44, 24,24,44, 44, 
24,44, 44, 44, 24,44, 24,24,
24,44, 44, 44, 24,44, 24,44, 
24,44, 44, 44, 24,44, 44, 24,
24,44, 44, 44, 24,44, 44, 44, 
24,44, 44, 44, 44, 24,24,24,
24,44, 44, 44, 44, 24,24,44, 
24,44, 44, 44, 44, 24,44, 24,
24,44, 44, 44, 44, 24,44, 44, 
24,44, 44, 44, 44, 44, 24,24,
24,44, 44, 44, 44, 44, 24,44, 
24,44, 44, 44, 44, 44, 44, 24,
24,44, 44, 44, 44, 44, 44, 44
};