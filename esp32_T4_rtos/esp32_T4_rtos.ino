#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define INCLUDE_vTaskSuspend 1
#define configUSE_TIMERS 1

#define A_PERIOD 2000 //ms
#define B_PERIOD 501 //ms
#define C_PERIOD 98 //ms
#define TIMER_TASK_PRIO 5 //int
//-------------------------------

TimerHandle_t Timer_A;
TimerHandle_t Timer_B;
TimerHandle_t Timer_C;

SemaphoreHandle_t SerialSema;

SemaphoreHandle_t AStart;
SemaphoreHandle_t BStart;
SemaphoreHandle_t CStart;

SemaphoreHandle_t ADone;
SemaphoreHandle_t BDone;
SemaphoreHandle_t CDone;

//-------------------------------

void A_callback(TimerHandle_t Timer_A); 
void TaskA(void *pvParameters);
TaskHandle_t A;
int A_violations = 0;

void B_callback(TimerHandle_t Timer_B); 
void TaskB(void *pvParameters);
TaskHandle_t B;
int B_violations = 0;

void C_callback(TimerHandle_t Timer_C); 
void TaskC(void *pvParameters);
TaskHandle_t C;
int C_violations = 0;

//-------------------------------

void setup(){
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

//---------SEMAPHORES---------//
  //SerialSema = xSemaphoreCreateBinary();
  SerialSema = xSemaphoreCreateMutex();

  AStart = xSemaphoreCreateBinary();
  BStart = xSemaphoreCreateBinary();
  CStart = xSemaphoreCreateBinary();
  
  ADone = xSemaphoreCreateBinary();
  BDone = xSemaphoreCreateBinary();
  CDone = xSemaphoreCreateBinary();

//---------TIMER INITIALIZING----------//
  Timer_A = xTimerCreate("Task A periodicity",A_PERIOD/portTICK_PERIOD_MS, pdTRUE, NULL, A_callback);
  Timer_B = xTimerCreate("Task A periodicity",B_PERIOD/portTICK_PERIOD_MS, pdTRUE, NULL, B_callback);
  Timer_C = xTimerCreate("Task A periodicity",C_PERIOD/portTICK_PERIOD_MS, pdTRUE, NULL, C_callback);
  
  //TaskHandle_t timer_task = NULL;
  TaskHandle_t timer_task = xTimerGetTimerDaemonTaskHandle();
  vTaskPrioritySet(timer_task, TIMER_TASK_PRIO);

//---------TASKS INITIALIZING--------------//
  xTaskCreatePinnedToCore(TaskA, "Task A", 2048, NULL, 1, &A,  ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskB, "Task B", 2048, NULL, 2, &B,  ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskC, "Task C", 2048, NULL, 3, &C,  ARDUINO_RUNNING_CORE);

  xTimerStart(Timer_A, 0);
  xTimerStart(Timer_B, 0);
  xTimerStart(Timer_C, 0);

  xSemaphoreGive(SerialSema);

  xSemaphoreGive(AStart);
  xSemaphoreGive(BStart);
  xSemaphoreGive(CStart)
;
  
  Serial.println("Setup Done!");
  //vTaskStartScheduler();

} void loop(){}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

int Serial_Send(int data){
  Serial.println(data);
  xSemaphoreTake(SerialSema,portMAX_DELAY);
  ets_delay_us(15000);
  xSemaphoreGive(SerialSema);
  return 0;
}

void A_callback(TimerHandle_t Timer_A){
  
  if(xSemaphoreTake(ADone,0) == pdFALSE){
    A_violations++;
    Serial.print("Task A: "); Serial.print(A_violations); 
    Serial.println(" process time violation!");
  }

  xSemaphoreGive(AStart);
}

void B_callback(TimerHandle_t Timer_B){
  
  if(xSemaphoreTake(BDone,0) == pdFALSE){
    B_violations++;
    Serial.print("Task B: "); Serial.print(B_violations); 
    Serial.println(" process time violation!");
  }

  xSemaphoreGive(BStart);
}

void C_callback(TimerHandle_t Timer_C){
  
  if(xSemaphoreTake(CDone,0) == pdFALSE){
    C_violations++;
    Serial.print("Task C: "); Serial.print(C_violations); 
    Serial.println(" process time violation!");
  }

  xSemaphoreGive(CStart);
}

void TaskA(void *pvParameters){
    for(;;)
    {
      xSemaphoreTake(AStart,portMAX_DELAY);

      Serial.println("A!");
      Serial_Send(1);
      //ets_delay_us(16000);
      ets_delay_us(1000);

      xSemaphoreGive(ADone);
    }
}

void TaskB(void *pvParameters){
    for(;;)
    {
      xSemaphoreTake(BStart,portMAX_DELAY);

      Serial.println("B!");
      ets_delay_us(200000);

      xSemaphoreGive(BDone);
    }
}

void TaskC(void *pvParameters){
    for(;;)
    {
      xSemaphoreTake(CStart,portMAX_DELAY);

      Serial.println("C!");
      Serial_Send(3);
      //ets_delay_us(17000);
      ets_delay_us(2000);
      
      xSemaphoreGive(CDone);
    }
}
