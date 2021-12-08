#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#define NUM_TASKS 10

// define two tasks for Blink & AnalogRead
void Task(void *pvParameters);
void task_func(int n)
{
    Serial.println(n);
    return;
};

// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  int arr[NUM_TASKS];
  
  for(int i = 0;i<NUM_TASKS;++i)
  {
    arr[i] = i;
    xTaskCreatePinnedToCore(
      Task
      ,  "Task"   // A name just for humans
      ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  (arr + i)
      ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  NULL 
      ,  ARDUINO_RUNNING_CORE);
  }
  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

  void Task(void *argv)  // This is a task.
  {
    (void) pvParameters;
    ets_delay_us(1000);
    task_func(i);
  }
