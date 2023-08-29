#include<Arduino.h>
#include<RTOS.h>

#define LED1 18
#define LED2 21

static TaskHandle_t task_1=NULL;
static TaskHandle_t task_2=NULL;
static TaskHandle_t task_3=NULL;

int n=0,m=0,t=0;

void blink1(void *parameters){
  for(;;){
    Serial.print("LED->1 count");
    Serial.println(n++);
    // digitalWrite(LED1,HIGH);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    // digitalWrite(LED1,LOW);
    // vTaskDelay(1000/portTICK_PERIOD_MS); 
    }
}
void blink2(void *parameters){
  for(;;){
    Serial.print("LED->2 count");
    Serial.println(m++);
    // digitalWrite(LED2,HIGH);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    // digitalWrite(LED2,LOW);
    // vTaskDelay(1000 /portTICK_PERIOD_MS); 
  }
}

void blink3(void *parameters){
  for(;;){
    Serial.print("LED->2 count");
    Serial.println(t++);
    // digitalWrite(LED2,HIGH);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    // digitalWrite(LED2,LOW);
    // vTaskDelay(1000 /portTICK_PERIOD_MS); 
  }
}

// void important(){
//   xTaskSuspendAll();
//   delay(2000);
//   xTaskResume();
// }

void setup() {
  Serial.begin(300); //lower baud rate to watch the preemption of higher priority task over lower one
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
//   Serial.println(ESP_LOGI(tag,"Number of cores:%i",portNUM_PROCESSORS));
  vTaskDelay(1000/portTICK_PERIOD_MS);//to give a chance to serial to connect otherwise we miss some output. 
  xTaskCreatePinnedToCore(
    blink1, //function name 
    "LED->1", //task name
    1024, 
    NULL,//task parameters
    1,//task priority
    &task_1,// task handle (interact with task from within other task)
    0); //CORE-0

  xTaskCreatePinnedToCore(
    blink2, //function name 
    "LED->2", //task name
    1024,
    NULL,//task parameters
    1,//task priority
    &task_2,// task handle (interact with task from within other task).
    1); //CORE-1

    
  xTaskCreatePinnedToCore(
    blink3, //function name 
    "LED->3", //task name
    1024,
    NULL,//task parameters
    2,//task priority
    &task_3,// task handle (interact with task from within other task).
    tskNO_AFFINITY); //CORE0/1
}


void loop() {
  // for(int i=0;i<3;i++){
  //   vTaskSuspend(task_2);
  //   vTaskDelay(1000/portTICK_PERIOD_MS);
  //   vTaskResume(task_2);
  //   vTaskDelay(1000/portTICK_PERIOD_MS);
  // }
}
