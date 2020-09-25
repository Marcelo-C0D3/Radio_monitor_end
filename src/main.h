#include "user_interface.h"

class Manager
{
private:
  const int altOnda = 15;
  const int hReport = 13;
  const String marcelo = "5548 ";
  const String hostNumber = "14155238886";
  const String tokenID = " ";
  const String tokenAuth = " ";
  const String hostServer = "http://189.8.209.100";
  const String hostPort = ":8080/";
  const String tittleON = "Som+Maior+-+FM+-+100.7+-+information+Radio:+ON";
  const String tittleOFF = "Som+Maior+-+FM+-+100.7+-+information+Radio:+OFF";
  const String tittleReport = "Relatorio+Radio+100.7+-+24Hrs";
  const String relatControl = "Relatorio de quedas: %A0 ";
  const char *serveName = "Radio_Monitor";
  const char *servePass = "s3nh4321";
  int T = 30000;
  int chargingT = 30000;

public:
  int valor_AD = 0, soma1 = 0, soma2 = 0, soma3 = 0, soma4 = 0, i = 0, i2 = 0, i3 = 0, i4 = 0, x = 1, state = 0, idRelat = 0;
  double med1 = 0, med2 = 0, med3 = 0, med4 = 0;
  const long utcOffsetInSeconds = -10800; //Ajuste fusu-horario BR SC
  
  bool _timeout1 = false, _timeout2 = false, _timeout3 = false;

  String assunto = " ";
  String relatorio = "Relatorio de quedas: %A0 ";
  String queda = " ";
  String day = " ";
  String hours = " ";
  String minutes = " ";
  String seconds = " ";

  os_timer_t mTimer1, mTimer2, mTimer3; //timer 1./timer 2./timer 3.

  String checkWiFiConect();
  void getValues();
  void callMedia();
  void report();
  void timerReport();
  void sending(String subject, String to, String space);

  void usrInit();
  void reset_timer1();
  void reset_timer2();
  void reset_timer3();
};