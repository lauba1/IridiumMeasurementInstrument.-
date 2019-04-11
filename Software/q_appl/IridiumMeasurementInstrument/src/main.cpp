#include <Arduino.h>
#include <TaskScheduler.h>
#include <Bmp280App.h>
#include <Mcp9808App.h>
#include <Sht31App.h>
#include <Ccs811App.h>
#include <Tsl2591App.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
/*Defines*/
#define TASK_INTERVAL_S_TO_MS(x) (x*1000u)
#define MCP9808_TASK_INTERVAL_S   TASK_INTERVAL_S_TO_MS(30u)
#define BMP280_TASK_INTERVAL_S    TASK_INTERVAL_S_TO_MS(30u)
#define SHT31_TASK_INTERVAL_S     TASK_INTERVAL_S_TO_MS(30u)
#define CCS811_TASK_INTERVAL_S    TASK_INTERVAL_S_TO_MS(30u)
#define TSL2591_TASK_INTERVAL_S   TASK_INTERVAL_S_TO_MS(30u)

#define WIFI_AP "Domcio"
#define WIFI_PASSWORD "NikolaKasia69"
#define TOKEN "YXl2r7XNlydZTsIdM4Vf" 
/*Some testing callback */
void ReadMCP9808_Temp(void);
void ReadBMP280(void);
void ReadSHT31(void);
void ReadCCS811(void);
void ReadTSL2591(void);
void InitWiFi(void);
void reconnect(void);
int I2C_ClearBus(void);

Task cTask1(MCP9808_TASK_INTERVAL_S, TASK_FOREVER,&ReadMCP9808_Temp);
Task cTask2(BMP280_TASK_INTERVAL_S, TASK_FOREVER,&ReadBMP280);
Task cTask3(SHT31_TASK_INTERVAL_S, TASK_FOREVER,&ReadSHT31);
Task cTask4(CCS811_TASK_INTERVAL_S,TASK_FOREVER,&ReadCCS811);
Task cTask5(TSL2591_TASK_INTERVAL_S,TASK_FOREVER,&ReadTSL2591);

Scheduler cRunner;
char thingsboardServer[] = "tb.szymonstuglik.com";
WiFiClient wifiClient;
int TogglePin = 0;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  /*Init sensors*/
  Bmp280App_Init();
  Mcp9808App_Init();
  Sht31App_Init();
  Ccs811App_Init();
  Tsl2591App_Init();
  /*Init WiFi*/ 
  Serial.println("Connected to Cloud");
  InitWiFi(); // connect to WiFi network 
  client.setServer( thingsboardServer, 1883 ); //Set MQTT server
  lastSend = 0;
  /*Init scheduler*/ 
  cRunner.init();
  Serial.println("Initialized scheduler");
  /*Add task*/
  cRunner.addTask(cTask1);
  Serial.println("added t1");
  cRunner.addTask(cTask2);
  Serial.println("added t2");
  cRunner.addTask(cTask3);
  Serial.println("added t3");
  cRunner.addTask(cTask4);
  Serial.println("added t4");
  cRunner.addTask(cTask5);
  Serial.println("added t5");
  /*some delay during startup*/
  delay(5000u);
  /*Enable tasks*/
  cTask1.enable();
  Serial.println("Enabled t1");
  cTask2.enable();
  Serial.println("Enabled t2");
  cTask3.enable();
  Serial.println("Enabled t3");
  cTask4.enable();
  Serial.println("Enabled t4");
  cTask5.enable();
  Serial.println("Enabled t5");
}

void loop() {
  // put your main code here, to run repeatedly:
  cRunner.execute();
    if ( !client.connected() ) {
      Serial.println("Reconnect");
    reconnect();
    
  }
}

void ReadBMP280()
{
  float fPressure = 0;
  TogglePin ^= 1;
  if(TogglePin == 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else
  {
    /* code */
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  
  Serial.print("t1: ");
  Serial.println(millis());

  Bmp280App_Read(&fPressure);
  String sPressure = String(fPressure/100); /*hPa*/
  
  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"PressBmp\":"; payload += sPressure; //payload += ",";
  payload += "}";
  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );
}

void ReadMCP9808_Temp()
{  
  float fTemp = 0;
  Serial.print("t2: ");
  Serial.println(millis());
  Mcp9808App_Read(&fTemp);
  String sTemp = String(fTemp);
  
  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"TempMcp\":"; payload += sTemp; //payload += ",";
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );
  
  int rtn = I2C_ClearBus(); // clear the I2C bus first before calling Wire.begin()
  if (rtn != 0) 
  {
    Serial.println(F("I2C bus error. Could not clear"));
    if (rtn == 1) {
      Serial.println(F("SCL clock line held low"));
    } else if (rtn == 2) {
      Serial.println(F("SCL clock line held low by slave clock stretch"));
    } else if (rtn == 3) {
      Serial.println(F("SDA data line held low"));
    }
  };
  /*checking task telemetry*/
  // Prepare a JSON payload string
  
  String millis = String(rtn);
  String payloadTask = "{";
  payloadTask += "\"TaskMcpError\":"; payloadTask += millis; //payload += ",";
  payloadTask += "}";

  // Send payload
  char attributesTask[100];
  payloadTask.toCharArray( attributesTask, 100 );
  client.publish( "v1/devices/me/telemetry", attributesTask );
  Serial.println( attributesTask );
}

