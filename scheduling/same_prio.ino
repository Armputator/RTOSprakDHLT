#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define mainDELAY_LOOP_COUNT    ( 0xffffff )

void setup() {
  Serial.begin(115200);
  
  xTaskCreatePinnedToCore(
    vTask1
    ,  "Task1"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    vTask2
    ,  "Task2"
    ,  1024  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

void vTask1(void *pvParameters)  // This is a task.
{
  const char *pcTaskName = "Task 1 is running";
  volatile uint32_t ul;
  for (;;) // A Task shall never return or exit.
  {
    Serial.println(pcTaskName);
    for(ul=0; ul < mainDELAY_LOOP_COUNT; ul++) {
    }
  }
}

void vTask2(void *pvParameters)  // This is a task.
{
  const char *pcTaskName = "Task 2 is running";
  volatile uint32_t ul;
  for (;;) // A Task shall never return or exit.
  {
    Serial.println(pcTaskName);
    for(ul=0; ul < mainDELAY_LOOP_COUNT; ul++) {
    }
  }
}
