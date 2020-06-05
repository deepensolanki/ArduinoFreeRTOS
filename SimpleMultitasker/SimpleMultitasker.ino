#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 
#include <stdio.h>

QueueHandle_t queue_1;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 20, 4);

void setup() 
{
  Serial.begin(9600);
  queue_1 = xQueueCreate(5, sizeof(int));
  if (queue_1 == NULL) 
  {
    Serial.println("Queue can not be created");
  }
  xTaskCreate(TaskDisplay, "DisplayTask", 128, NULL, 1, NULL);
  xTaskCreate(TaskSensor, "SensorTask", 128, NULL, 3, NULL);
  vTaskStartScheduler();
}
void loop() 
{
  
}
void TaskDisplay(void * pvParameters) 
{
  int sensorData = 0;
  char buf[10];
  lcd.init();
  lcd.backlight();
  while(1) 
  {
    
    if (xQueueReceive(queue_1, &sensorData, portMAX_DELAY) == pdPASS) 
    {
      lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
      sprintf(buf, "%4d", sensorData);
      lcd.print(buf); // Print the string "Hello World!"
      //delay(500);
      //lcd.clear();
    }
  }
}

void TaskSensor(void * pvParameters) 
{
  int sensorData; 
  while(1) 
  {
    Serial.println("\nSensorTask");
    sensorData = analogRead(A0);
    //Serial.println(sensorData);
    xQueueSend(queue_1, &sensorData, portMAX_DELAY);
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}
