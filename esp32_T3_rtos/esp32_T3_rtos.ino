#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define INCLUDE_vTaskSuspend 1
#define configUSE_TIMERS 1

#define NUM_TASKS 5 //max 256 tasks because of task number array (see setup funciton)
#define OUTPUT_PERIOD 1000 //time between two outputs in milliseconds
#define TIMER_TASK_PRIO 10 //priority of timer daemon integer

//-------------------------------

uint32_t counter[NUM_TASKS] = {0};
unsigned long lastincrease = 0;
unsigned long lastoutput = 0;

TimerHandle_t output_tmr;
TaskHandle_t timer_task = NULL;
SemaphoreHandle_t puts_semaphore;
QueueHandle_t output_queue;

typedef struct {
  uint8_t t_id;
  uint32_t cnt;
} cntr_info;
//-------------------------------

void Task(void *pvParameters);
 
void task_func(void *pvParameters);

void Puts(void *pvParameters);

void PutsTMR_callback(TimerHandle_t output_tmr); 

//-------------------------------

void setup(){
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  puts_semaphore = xSemaphoreCreateBinary();
  
  uint8_t arr[NUM_TASKS]; //maximum of 256 tasks 

//---------OUTPUT TIMER INITIALIZING----------//
  output_tmr = xTimerCreate("OutputTimer",OUTPUT_PERIOD/portTICK_PERIOD_MS, pdTRUE, (void*)(NUM_TASKS + 1), PutsTMR_callback);
  xTimerStart(output_tmr, 0);
  timer_task = xTimerGetTimerDaemonTaskHandle();
  vTaskPrioritySet(timer_task, TIMER_TASK_PRIO);

//---------TASKS INITIALIZING--------------//
  for(int i = 0;i<NUM_TASKS;i++)
  {
    arr[i] = i;
    //Serial.print("i:");Serial.print(i,DEC);

    xTaskCreatePinnedToCore(Task, "Task" + i, 2048, (void *)(arr + i), 1, NULL,  ARDUINO_RUNNING_CORE); //all tasks priority 1
    //xTaskCreatePinnedToCore(Task, "Task" + i, 2048, (void *)(arr + i), i, NULL,  ARDUINO_RUNNING_CORE); //tasks increasing priority, 0 to NUM_TASKS - 1
    //xTaskCreatePinnedToCore(Task, "Task" + i, 2048, (void *)(arr + i), (i == 0 ? 1 : (i % 2 == 0 ? 1 : 2)), NULL,  ARDUINO_RUNNING_CORE); //tasks alternating priority between 1 and 2
    
    Serial.print("|a:");Serial.println(arr[i],DEC);
  }

//---------QUEUE INTIALIZING----------------//
output_queue = xQueueCreate(NUM_TASKS, sizeof(cntr_info));

//---------OUTPUT TASK INITIALIZING----------//
  //xTaskCreatePinnedToCore(Puts, "Output", 2048, NULL, NUM_TASKS, NULL,  ARDUINO_RUNNING_CORE);  //priority NUM_TASKS
  //xTaskCreatePinnedToCore(Puts, "Output", 2048, NULL, 1, NULL,  ARDUINO_RUNNING_CORE);  //priority 1
  xTaskCreatePinnedToCore(Puts, "Output", 2048, NULL, 3, NULL,  ARDUINO_RUNNING_CORE);  //priority 3

  
  Serial.println("Setup Done!");
} void loop(){}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void PutsTMR_callback( TimerHandle_t output_tmr)
{
  //Serial.println("Resuming Output function");
  xSemaphoreGive(puts_semaphore);
}

void task_func(void *pvParameters)
{
  ets_delay_us(1000);
  //Serial.println(":]");
  if(micros() - lastincrease >= 1000)
  {
    //Serial.println(*(uint8_t *)pvParameters);
    (*(counter + *(uint8_t *)pvParameters))++;
    cntr_info temp_item;
    temp_item.t_id = *(uint8_t *)pvParameters;
    temp_item.cnt = (*(counter + *(uint8_t *)pvParameters));
    xQueueSend(output_queue, (void *)&temp_item, 0);
    lastincrease = micros();
  }
  return;
}

void Task(void *pvParameters)
{
    uint8_t ID;
    ID = *(uint8_t *)pvParameters;
    vTaskDelay(1000);
    for(;;)
    {
      //ets_delay_us(1000);
      //func();
      //Serial.println(*(uint8_t *)(void *)&ID);
      task_func((void *)&ID);
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void Puts(void *pvParameters)
{
  vTaskDelay(1000);
  for(;;)
  {
    xSemaphoreTake(puts_semaphore, portMAX_DELAY);
    {
      for(int i = 0;i < NUM_TASKS;i++)
      {
        cntr_info temp_item;
        xQueueReceive(output_queue, (void*)&temp_item, 0);
        Serial.print("Counter ");
        Serial.print(temp_item.t_id);
        Serial.print(" : ");
        Serial.println(temp_item.cnt);
      }
      lastoutput = millis();
    }
  }
}
