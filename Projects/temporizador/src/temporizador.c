#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

uint8_t state = 0;
osTimerId_t timer1_id, timer2_id, timer3_id, timer4_id;
osThreadId_t app_main_id;

void callback(void *arg){
  state ^= (uint8_t)arg;
  LEDWrite((uint8_t)arg, state);
} // callback

void app_main(void *arg){
  osTimerStart(timer1_id, 10);
  osTimerStart(timer2_id, 5);
  osTimerStart(timer3_id, 4);
  osTimerStart(timer4_id, 1);
  osDelay(osWaitForever);
} // app_main

void main(void){
  SystemInit();
  LEDInit(LED4 | LED3 | LED2 | LED1);
  uint8_t led1 = LED1;
  uint8_t led2 = LED2;
  uint8_t led3 = LED3;
  uint8_t led4 = LED4;

  osKernelInitialize();

  app_main_id = osThreadNew(app_main, NULL, NULL);
  timer1_id = osTimerNew(callback, osTimerPeriodic, (void *)(led1), NULL);
  timer2_id = osTimerNew(callback, osTimerPeriodic, (void *)(led2), NULL);
  timer3_id = osTimerNew(callback, osTimerPeriodic, (void *)(led3), NULL);
  timer4_id = osTimerNew(callback, osTimerPeriodic, (void *)(led4), NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main