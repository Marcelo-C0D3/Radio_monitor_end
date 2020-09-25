#include <Arduino.h>
#include <ESP8266HTTPClient.h>                     //Network HTTP librarie
#include <../lib/NTPClient/NTPClient.h>            //Network time protocol lib.
#include <ESP8266WiFi.h>                           //Wifi Propierts to esp8266
#include <WiFiUdp.h>                               //Service UDP, time!
#include <ESP8266WebServer.h>                      //Local WebServer used to serve the configuration portal
#include <WiFiClient.h>
#include <../lib/WiFiManager-master/WiFiManager.h> // WiFi Configuration Magic ( https://github.com/zhouhan0126/WIFIMANAGER-ESP32 ) >> https://github.com/tzapu/WiFiManager (ORIGINAL)
#include <main.h>                                  //Progam Main Radio_Pio_v2

bool shouldSaveConfig = false;                                                                      //Flag Save  config WiFi
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"}; //Dayes of week :D

Manager go; // reference to class main
// Define NTP Client to get time
HTTPClient http;
WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", go.utcOffsetInSeconds);
WiFiManager wifiManager;

void configModeCallback(WiFiManager *myWiFiManager);
void saveConfigCallback();
void tCallback1(void *tCall);
void tCallback2(void *tCall);
void tCallback3(void *tCall);

void setup()
{
  Serial.begin(115200);

  Serial.println(go.checkWiFiConect()); //Verifica e connecta ao wifi.
  //callback para quando entra em modo de configuração AP
  wifiManager.setAPCallback(configModeCallback);         //callback para quando se conecta em uma rede, ou seja, quando passa a trabalhar em modo estação
  wifiManager.setSaveConfigCallback(saveConfigCallback); // save configs
  go.usrInit();
}

void loop()
{

  if (go.i < 30)
  {
    go.getValues();
  }
  else
  {
    go.callMedia();
  }
}

//callback que indica que o ESP entrou no modo AP
String Manager::checkWiFiConect()
{

  if (WiFi.status() != WL_CONNECTED)
  {
    wifiManager.autoConnect(serveName, servePass);
    return "Conexao Reestabelecida!....";
  }
  else
  {
    return "Conectado!!";
  }
}

void configModeCallback(WiFiManager *myWiFiManager)
{
  //  Serial.println("Entered config mode");
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP());                      //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede
}
//callback que indica que salvamos uma nova rede para se conectar (modo estação)
void saveConfigCallback()
{
  //  Serial.println("Should save config");
  Serial.println("Configuração salva");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
}

void Manager::getValues()
{

  valor_AD = analogRead(A0);
  soma1 = soma1 + valor_AD;
  i++;
}
void Manager::callMedia()
{
  switch (x)
  {
  case 1:
    med1 = soma1 / i;
    i = 0, soma1 = 0;
    soma2 = soma2 + med1;
    i2++;
    if (i2 == 15)
    {
      x = 2;
    }
    break;
  case 2:
    med2 = soma2 / i2;
    i2 = 0, soma2 = 0;
    soma3 = soma3 + med2;
    x = 1;
    i3++;
    if (i3 == 10)
    {
      x = 3;
    }
    break;
  case 3:
    med3 = soma3 / i3;
    i3 = 0, soma3 = 0;
    soma4 = soma4 + med3;
    x = 1;
    i4++;
    if (i4 == 5)
    {
      x = 4;
    }
    break;
  case 4:
    med4 = soma4 / i4;
    i4 = 0, soma4 = 0;
    x = 200;
    go.report();
    Serial.println("-");
    Serial.println(med1);
    Serial.println(med2);
    Serial.println(med3);
    Serial.println(med4);
    break;
  case 200:
    if (med4 > altOnda)
    {
      reset_timer2();
      yield();
    }
    x = 1;
    break;
  default:

    break;
  }

  /*
  if (x == 15)
  {
    media1 = soma1 / x;
    x = 0;
    soma1 = 0;
    delay(100);
    go.report();
  }
  else if (x == 3 && media1 > altOnda)
  {
    x++;
    delay(100);
    reset_timer2();
    yield();
  }
  else
  {
    delay(100);
    med = soma / i;
    soma1 = soma1 + med;
    i = 0;
    x++;
    soma = 0;
  }
  */
}