void ReadSHT31(void)
{
  float temp = 0;
  float hum = 0;
  Serial.print("t3: ");
  Serial.println(millis());
  Sht31App_Read(&temp, &hum);

  String temperature = String(temp);
  String humidity = String(hum);


  // Just debug messages
  Serial.print( "Sending temperature and humidity : [" );
  Serial.print( temperature ); Serial.print( "," );
  Serial.print( humidity );
  Serial.print( "]   -> " );

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"tempSht\":"; payload += temperature; payload += ",";
  payload += "\"humSht\":"; payload += humidity;
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );
}

void ReadCCS811(void)
{
  Serial.print("t4: ");
  Serial.println(millis());
  Ccs811App_Read();
}

void ReadTSL2591(void)
{
  float fLux = 0;
  Serial.print("t5: ");
  Serial.println(millis());
  Tsl2591App_Read(&fLux);

  String sLux = String(fLux);
  
  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"LuxTsl\":"; payload += fLux; //payload += ",";
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "v1/devices/me/telemetry", attributes );
  Serial.println( attributes );
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("33f10720-52d0-11e9-af6d-8928b3676fd7", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}

/**
 * This routine turns off the I2C bus and clears it
 * on return SCA and SCL pins are tri-state inputs.
 * You need to call Wire.begin() after this to re-enable I2C
 * This routine does NOT use the Wire library at all.
 *
 * returns 0 if bus cleared
 *         1 if SCL held low.
 *         2 if SDA held low by slave clock stretch for > 2sec
 *         3 if SDA held low after 20 clocks.
 */
int I2C_ClearBus(void) {
#if defined(TWCR) && defined(TWEN)
  TWCR &= ~(_BV(TWEN)); //Disable the Atmel 2-Wire interface so we can control the SDA and SCL pins directly
#endif
  pinMode(SDA, INPUT_PULLUP); // Make SDA (data) and SCL (clock) pins Inputs with pullup.
  pinMode(SCL, INPUT_PULLUP);

  delay(500);  // Wait 2.5 secs. This is strictly only necessary on the first power
  // up of the DS3231 module to allow it to initialize properly,
  // but is also assists in reliable programming of FioV3 boards as it gives the
  // IDE a chance to start uploaded the program
  // before existing sketch confuses the IDE by sending Serial data.

  boolean SCL_LOW = (digitalRead(SCL) == LOW); // Check is SCL is Low.
  if (SCL_LOW) { //If it is held low Arduno cannot become the I2C master. 
    return 1; //I2C bus error. Could not clear SCL clock line held low
  }

  boolean SDA_LOW = (digitalRead(SDA) == LOW);  // vi. Check SDA input.
  int clockCount = 20; // > 2x9 clock

  while (SDA_LOW && (clockCount > 0)) { //  vii. If SDA is Low,
    clockCount--;
  // Note: I2C bus is open collector so do NOT drive SCL or SDA high.
    pinMode(SCL, INPUT); // release SCL pullup so that when made output it will be LOW
    pinMode(SCL, OUTPUT); // then clock SCL Low
    delayMicroseconds(10); //  for >5uS
    pinMode(SCL, INPUT); // release SCL LOW
    pinMode(SCL, INPUT_PULLUP); // turn on pullup resistors again
    // do not force high as slave may be holding it low for clock stretching.
    delayMicroseconds(10); //  for >5uS
    // The >5uS is so that even the slowest I2C devices are handled.
    SCL_LOW = (digitalRead(SCL) == LOW); // Check if SCL is Low.
    int counter = 20;
    while (SCL_LOW && (counter > 0)) {  //  loop waiting for SCL to become High only wait 2sec.
      counter--;
      delay(100);
      SCL_LOW = (digitalRead(SCL) == LOW);
    }
    if (SCL_LOW) { // still low after 2 sec error
      return 2; // I2C bus error. Could not clear. SCL clock line held low by slave clock stretch for >2sec
    }
    SDA_LOW = (digitalRead(SDA) == LOW); //   and check SDA input again and loop
  }
  if (SDA_LOW) { // still low
    return 3; // I2C bus error. Could not clear. SDA data line held low
  }

  // else pull SDA line low for Start or Repeated Start
  pinMode(SDA, INPUT); // remove pullup.
  pinMode(SDA, OUTPUT);  // and then make it LOW i.e. send an I2C Start or Repeated start control.
  // When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus.
  /// A Repeat Start is a Start occurring after a Start with no intervening Stop.
  delayMicroseconds(10); // wait >5uS
  pinMode(SDA, INPUT); // remove output low
  pinMode(SDA, INPUT_PULLUP); // and make SDA high i.e. send I2C STOP control.
  delayMicroseconds(10); // x. wait >5uS
  pinMode(SDA, INPUT); // and reset pins as tri-state inputs which is the default state on reset
  pinMode(SCL, INPUT);
  return 0; // all ok
}
