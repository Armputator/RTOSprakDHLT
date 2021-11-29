#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define mainDELAY_LOOP_COUNT    ( 0xffffff )

static const char *pcTextForTask1 = "Task 1 is running";
static const char *pcTextForTask2 = "Task 2 is running";

void setup() {
  Serial.begin(115200);
  
  xTaskCreatePinnedToCore(
    vTaskFunction
    ,  "Task1"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  (void*)pcTextForTask1
    ,  1  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  xTaskCreatePinnedToCore(
    vTaskFunction
    ,  "Task2"
    ,  1024  // Stack size
    ,  (void*)pcTextForTask2
    ,  2  // Priority
    ,  NULL 
    ,  ARDUINO_RUNNING_CORE);

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{
  // Empty. Things are done in Tasks.
}

void vTaskFunction(void *pvParameters)  // This is a task.
{
  char *pcTaskName;
  pcTaskName = (char*) pvParameters;
  volatile uint32_t ul;
  for (;;) // A Task shall never return or exit.
  {
    Serial.println(pcTaskName);
    for(ul=0; ul < mainDELAY_LOOP_COUNT; ul++) {
    }
  }
}
