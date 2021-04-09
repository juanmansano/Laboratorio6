#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

#define QUEUEMESSAGE_SIZE 8

osThreadId_t produtor_id, consumidor_id;
osMessageQueueId_t filaMensagem_id;

void produtor(void *arg){
  uint8_t count;
  while(1){
    
    osMessageQueuePut(filaMensagem_id, &count, 0, 0);
    
    count++;
    count &= 0x0F; // produz nova informação
    osDelay(500);
  } // while
} // produtor

void consumidor(void *arg){
  uint8_t state;
  
  while(1){
    
    osMessageQueueGet(filaMensagem_id, &state, NULL, 0);
    
    LEDWrite(LED4 | LED3 | LED2 | LED1, state); // apresenta informação consumida
    osDelay(500);
  } // while
} // consumidor

void main(void){
  SystemInit();
  LEDInit(LED4 | LED3 | LED2 | LED1);

  osKernelInitialize();

  produtor_id = osThreadNew(produtor, NULL, NULL);
  consumidor_id = osThreadNew(consumidor, NULL, NULL);
  
  filaMensagem_id = osMessageQueueNew(QUEUEMESSAGE_SIZE, sizeof(uint8_t), NULL);
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
