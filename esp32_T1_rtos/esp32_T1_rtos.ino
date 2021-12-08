#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define NUM_TASKS 10

//-------------------------------

uint32_t counter[NUM_TASKS] = {0};
unsigned long lastincrease = 0;
unsigned long lastoutput = 0;

//-------------------------------

void Task(void *pvParameters);
 
void task_func(void *pvParameters);

void Puts(void *pvParameters);

//-------------------------------

void setup(){
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  
  uint8_t arr[NUM_TASKS];
  
  for(int i = 0;i<NUM_TASKS;i++)
  {
    arr[i] = i;
    Serial.print("i:");Serial.print(i,DEC);Serial.print("|a:");Serial.println(arr[i],DEC);
    
    xTaskCreatePinnedToCore(Task, "Task" + i, 2048, (void *)(arr + i), 1, NULL,  ARDUINO_RUNNING_CORE);
  }
  xTaskCreatePinnedToCore(Puts, "Output", 2048, NULL, 2, NULL,  ARDUINO_RUNNING_CORE);
  
  Serial.println("Setup Done!");
} void loop(){};

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void task_func(void *pvParameters)
{
    //ets_delay_us(3000);
    Serial.println(":]");
    
    if(micros() - lastincrease >= 1000)
    {
      (*(uint32_t *)pvParameters)++;
      lastincrease = micros();
    }
    return;
};

void func(void){  Serial.println(":]"); }; //unnecessary if task_func works

void Task(void *pvParameters)
{
    uint8_t *k;
    k = (uint8_t *)pvParameters;
    uint32_t *taskcounter = counter + *k;
    for(;;)
    {
      //ets_delay_us(1000);
      //func();
      task_func((void *)taskcounter);
    }
};

void Puts(void *pvParameters)
{
  vTaskDelay(100);
  for(;;)
  {
    if(millis() - lastoutput >= 1000)
    {
      for(int i = 0;i < NUM_TASKS;i++)
      {
        Serial.print("Counter ");
        Serial.print(i);
        Serial.print(" : ");
        Serial.println(*(counter+i));
      }
      lastoutput = millis();
    };
  };
};
