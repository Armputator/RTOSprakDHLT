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
void task_func(void *argc)
{
    Serial.println(*(int *)argc);
    return;
};

//int main(void){
void setup(){
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
 int arr[NUM_TASKS];
  
  for(int i = 0;i<NUM_TASKS;i++)
  {
    arr[i] = i;
    Serial.print("i:");Serial.print(i,DEC);Serial.print("|a:");Serial.println(arr[i],DEC);
    xTaskCreatePinnedToCore(
      Task
      ,  "Task"   // A name just for humans
      ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  (void *)(arr + i)
      ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  NULL 
      ,  ARDUINO_RUNNING_CORE);
  }
  
  vTaskStartScheduler();
}

void loop(){
};
/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

  void Task(void *pvParameters)  // This is a task.
  {
    for(;;){
      (void) pvParameters;
      ets_delay_us(1000);
      task_func(pvParameters);
    }
  }
