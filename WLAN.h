#include <ESP8266WiFi.h>
//wenn esp32 dann die anderen Bibliotheken nutzen
//#include "WiFi.h"
//#include <WiFiClient.h>


//name und Passwort der WLan
char* WLan_Name = "muccc.legacy-2.4GHz";//"RobsTest"; //Com2u.de.WLAN2 //UPCDB86BBB //"muccc.legacy-2.4GHz"
char* WLan_password = "haileris";//"Schiller12"; //SOMMERREGEN05  //"haileris"


WiFiServer server(5045);
WiFiServer serverWebserver(80);
WiFiClient client;
WiFiClient clientWebServer;

IPAddress local_IP(192, 168, 2, 22);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);

bool wasConnectet = false;

class WLAN
{
  public:
    void Initializieren(int countOfMotors);
    void Write(String text);
    void GetLastCommand(String* lastCommand);
    void WebserverTick(NewComand* comand,String* lastCommand);
    void generatePage();
    void Tick();

  private:
    void sendPage();
    String data;

    String text;
    String PageText[54];

    bool Login = true;


    String commandWebServer;
    

    String output5State = "off";
    String output4State = "off";
    String NumberText = "";
    String header;

    int CountOfMotors = 0;

    int zeilencounterMax=0;
    int zeilencounter=0;
    bool writePage=false;
};

void WLAN::Initializieren(int countOfMotors)
{
  CountOfMotors = countOfMotors;
  generatePage();

  if (Login)
  {
    Serial.print("Connecting to ");
    Serial.println(WLan_Name);


    WiFi.mode(WIFI_STA);
    WiFi.begin(WLan_Name, WLan_password);
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
      counter++;
      if (counter == 20)
      {
        Serial.println("WiFi not connected!!!");
        return;
      }
    }

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    //wir geben die IP adresse die uns das Wlan gegeben hat aus
    Serial.println(WiFi.localIP());
    server.begin();
    serverWebserver.begin();
  }
  else
  {

    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP("TEST");
    server.begin();
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());

  }
}

void WLAN::GetLastCommand(String* lastCommand)
{
  if (client.connected() == false)
  {
    if (wasConnectet == true)
    {
      Serial.println("Client verloren");
      wasConnectet = false;
    }
    client = server.available();
    if (client.connected())
    {
      Serial.println("neuer Client");
      wasConnectet = true;
    }
  }
  else
  {
    if (client.available() > 0)
    {
      char c = client.read();
      data += c;
      if (c == ';')
      {
        *lastCommand = data;
        data = "";

      }
    }
  }
}

void WLAN::Write(String text)
{
  //Serial.println("write"+text);
  client.print(text);
}

void WLAN::WebserverTick(NewComand* comand,String* lastCommand)
{
  if (clientWebServer.connected() == false)
  {
    clientWebServer = serverWebserver.available();
  }
  else
  {
    if (clientWebServer.available() > 0)
    {
      char c = clientWebServer.read();
      if (c == '\n')
      {
        if (commandWebServer.length() == 0)
        {
          zeilencounter=0;
          writePage=true;
          //sendPage();
        }
        else
        {
          //was kahm via commandWebServer
          //Serial.println(commandWebServer);

          if (commandWebServer.indexOf("GET /testpin-") >= 0)
          {
            int start = commandWebServer.indexOf("/");
            int stop = commandWebServer.indexOf(";");
            String comandText = commandWebServer.substring(start + 1, stop+1);            
            Serial.println(comandText);
            *lastCommand = comandText;
          }
          if (commandWebServer.indexOf("get?inputPos_") >= 0)
          {
            //inputPos_1=400
            // Serial.println(commandWebServer);
            Serial.println("wert input gefunden");
            int cmdPosMotorNumber = commandWebServer.indexOf("_");
            int cmdPos = commandWebServer.indexOf("=");
            String motorNumberText = commandWebServer.substring(cmdPosMotorNumber + 1, cmdPos);
            String targetText = commandWebServer.substring(cmdPos + 1, commandWebServer.length());
            comand->whatComand = 1;
            comand->motorNumber = motorNumberText.toInt();;
            comand->motorTarget = targetText.toInt();
            Serial.println(comand->motorNumber);
            Serial.println(comand->motorTarget);
          }
          commandWebServer = "";
        }
      }
      else if (c != '\r')
      { // wenn etwas kommt was kein Zeilenumbruch ist,
        commandWebServer += c;      // füge es am Ende von currentLine an
      }
    }
  }
}

