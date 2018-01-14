#include <Homie.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include "SparkFunHTU21D.h"
#include <EEPROM.h>
#include <Button.h>
#include <ButtonEventCallback.h>
#include <PushButton.h>
#include <Agenda.h>

#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D
#define TEMP_DELTA 1 // delta for thermostat logic

#define DEVICE_MODEL "M-Thermo"
#define FW_VERSION "1.0.4"

uint8_t bender [] = {
0x00, 0x00, 0xFC, 0xFE, 0xFF, 0xAB, 0xAB, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x3C, 0x7E, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF, 0x7E, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0xE0, 0xF0, 0xF8, 0xFC, 0xFC, 0xF8, 0xF0, 0xE0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0E, 0x3F, 0x7F, 0x7F, 0xFF, 0xEF, 0xE7, 0xE1, 0x70, 0x7F, 0x3F, 0x0E, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


//////////////////////////////////
// Tasks definitions            //
//////////////////////////////////
//void readSensor();
//Task t1(10000, TASK_FOREVER, &readSensor);
//Scheduler runner;


Agenda scheduler;
int readSensorsTask;
int debounceButtonsTask;

//////////////////////////////////
// MicroOLED Object Declaration //
//////////////////////////////////

MicroOLED oled(PIN_RESET, DC_JUMPER);  // I2C Example
HTU21D tempHumSensor;

int SCREEN_WIDTH = oled.getLCDWidth();
int SCREEN_HEIGHT = oled.getLCDHeight();
boolean serviceScreen = false;
boolean isDisplayOn = true;
int currentMenuStep = 1;


//////////////////////////////////
// Buttons definitions          //
//////////////////////////////////

PushButton buttonUp = PushButton(14);
PushButton buttonDown = PushButton(12);


//////////////////////////////////
// Temperature & Humidity       //
//////////////////////////////////

float humd;
float temp;
float tempCalibration = -4.0;
int offValue = 15;
int setpoint = offValue;
boolean isHeating = false;


//////////////////////////////////
// Relay definitions            //
//////////////////////////////////

int relayPin = D7;




boolean resetFlag = false;
unsigned long lastTemperatureSent = 0;

HomieNode temperatureNode("temperature", "temperature");
HomieNode setpointNode("setpoint", "temperature");
HomieNode humidityNode("humidity", "humidity");


void setupHandler() {
  temperatureNode.setProperty("unit").send("c");
  setpointNode.setProperty("unit").send("c");
  humidityNode.setProperty("unit").send("%");
}

void readSensor() {
  Serial.println("-------------");
  humd = tempHumSensor.readHumidity();
  temp = tempHumSensor.readTemperature();
  temp = temp+tempCalibration;
  
  controlRelay();
  printDashboard();
  
  if(Homie.isConnected()){
    temperatureNode.setProperty("value").send(String(temp));
    humidityNode.setProperty("value").send(String(humd));
  }

  Serial.print(" Temperature:");
  Serial.print(temp, 1);
  Serial.print("C");
  Serial.print(" Humidity:");
  Serial.print(humd, 1);
  Serial.print("%");
  Serial.println(setpoint);
}

void controlRelay(){
  if (temp < (setpoint - TEMP_DELTA)) {
    digitalWrite(relayPin, HIGH);
    isHeating = true;
  }  
  if (temp > (setpoint + TEMP_DELTA)) {
    digitalWrite(relayPin, LOW);
    isHeating = false;
  } 
}


void printHeatingIcon(){
  oled.line(18, 27, 27, 27);
  oled.pixel(18, 26);
  oled.pixel(18, 25);
  oled.pixel(19, 24);
  oled.pixel(19, 23);
  oled.pixel(18, 21);

  oled.pixel(21, 26);
  oled.pixel(21, 25);
  oled.pixel(22, 24);
  oled.pixel(22, 23);
  oled.pixel(21, 21);

  oled.pixel(24, 26);
  oled.pixel(24, 25);
  oled.pixel(25, 24);
  oled.pixel(25, 23);
  oled.pixel(24, 21);

  oled.pixel(27, 26);
  oled.pixel(27, 25);
  oled.pixel(28, 24);
  oled.pixel(28, 23);
  oled.pixel(27, 21);
}

void printMenu()
{
  oled.clear(PAGE);                                      
  oled.setCursor(0,0);
  oled.setFontType(0);  
  switch(currentMenuStep) { 
    case 1 : 
            oled.println("WiFi"); 
            oled.setCursor(0,16);
            if(WiFi.status() != WL_CONNECTED){
              oled.println("Not Conn"); 
            } else {
              oled.print(Homie.getConfiguration().wifi.ssid);                           
            }
            break;
    case 2 : 
            if(WiFi.status() != WL_CONNECTED){
              oled.println("MAC"); 
              oled.setCursor(0,16);
              oled.println(WiFi.macAddress()); 
            } else {
              oled.println("IP");
              oled.setCursor(0,16);
              oled.print(WiFi.localIP());                           
            }
            break;
    case 3 : 
            oled.println("Device ID");
            oled.setCursor(0,16);
            oled.print(Homie.getConfiguration().deviceId);  
            break;
    case 4 : 
            oled.println("Name");
            oled.setCursor(0,16);
            oled.print(Homie.getConfiguration().name);  
            break;
    case 5 : 
            oled.println("Version");
            oled.setCursor(0,16);
            oled.println(DEVICE_MODEL); 
            oled.println(FW_VERSION);  
            break;
    case 6 : 
            oled.println("Reboot");
            oled.setCursor(0,16);
            oled.println("Hold ANY  button 3s");
            break;
    case 7 : 
            oled.println("Factory");
            oled.setCursor(0,16);
            oled.println("Hold ANY  button 3s");
            break;
    case 8 : 
            oled.println("Exit");
            oled.setCursor(0,16);
            oled.println("Hold ANY  button 3s");
            break;
  }  
  oled.setCursor(0,40);  
  oled.print("^v: Move");  
  oled.display();
}

void printDashboard()
{
  if(serviceScreen == false){
    oled.clear(PAGE);
    oled.drawBitmap(bender);
    oled.setFontType(2);                                      
    oled.setCursor(13,0);                                      
    oled.print(temp, 1);
    oled.setFontType(1);
    oled.setCursor(58,0);
    oled.print("C");
  
    if(setpoint == offValue){
      oled.setFontType(0);
      oled.setCursor(44,20);
      oled.print("OFF"); 
    } else {
      oled.setFontType(0);
      oled.setCursor(33,20);
      oled.print(float(setpoint), 1);
      oled.print("C"); 
    }
  
    oled.setCursor(17,36);
    oled.setFontType(1);
    oled.print(humd, 1);
    oled.setFontType(1);
    oled.setCursor(55,36);
    oled.print("%");
    if(isHeating){
     printHeatingIcon(); 
    }
    oled.display();
  }
}

void configurePushButton(Bounce& bouncedButton){
    // Set the debounce interval to 60ms - default is 10ms
    bouncedButton.interval(60);
}

void factoryReset(){
  oled.clear(PAGE);                                      
  oled.setCursor(0,0);
  oled.setFontType(0);                                      
  oled.print("Factory Reset...");  
  oled.display();
  resetFlag = true;
}

void reboot(){
  Serial.print("reboot");
  oled.clear(PAGE);                                      
  oled.setCursor(0,0);
  oled.setFontType(0);                                      
  oled.print("Reboot...");  
  oled.display();
  ESP.restart();
}

void turnOffScreen(){
  if(!serviceScreen){
    if(isDisplayOn){
      isDisplayOn = false;
      oled.command(DISPLAYOFF);
    }
  }
}

void resetDebounseButtonsTask(){
  scheduler.remove(debounceButtonsTask);
  debounceButtonsTask = scheduler.insert(&turnOffScreen, 15000000, true);
}

void onButtonShortHold(Button& btn, uint16_t duration){
  if(serviceScreen){
    switch(currentMenuStep) {
      case 6 : 
              reboot();
              break;
      case 7 : 
              factoryReset();
              break;
      case 8 : 
              serviceScreen = false;
              currentMenuStep = 1;
              printDashboard();
              break;
    }  
  } else {
    serviceScreen = true;
    currentMenuStep = 1;
    printMenu();
  }
}


void onButtonPressed(Button& btn, uint16_t duration){
  resetDebounseButtonsTask();
  if(!isDisplayOn){
      isDisplayOn = true;
      oled.command(DISPLAYON); 
    } else {
      if(serviceScreen){
        if(duration < 3000){
          if(btn.is(buttonDown)){ // button DOWN is pressed
            if(currentMenuStep == 8){
              currentMenuStep = 1;
            } else {
              currentMenuStep++;
            }
            
          } else {
            if(currentMenuStep == 1){
              currentMenuStep = 8;
            } else {
              currentMenuStep--;
            }
          }
          
          // serviceScreen = false;
          printMenu();
        }
      } else {
        if(btn.is(buttonUp)){ // button UP is pressed
          if(setpoint < 28){
            setpoint++;
            printDashboard();
            if(Homie.isConnected()){
              setpointNode.setProperty("value").send(String(setpoint));
            }
          }
        } else { // button DOWN is pressed
          if(setpoint > 15){
            setpoint--;
            printDashboard();
            if(Homie.isConnected()){
              setpointNode.setProperty("value").send(String(setpoint));
            }
          }
        }
      }
    }
}

void setup() {
  resetFlag = false;
  serviceScreen = false;
  isDisplayOn = true;
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  oled.begin();
  oled.clear(ALL);
  oled.setFontType(3);                                      
  oled.setCursor(14,24);                                      
  oled.print("Loading...");
  oled.display();  

  //runner.init();
  //runner.addTask(t1);
  //t1.enable();
  
  buttonUp.configureButton(configurePushButton);
  buttonDown.configureButton(configurePushButton);
  buttonUp.onRelease(onButtonPressed);
  buttonDown.onRelease(onButtonPressed);
  
  buttonUp.onHold(3000, onButtonShortHold);
  buttonDown.onHold(3000, onButtonShortHold);
  
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setBrand("M-Tech");
  Homie_setFirmware(DEVICE_MODEL, FW_VERSION);
  Homie.setSetupFunction(setupHandler);
  Homie.disableLedFeedback();

  temperatureNode.advertise("unit");
  temperatureNode.advertise("value");

  setpointNode.advertise("unit");
  setpointNode.advertise("value");

  humidityNode.advertise("unit");
  humidityNode.advertise("value");

  tempHumSensor.begin();
  
  Homie.setup();
  
  readSensor();
  readSensorsTask = scheduler.insert(&readSensor, 10000000, false); // 10 secs
  debounceButtonsTask = scheduler.insert(&turnOffScreen, 15000000, true); // 15 secs
}

void loop() {
  if(resetFlag){
    resetFlag = false;
    Homie.reset();
  }
  Homie.loop();
  scheduler.update();
  buttonUp.update();
  buttonDown.update();
}
