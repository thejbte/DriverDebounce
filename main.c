#include "debounce.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>

debounceData_t debounceData;
void *function_systick(void *arg);   //thread function
void *function_changePin(void *arg); //thread function
void delay_ms(uint32_t ms);



const int stateInit = DEBOUNCE_PULL_DOWN;
volatile int pinState = DEBOUNCE_PULL_DOWN == stateInit? 0: 1;

//compile with gcc debounce.c main.c -lpthread -Wall -Wextra -o main
int main()
{
  printf("Version %s \n", DEBOUNCE_CAPTION);
  pthread_t thread1, thread2;
  debounceInit(&debounceData, 10, 1, DEBOUNCE_COUNTERTICK,NULL, stateInit);
  // debounceInit(&debounceData, 10,DEBOUNCE_WAIT,delay_ms, DEBOUNCE_PULL_UP);

  pthread_create(&thread1, NULL, function_systick, NULL);
  pthread_create(&thread2, NULL, function_changePin, NULL);
  while (1)
  {

    //usleep(100000);
    delay_ms(1000);
  }

  return 0;
}





void *function_changePin(void *arg)
{
  static int CounterChangePin = 0;
  // ts.tv_sec = msec / 1000;
  //ts.tv_nsec = (msec % 1000) * 1000000;
  struct timespec time = {1, 0}; // 1ms

  while (1)
  {
    nanosleep(&time, &time);
    if (CounterChangePin++ > 5)
    {
      CounterChangePin = 0;
      pinState = !pinState ;
      //debounceClearFlag(&debounceData, DEBOUNCE_FALLING);
      //debounceClearFlag(&debounceData, DEBOUNCE_RISING);
    }

    printf("status: %d \n", debounceGetStatus(&debounceData));
    printf("falling edge: %d \n", debounceIsFallingEdgeCf(&debounceData));
    printf("Rising edge: %d \n", debounceIsRisingEdgeCf(&debounceData));
    printf("==================\n");
  }
}

void *function_systick(void *arg)
{

  struct timespec time = {0, 1000000}; // 1ms
  while (1)
  {
    nanosleep(&time, &time);
    debounceUpdate(&debounceData, pinState);
  }
}

void delay_ms(uint32_t ms){
  struct timespec ts;
  ts.tv_sec = ms / 1000;
  ts.tv_nsec = (ms % 1000) * 1000000;
  nanosleep(&ts, &ts);
  //printf("delay -----");
}

//TODO: probar con micro