void WLAN::sendPage()
{
  Serial.println("Schreibe Seite raus");

  for(int i=0;i<zeilencounterMax;i++)
  {
    clientWebServer.println(PageText[i]);
  }

  // Die HTTP-Antwort wird mit einer Leerzeile beendet
  clientWebServer.println();

  clientWebServer.stop();
  commandWebServer = "";
}

void WLAN::generatePage()
{ 
  PageText[zeilencounterMax++]="HTTP/1.1 200 OK";
  PageText[zeilencounterMax++]="Content-type:text/html";
  PageText[zeilencounterMax++]="Connection: close";
  PageText[zeilencounterMax++]="";
  PageText[zeilencounterMax++]="<!DOCTYPE html><html>";
  PageText[zeilencounterMax++]="<head></head>";
  PageText[zeilencounterMax++]="<body><h1>ESP8266 Motor Steuerung</h1>";
  PageText[zeilencounterMax++]="<body><h1>ESP8266 Motor Steuerung text 2</h1>";


  for (int i = 0; i < 9; i++)
  {
    NumberText = String(i + 1);

    PageText[zeilencounterMax++]="<form action='get'>";
    PageText[zeilencounterMax++]=" Check PIN <input name='inputcheckbox_" + NumberText + "' type='checkbox' onclick=\"if (document.querySelector('[name=inputcheckbox_" + NumberText + "]').checked) { fetch('/testpin-" + NumberText + ",1;'); } else { fetch('/testpin-" + NumberText + ",0;'); }\">";
    PageText[zeilencounterMax++]="</form><br>";
  }

  for (int i = 0; i < CountOfMotors; i++)
  {
    NumberText = String(i + 1);
    PageText[zeilencounterMax++]="<form action='get'>";
    PageText[zeilencounterMax++]="Enter Integer : <input type='number' step='1' min='0' max='12000' name='inputPos_" + NumberText + "'>";
    PageText[zeilencounterMax++]="<input type='submit' value='Submit'>";
    PageText[zeilencounterMax++]="</form><br>";
  }

  PageText[zeilencounterMax++]="<div>";
  PageText[zeilencounterMax++]="<input type='button' id='control_left' value='LEFT' onmousedown='isLeft = true' onmouseup='isLeft = false' onkeydown='isLeft = true' onkeyup='isLeft = false' onfocusout='isLeft = false'>";
  PageText[zeilencounterMax++]="<input type='button' id='control_right' value='RIGHT' onmousedown='isRight = true' onmouseup='isRight = false' onkeydown='isRight = true' onkeyup='isRight = false' onfocusout='isRight = false'>";
  PageText[zeilencounterMax++]="<input type='button' id='control_up' value='UP' onmousedown='isUp = true' onmouseup='isUp = false'>";
  PageText[zeilencounterMax++]="<input type='button' id='control_down' value='DOWN'onmousedown='isDown = true' onmouseup='isDown = false'>";
  PageText[zeilencounterMax++]="</div>";
  PageText[zeilencounterMax++]="<script>let isUp=!1,isLeft=!1,isDown=!1,isRight=!1,isFirstRequestDone=!0,isSecondRequestDone=!0;const handler=setInterval(()=>{if(!isFirstRequestDone||!isSecondRequestDone){console.log('Pending Request!');return}isUp&&(console.log('isUp!'),isFirstRequestDone=!1,isSecondRequestDone=!1,fetch('/get?inputPos_1=600').then(()=>{isFirstRequestDone=!0}),fetch('/get?inputPos_2=600').then(()=>{isSecondRequestDone=!0})),isLeft&&(console.log('isLeft!'),isFirstRequestDone=!1,isSecondRequestDone=!0,fetch('/get?inputPos_1=600').then(()=>{isFirstRequestDone=!0})),isDown&&(console.log('isDown!'),isFirstRequestDone=!1,isSecondRequestDone=!1,fetch('/get?inputPos_1=-600').then(()=>{isFirstRequestDone=!0}),fetch('/get?inputPos_2=-600').then(()=>{isSecondRequestDone=!0})),isRight&&(console.log('isRight!'),isFirstRequestDone=!1,isSecondRequestDone=!0,fetch('/get?inputPos_2=600').then(()=>{isFirstRequestDone=!0}))},300);</script>";


  PageText[zeilencounterMax++]="</body></html>";
  PageText[zeilencounterMax]="";
  
  Serial.println(zeilencounterMax);
}

void WLAN::Tick()
{
  if(!writePage)
  {
    return;
  }
  clientWebServer.println(PageText[zeilencounter]);
  zeilencounter++;

  if((zeilencounterMax-1)==zeilencounter)
  {
    Serial.println("Page fertig");
    clientWebServer.println();
    clientWebServer.stop();
    commandWebServer = "";
    writePage=false;    
  }
}
