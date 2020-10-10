#include "board.h"

uint32_t BaordState = GO_ON;
//call in loop cycle
void PowerStart(void)
{
  mb.registers.one[mbREG_back_timer] = 10;
}

uint32_t GoOnStep = 0;
void PowerLogic(void)
{
  if ((mb.registers.one[mbREG_LedState] == 4) && (mb.registers.one[mbREG_JETSON_ON] > 0))
    mb.registers.one[mbREG_LedState] = 5;//5-8 led blink red
  if ((mb.registers.one[mbREG_LedState] == 5) && (mb.registers.one[mbREG_jetson_usb] > 0))
  {
    mb.registers.one[mbREG_LedState] = 6;//9-12 led blink red    
    mb.u16timeOut = 80;
    mb.registers.one[mbREG_mb_timeout] = mb.u16timeOut;
  }
  if ((mb.registers.one[mbREG_LedState] == 6) && (mb.registers.one[mbREG_mb_timeout] < 50))
    mb.registers.one[mbREG_LedState] = 0; //yellow round
    
  
  switch(BaordState)
  {
    case GO_ON:      
      if (mb.registers.one[mbREG_back_timer] == 0)
      {
        if (mb.registers.one[mbREG_adc_Vbat] < 9000) 
        {
          mb.registers.one[mbREG_back_timer] = 5;
          mb.registers.one[mbREG_shutdown] = 1;  
          mb.registers.one[mbREG_PPM_PON] = 0;
          mb.registers.one[mbREG_SRV_PON] = 0;  
          BaordState = GO_OFF;
        }
        else
        {
          BaordState = WORK;
          mb.registers.one[mbREG_JETSON_ON] = 1;       
        }
      }       
      break;
    case WORK:    
      if (mb.registers.one[mbREG_adc_Vbat] < 9500)              //if low battery
      {
        mb.registers.one[mbREG_back_timer] = 60;
        mb.registers.one[mbREG_shutdown] = 1;  
        mb.registers.one[mbREG_PPM_PON] = 0;
        mb.registers.one[mbREG_SRV_PON] = 0;
        BaordState = GO_OFF;
      }
      else
      {
        if (mb.registers.one[mbREG_button_cmd] == 1)            //soft off all board
        {
          mb.registers.one[mbREG_button_cmd] = 0;
          mb.registers.one[mbREG_back_timer] = 60;
          mb.registers.one[mbREG_shutdown] = 1; 
          mb.registers.one[mbREG_JETSON_SOFF] = 1;
          mb.registers.one[mbREG_PPM_PON] = 0;
          mb.registers.one[mbREG_SRV_PON] = 0;
          mb.registers.one[mbREG_LedState] = 7;//red round blink
          BaordState = GO_OFF;
        }
        else if (mb.registers.one[mbREG_button_cmd] == 2)       //hard off all board
        {         
          mb.registers.one[mbREG_JETSON_SOFF] = 1;
          mb.registers.one[mbREG_back_timer] = 3;
          mb.registers.one[mbREG_shutdown] = 1;           
          BaordState = BRD_OFF;
        }
      }
      //if ()
      break;
    case GO_OFF:
      if ((mb.registers.one[mbREG_jetson_usb] == 0) || (mb.registers.one[mbREG_button_cmd] == 2) || (mb.registers.one[mbREG_back_timer] == 0))
      {
        //mb.registers.one[mbREG_JETSON_ON] = 0;
        mb.registers.one[mbREG_JETSON_SOFF] = 1;
        mb.registers.one[mbREG_5VDC_PON] = 0;
        mb.registers.one[mbREG_back_timer] = 3;     
        mb.registers.one[mbREG_LedState] = 8;//all light red
        BaordState = BRD_OFF;
      }
      break;
    case BRD_OFF:
      if (!mb.registers.one[mbREG_back_timer])
      {
        mb.registers.one[mbREG_ALL_PON] = 0;
      }
      break;
  }
}

//need to call one per seconds
uint32_t BoardBackTimer(void)
{  
  if (mb.registers.one[mbREG_back_timer]) mb.registers.one[mbREG_back_timer]--;
  else 
    return 1;
  return 0;
}