#include "debounce.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>

debounceData_t debounceData;
void *function_systick(void *arg);   //thread function
void *function_changePin(void *arg); //thread function

volatile int pinState = 1;

//compile with gcc debounce.c main.c -lpthread -Wall -Wextra -o mai
int main()
{
  printf("Version %s", DEBOUNCE_CAPTION);
  pthread_t thread1, thread2;
  debounceInit(&debounceData, 10, DEBOUNCE_PULL_UP);

  pthread_create(&thread1, NULL, function_systick, NULL);
  pthread_create(&thread2, NULL, function_changePin, NULL);
  while (1)
  {

    usleep(100000);
  }

  return 0;
}

void *function_changePin(void *arg)
{
  static int CounterChangePin = 0;
  // ts.tv_sec = msec / 1000;
  //ts.tv_nsec = (msec % 1000) * 1000000;
  printf("Init...\n");
  struct timespec time = {1, 0}; // 1ms

  while (1)
  {
    nanosleep(&time, &time);
    if (CounterChangePin++ > 5)
    {
      CounterChangePin = 0;
      pinState = 0;
    }
    else
    {
      // Only if debounceIsFallingEdgeCf or debounceIsRisingEdgeCf is true
      // if not remove the following line
      pinState = 1;
    }
    printf("falling edge: %d \n", debounceIsFallingEdgeCf(&debounceData));
    printf("Rising edge: %d \n", debounceIsRisingEdgeCf(&debounceData));
    printf("status: %d \n", debounceGetStatus(&debounceData));
  }
}

void *function_systick(void *arg)
{
  static int CounterChangePin = 0;
  printf("Init...\n");
  struct timespec time = {0, 1000000}; // 1ms
  while (1)
  {
    nanosleep(&time, &time);
    debounceUpdate(&debounceData, pinState);
  }
}
