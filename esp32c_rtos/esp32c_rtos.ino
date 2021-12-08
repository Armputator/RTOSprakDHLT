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
void task_func(uint8_t *argc)
{
    Serial.println(":]");
    Serial.println(*argc);
    return;
};

//int main(void){
void setup(){
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  uint8_t arr[NUM_TASKS];
  int i = 0;
  
  while(i < NUM_TASKS)
  {
    arr[i] = i;
    Serial.print("i:");Serial.print(i,DEC);Serial.print("|a:");Serial.println(arr[i],DEC);
    
    xTaskCreatePinnedToCore(
      Task
      ,  "Task"   // A name just for humans
      ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
      ,  (void *)(arr + i)
      ,  0  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      ,  NULL 
      ,  ARDUINO_RUNNING_CORE);
    i++;
  }
  
  vTaskStartScheduler();
}

void loop(){};
/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

  void Task(void *pvParameters)  // This is a task.
  {
    uint8_t *k;
    k = (uint8_t *) pvParameters;
    
    for(;;){
      ets_delay_us(10000);
      task_func(k);
    }
  }
