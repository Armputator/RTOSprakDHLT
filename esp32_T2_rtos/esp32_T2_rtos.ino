#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define NUM_TASKS 5 //max 256 tasks because of task number array (see setup funciton)
#define OUTPUT_PERIOD 1000 //time between two outputs in milliseconds
#define TIMER_TASK_PRIO 10 //priority integer

//-------------------------------

uint32_t counter[NUM_TASKS] = {0};
unsigned long lastincrease = 0;
unsigned long lastoutput = 0;
volatile TimerHandle_t output_tmr;
TaskHandle_t timer_task;

//-------------------------------

void Task(void *pvParameters);
 
void task_func(void *pvParameters);

void Puts(void *pvParameters);

void PutsTMR_callback(TimerHandle_t output_tmr); 

//-------------------------------

void setup(){
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  uint8_t arr[NUM_TASKS]; //maximum of 256 tasks 
  
  for(int i = 0;i<NUM_TASKS;i++)
  {
    arr[i] = i;
    //Serial.print("i:");Serial.print(i,DEC);

    xTaskCreatePinnedToCore(Task, "Task" + i, 2048, (void *)(arr + i), 1, NULL,  ARDUINO_RUNNING_CORE); //all tasks priority 1
    //xTaskCreatePinnedToCore(Task, "Task" + i, 2048, (void *)(arr + i), i, NULL,  ARDUINO_RUNNING_CORE); //tasks increasing priority, 0 to NUM_TASKS - 1
    //xTaskCreatePinnedToCore(Task, "Task" + i, 2048, (void *)(arr + i), (i == 0 ? 1 : (i % 2 == 0 ? 1 : 2)), NULL,  ARDUINO_RUNNING_CORE); //tasks alternating priority between 1 and 2
    
    Serial.print("|a:");Serial.println(arr[i],DEC);
  }
  
  //xTaskCreatePinnedToCore(Puts, "Output", 2048, NULL, NUM_TASKS, NULL,  ARDUINO_RUNNING_CORE);  //priority NUM_TASKS
  //xTaskCreatePinnedToCore(Puts, "Output", 2048, NULL, 1, NULL,  ARDUINO_RUNNING_CORE);  //priority 1
  xTaskCreatePinnedToCore(Puts, "Output", 2048, NULL, NUM_TASKS, NULL,  ARDUINO_RUNNING_CORE);  //priority 3

  output_tmr =  = xTimerCreate("OutputTimer",OUTPUT_PERIOD/portTICK_PERIOD_MS, pdTRUE, NUM_TASKS + 1, PutsTMR_callback);
  timer_task = xTimerGetTimerDaemonTaskHandle();
  vTaskPrioritySet(timer_task, TIMER_TASK_PRIO);
  xTimerStart(output_tmr, 0);
  
  Serial.println("Setup Done!");
} void loop(){}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void PutsTMR_callback( TimerHandle_t output_tmr)
{
  //Serial.println("Resuming Output function");
  vTaskResume(Puts);
}

void task_func(void *pvParameters)
{
  if(true)
  {
    ets_delay_us(1000);
    //Serial.println(":]");
    if(micros() - lastincrease >= 1000)
    {
      (*(uint32_t *)pvParameters)++;
      lastincrease = micros();
    }
    return;
  }
}

void Task(void *pvParameters)
{
  if(true)
  {
    uint8_t *k;
    k = (uint8_t *)pvParameters;
    uint32_t *taskcounter = counter + *k;
    vTaskDelay(1000);
    for(;;)
    {
      //ets_delay_us(1000);
      //func();
      task_func((void *)taskcounter);
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
  }
}

void Puts(void *pvParameters)
{
  vTaskDelay(1000);
  for(;;)
  {
    {
      for(int i = 0;i < NUM_TASKS;i++)
      {
        Serial.print("Counter ");
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(*(counter+i));
      }
      lastoutput = millis();
    }
    vTaskSuspend(NULL);
  }
}
