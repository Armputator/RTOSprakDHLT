#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define INCLUDE_vTaskSuspend 1
#define configUSE_TIMERS 1

#define A_PERIOD 2000
#define B_PERIOD 501
#define C_PERIOD 98
#define TIMER_TASK_PRIO 5
//-------------------------------

TimerHandle_t Timer_A;
TimerHandle_t Timer_B;
TimerHandle_t Timer_C;

//-------------------------------

void TaskA(void *pvParameters);
TaskHandle_t A;
void TaskB(void *pvParameters);
TaskHandle_t B;
void TaskC(void *pvParameters);
TaskHandle_t C;

void A_callback(TimerHandle_t Timer_A); 

void B_callback(TimerHandle_t Timer_B); 

void C_callback(TimerHandle_t Timer_C); 

//-------------------------------

void setup(){
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

//---------TIMER INITIALIZING----------//
  Timer_A = xTimerCreate("Task A periodicity",A_PERIOD/portTICK_PERIOD_MS, pdTRUE, NULL, A_callback);
  Timer_B = xTimerCreate("Task A periodicity",B_PERIOD/portTICK_PERIOD_MS, pdTRUE, NULL, B_callback);
  Timer_C = xTimerCreate("Task A periodicity",C_PERIOD/portTICK_PERIOD_MS, pdTRUE, NULL, C_callback);
  
  xTimerStart(Timer_A, 0);
  xTimerStart(Timer_B, 0);
  xTimerStart(Timer_C, 0);
  
  //TaskHandle_t timer_task = NULL;
  TaskHandle_t timer_task = xTimerGetTimerDaemonTaskHandle();
  TaskPrioritySet(timer_task, TIMER_TASK_PRIO);

//---------TASKS INITIALIZING--------------//
  xTaskCreatePinnedToCore(TaskA, "Task A", 2048, NULL, 1, &A,  ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskB, "Task B", 2048, NULL, 2, &B,  ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskC, "Task C", 2048, NULL, 3, &C,  ARDUINO_RUNNING_CORE);
  
  Serial.println("Setup Done!");
} void loop(){}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void A_callback(TimerHandle_t Timer_A){
  vTaskResume(A);
}

void B_callback(TimerHandle_t Timer_B){
  vTaskResume(B);
}

void C_callback(TimerHandle_t Timer_C){
  vTaskResume(C);
}

void TaskA(void *pvParameters)
{
    for(;;)
    {
      Serial.println("A!");
      ets_delay_us(16000);
      vTaskSuspend(NULL);
    }
}

void TaskB(void *pvParameters)
{
    for(;;)
    {
      Serial.println("B!");
      ets_delay_us(200000);
      vTaskSuspend(NULL);
    }
}

void TaskC(void *pvParameters)
{
    for(;;)
    {
      Serial.println("C!");
      ets_delay_us(17000);
      vTaskSuspend(NULL);
    }
}