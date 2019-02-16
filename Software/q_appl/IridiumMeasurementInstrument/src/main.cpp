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
#define MCP9808_TASK_INTERVAL_S TASK_INTERVAL_S_TO_MS(35u)
#define BMP280_TASK_INTERVAL_S TASK_INTERVAL_S_TO_MS(35u)
#define SHT31_TASK_INTERVAL_S TASK_INTERVAL_S_TO_MS(35u)
#define CCS811_TASK_INTERVAL_S TASK_INTERVAL_S_TO_MS(35u)
#define TSL2591_TASK_INTERVAL_S TASK_INTERVAL_S_TO_MS(1u)

#define WIFI_AP "Domcio"
#define WIFI_PASSWORD "NikolaKasia69"
#define TOKEN "bg3MVAbYlw5kPOSDG460" 
/*Some testing callback */
void ReadMCP9808_Temp(void);
void ReadBMP280(void);
void ReadSHT31(void);
void ReadCCS811(void);
void ReadTSL2591(void);
void InitWiFi();
void reconnect();

Task cTask1(MCP9808_TASK_INTERVAL_S, TASK_FOREVER,&ReadMCP9808_Temp);
Task cTask2(BMP280_TASK_INTERVAL_S, TASK_FOREVER,&ReadBMP280);
Task cTask3(SHT31_TASK_INTERVAL_S, TASK_FOREVER,&ReadSHT31);
Task cTask4(CCS811_TASK_INTERVAL_S,TASK_FOREVER,&ReadCCS811);
Task cTask5(TSL2591_TASK_INTERVAL_S,TASK_FOREVER,&ReadTSL2591);

Scheduler cRunner;
char thingsboardServer[] = "demo.thingsboard.io";
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
  InitWiFi();
  client.setServer( thingsboardServer, 1883 );
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
  Bmp280App_Read(); 
}

void ReadMCP9808_Temp()
{  
  Serial.print("t2: ");
  Serial.println(millis());
  Mcp9808App_Read();
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
  payload += "\"temperature\":"; payload += temperature; payload += ",";
  payload += "\"humidity\":"; payload += humidity;
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
  Serial.print("t5: ");
  Serial.println(millis());
  Tsl2591App_Read();
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
    if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
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