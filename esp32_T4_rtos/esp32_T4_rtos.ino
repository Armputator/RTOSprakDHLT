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
SemaphoreHandle_t ASema;
SemaphoreHandle_t BSema;
SemaphoreHandle_t CSema;

//-------------------------------

void TaskA(void *pvParameters);
TaskHandle_t A;
bool A_Done = false;
int A_violations = 0;

void TaskB(void *pvParameters);
TaskHandle_t B;
bool B_Done = false;
int B_violations = 0;

void TaskC(void *pvParameters);
TaskHandle_t C;
bool C_Done = false;
int C_violations = 0;

void A_callback(TimerHandle_t Timer_A); 

void B_callback(TimerHandle_t Timer_B); 

void C_callback(TimerHandle_t Timer_C); 

//-------------------------------

void setup(){
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  SerialSema = xSemaphoreCreateBinary();
  ASema = xSemaphoreCreateBinary();
  BSema = xSemaphoreCreateBinary();
  CSema = xSemaphoreCreateBinary();

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
  xSemaphoreGive(ASema);
  xSemaphoreGive(BSema);
  xSemaphoreGive(CSema);
  
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
  xSemaphoreGive(ASema);
  if(A_Done){
    //vTaskResume(A);
  } else {
    A_violations++;
    Serial.print("Task A: "); Serial.print(A_violations); 
    Serial.println(" process time violation!");
  }
}

void B_callback(TimerHandle_t Timer_B){
  xSemaphoreGive(BSema);
  if(B_Done){
    //vTaskResume(B);
  } else {
    B_violations++;
    Serial.print("Task B: "); Serial.print(B_violations); 
    Serial.println(" process time violation!");
  }
}

void C_callback(TimerHandle_t Timer_C){
  xSemaphoreGive(CSema);
  if(C_Done){
    //vTaskResume(C);
  } else {
    C_violations++;
    Serial.print("Task C: "); Serial.print(C_violations); 
    Serial.println(" process time violation!");
  }
}

void TaskA(void *pvParameters){
    for(;;)
    {
      xSemaphoreTake(ASema,portMAX_DELAY);
      A_Done = false;
      Serial.println("A!");
      Serial_Send(1);
      //ets_delay_us(16000);
      ets_delay_us(1000);
      A_Done = true;
      //vTaskSuspend(NULL);
    }
}

void TaskB(void *pvParameters){
    for(;;)
    {
      xSemaphoreTake(BSema,portMAX_DELAY);
      B_Done = false;
      Serial.println("B!");
      ets_delay_us(200000);
      B_Done = true;
      //vTaskSuspend(NULL);
    }
}

void TaskC(void *pvParameters){
    for(;;)
    {
      xSemaphoreTake(CSema,portMAX_DELAY);
      C_Done = false;
      Serial.println("C!");
      Serial_Send(3);
      //ets_delay_us(17000);
      ets_delay_us(2000);
      C_Done = true;
      //vTaskSuspend(NULL);
    }
}
