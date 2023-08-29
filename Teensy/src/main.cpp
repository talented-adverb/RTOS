#include <FreeRTOS_TEENSY4.h>

// The LED is attached to pin 13 on the Teensy 4.0
const uint8_t LED_PIN = 13;

// Declare a semaphore handle.
SemaphoreHandle_t sem;
//------------------------------------------------------------------------------
/*
 * Thread 1, turn the LED off when signalled by thread 2.
 */
// Declare the thread function for thread 1.
static void Thread1(void* arg) {
  while (1) {

    Serial.println("Thread 1 : Waiting on Thread 2 to turn LED OFF");

    // Wait for signal from thread 2.
    xSemaphoreTake(sem, portMAX_DELAY);

    Serial.println("Thread 1 : Turning LED OFF");

    // Turn LED off.
    digitalWrite(LED_PIN, LOW);
  }
}
//------------------------------------------------------------------------------
/*
 * Thread 2, turn the LED on and signal thread 1 to turn the LED off.
 */
// Declare the thread function for thread 2.
static void Thread2(void* arg) {

  pinMode(LED_PIN, OUTPUT);

  while (1) {
    // Turn LED on.
    digitalWrite(LED_PIN, HIGH);

    Serial.println("Thread 2 : Turning LED ON");

    // Sleep for 200 milliseconds.
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);

    Serial.println("Thread 2 : Asking Thread 1 to turn LED OFF");

    // Signal thread 1 to turn LED off.
    xSemaphoreGive(sem);

    // Sleep for 200 milliseconds.
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
  }
}
//------------------------------------------------------------------------------
void setup() {
  portBASE_TYPE s1, s2;

  Serial.begin(9600);
  
  // initialize semaphore
  sem = xSemaphoreCreateCounting(1, 0);

  // create task at priority two
  s1 = xTaskCreate(Thread1, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  // create task at priority one
  s2 = xTaskCreate(Thread2, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  // check for creation errors
  if (sem== NULL || s1 != pdPASS || s2 != pdPASS ) {
    Serial.println("Creation problem");
    while(1);
  }

  Serial.println("Starting the scheduler !");

  // start scheduler
  vTaskStartScheduler();
  Serial.println("Insufficient RAM");
  while(1);
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
  // Not used.
}


// #include <FreeRTOS_TEENSY4.h>

// static TaskHandle_t task_1=NULL;
// static TaskHandle_t task_2=NULL;

// int n=0,m=0;
// int LED_PIN=13;
// void blink1(void *parameters){
//   for(;;){
//     Serial.print("LED->1 count");
//     Serial.println(n++);
//     digitalWrite(LED_PIN,HIGH);
//     vTaskDelay(500/portTICK_PERIOD_MS);
//     digitalWrite(LED_PIN,LOW);
//     vTaskDelay(500/portTICK_PERIOD_MS); 
//     }
// }
// void blink2(void *parameters){
//   for(;;){
//     Serial.print("LED->2 count");
//     Serial.println(m++);
//     digitalWrite(LED_PIN,HIGH);
//     vTaskDelay(2000/portTICK_PERIOD_MS);
//     digitalWrite(LED_PIN,LOW);
//     vTaskDelay(2000 /portTICK_PERIOD_MS); 
//   }
// }

// void setup() {
//   Serial.begin(300); //lower baud rate to watch the preemption of higher priority task over lower one
//   pinMode(LED_PIN,OUTPUT);
//   vTaskDelay(1000/portTICK_PERIOD_MS);//to give a chance to serial to connect otherwise we miss some output. 
//   xTaskCreate(
//     blink1, //function name 
//     "LED->1", //task name
//     1024, 
//     NULL,//task parameters
//     1,//task priority
//     &task_1
//     );

//   xTaskCreate(
//     blink2, //function name 
//     "LED->2", //task name
//     1024,
//     NULL,//task parameters
//     1,//task priority
//     &task_2// task handle (interact with task from within other task).
//     );
// }

// void loop() {

// }