//function to resports.
void Manager::report()
{
  
  if (_timeout3)
  {
    Serial.println("entrou 1");
    timerReport();
    digitalWrite(D5, !digitalRead(D5));
    _timeout3 = false;
    reset_timer3();
  }
  else if (med4 > altOnda && (state == 0 || state == 1))
  {
    Serial.println("entrou 2");
    reset_timer1();
    state = 3;
  }
  else if (_timeout2 && (state == 0 || state == 2))
  {
    Serial.println("entrou 3");
    state = 1;
    assunto = "A+rádio+está+fora+do+ar,+ou+Sem+áudio+perceptível.";

    sending(tittleOFF, marcelo , assunto);
    _timeout1 = false;

    queda = day + ", " + hours + ":" + minutes + ":" + seconds + "%A0";
    relatorio = relatorio + queda;
  }
  else if (med4 > altOnda && _timeout1 && (state == 0 || state == 3))
  {

    Serial.println("entrou 4");
    state = 2;
    assunto = "A+rádio+está+ativa+e+estável.";
    
    sending(tittleON, marcelo , assunto);
    _timeout2 = false;
  }
}

// function to reprot relat in time exact;
void Manager::timerReport()
{

  timeClient.update();
  day = daysOfTheWeek[timeClient.getDay()];
  hours = timeClient.getHours();
  minutes = timeClient.getMinutes();
  seconds = timeClient.getSeconds();

  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.println();

  if (relatorio != relatControl && timeClient.getHours() == hReport && idRelat == 0)
  {
    idRelat = 1;
    assunto = relatorio;
    //sending(tittleReport, "gustavo.lucca@satc.edu.br", assunto);
    //sending(tittleReport, "arthurlessa@gmail.com", assunto);
    sending(tittleReport, marcelo, assunto);
  }
  else if (relatorio == relatControl && timeClient.getHours() == hReport && idRelat == 0)
  {
    idRelat = 1;
    assunto = "A+rádio+não+apresentou+nenhuma+queda+nas+ultimas+24H.";
    //sending(tittleReport, "gustavo.lucca@satc.edu.br", assunto);
    //sending(tittleReport, "arthurlessa@gmail.com", assunto);
    sending(tittleReport, marcelo, assunto);
  }
  else if (timeClient.getHours() == (hReport + 1) && idRelat == 1)
  {
    idRelat = 0;
    relatorio = relatControl;
  }
}

void Manager::sending(String subject, String to, String space)
{
  Serial.println(go.checkWiFiConect()); //Verifica e connecta ao wifi.
  String hostLink = hostServer + hostPort + "?id=" + tokenID + "&token=" + tokenAuth + "&body=" + subject + "%0A%0A" + space + "&from=" + hostNumber + "&to=" + to;
  Serial.println(hostLink);
  // inicia a comunicacao com base na sua url.
  http.begin(client, hostLink);
  int httpCode = http.GET();                          // efetua uma requisicao do tipo get e retorna o código de status da conexao

  if (httpCode == 200)
  {
    Serial.println("Message send.");
  }
  else
  {
    Serial.print("Error sending message: ");
    Serial.println(httpCode);
  }
  http.end();
}
// inicia  Timers
void Manager::usrInit()
{
  os_timer_setfn(&mTimer1, tCallback1, NULL);
  os_timer_arm(&mTimer1, T, true);
  os_timer_setfn(&mTimer2, tCallback2, NULL);
  os_timer_arm(&mTimer2, T, true);
  os_timer_setfn(&mTimer3, tCallback3, NULL);
  os_timer_arm(&mTimer3, chargingT, true);
}

void Manager::reset_timer1()
{
  os_timer_arm(&mTimer1, T, true);
}
void Manager::reset_timer2()
{
  os_timer_arm(&mTimer2, T, true);
}
void Manager::reset_timer3()
{
  os_timer_arm(&mTimer3, chargingT, true);
}

void tCallback1(void *tCall)
{
  go._timeout1 = true;
}

void tCallback2(void *tCall)
{
  go._timeout2 = true;
}

void tCallback3(void *tCall)
{
  go._timeout3 = true;
}