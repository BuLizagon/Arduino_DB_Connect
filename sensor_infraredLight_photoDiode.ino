#include <SoftwareSerial.h>

int led = 8;            //LED
int p_sensor = 5;       //포토 다이오드 센서
int l_sensor = 4;       //적외선 센서

SoftwareSerial mySerial(2, 3); // WiFi모듈 RX,TX


String ssid = "";                     //WiFi 이름
String PASSWORD = "";                 //WiFi 비밀번호
String host = "";                     //데이터베이스 주소 or IP주소



//WiFi 연결
void connectWifi() {

  String mode = "AT+CWMODE=3";         //자동 연결 활성화

  mySerial.println(mode);
  delay(1000);

  String join = "AT+CWJAP=\"" + ssid + "\",\"" + PASSWORD + "\"";

  Serial.println("Connect Wifi...");
  mySerial.println(join);
  delay(1000);
  if (mySerial.find("OK"))
  {
    Serial.print("WIFI connect\n");
  } else
  {
    Serial.println("connect timeout\n");
  }
  delay(1000);
}

//php파일을 통해 데이터베이스 간접 연결
void httpclient(int p_input, int l_input) {
  delay(100);
  Serial.println("connect TCP...");
  mySerial.println("AT+CIPSTART=\"TCP\",\"" + host + "\",80");
  delay(500);
  if (Serial.find("ERROR")) return;

  Serial.println("Send data...");
  
  int url1 = p_input;    //포토 다이오드 센서
  int url2 = l_input;    //적외선 센서

  //php파일 데이터베이스 간접 연결
  String cmd = "GET ";       //DB주소/php파일.php?변수=값
  cmd += url1;
  cmd += "&l_sensor=";
  cmd += url2;
  cmd += " HTTP/1.0\r\n\r\n";
  mySerial.print("AT+CIPSEND=");
  mySerial.println(cmd.length());
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if (mySerial.find(">"))
  {
    Serial.print(">");
  }

  else
  {
    mySerial.println("AT+CIPCLOSE");
    Serial.println("connect timeout");
    delay(1000);
    return;
  }

  delay(500);

  mySerial.println(cmd);
  Serial.println(cmd);
  delay(100);
  if (Serial.find("ERROR")) return;
  mySerial.println("AT+CIPCLOSE");
  delay(100);
}

void setup() {

  Serial.begin(9600);
  mySerial.begin(9600);

  connectWifi();
  delay(6000);
}



void loop() {

  int l = digitalRead(l_sensor);      //적외선 센서
  int p = digitalRead(p_sensor);      //포토 다이오드 센서


  while (mySerial.available())
  {
    char response = mySerial.read();
    Serial.write(response);
    if (response == '\r') Serial.print('\n');
  }
  Serial.println("\n==================================\n");
  delay(2000);


  if (p == 1 and l == 0) {
    digitalWrite(led, HIGH);
    Serial.println("감지");
    delay(1000);

    httpclient(p, l);
    delay(1000);
  }
  else {
    digitalWrite(led, LOW);
    Serial.println("없음");
    delay(1000);
  }
  delay(1000);
}
