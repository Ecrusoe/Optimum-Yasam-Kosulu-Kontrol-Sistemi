#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Servo myServo;  // Servo motoru tanımla
const int servoPin = 9;  // Servo motorunun bağlı olduğu pin
const unsigned long intervalServo = 4000UL;  // 4 saniye (4 saniye * 1000 milisaniye/saniye)
const unsigned long intervalLed = 3000UL;  // 3 saniye (3 saniye * 1000 milisaniye/saniye)
unsigned long previousMillisServo = 0;
unsigned long previousMillisLed = 0;
bool ledState = LOW;  // LED'in başlangıçta kapalı olduğunu belirtir
const int ledPin = 10;
const int buzzerPin = 8;
const int lm35Pin = 0; // data pinini belirliyoruz
float lm35Deger; // gerilim değeri
float sicaklik; // Serial monitörden okuyacağımız sıcaklık değeri
int fan = 6;
const int sensorPin = A1;
int yuzde_nem;

void setup() {
  pinMode(ledPin, OUTPUT);  // LED pinini çıkış olarak ayarlar
  digitalWrite(ledPin, ledState);  // LED'in başlangıç durumunu ayarlar
  pinMode(A1, INPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Calistiriliyor...");
  delay(2000);
  lcd.clear();
  pinMode(buzzerPin, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(lm35Pin, INPUT); // lm35 data pinini giriş olarak ayarladık
  digitalWrite(fan, LOW);
  myServo.attach(servoPin);  // Servo motoru pini tanımla
  myServo.write(0);  // Servo motorunu başlangıçta 0 dereceye ayarla
  Serial.begin(9600);
}

void loop() {
  int veri = analogRead(A1);
  lm35Deger = analogRead(lm35Pin); // data bacagındaki veriyi okuduk
  lm35Deger = (lm35Deger / 1023) * 5000; // degeri mV 'a dönüştürecek
  sicaklik = lm35Deger / 10.0; // sıcaklık başına 10mV üreteceği ifade
  int sensorValue = analogRead(sensorPin);  // Toprak nem sensöründen değer oku
  yuzde_nem = map(sensorValue, 0, 1023, 0, 100);  // Sensor değerini 0-100 aralığına çevir
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillisLed >= intervalLed) {
    previousMillisLed = currentMillis;
    ledState = !ledState;  // LED durumunu değiştir
    digitalWrite(ledPin, ledState);  // LED'i aç veya kapat
  }

  if (currentMillis - previousMillisServo >= intervalServo) {
    previousMillisServo = currentMillis;
    myServo.write(90);  // Servo motorunu 90 dereceye ayarla
    delay(4000);  // 4 saniye bekle
    myServo.write(0);  // Servo motorunu tekrar 0 dereceye ayarla
  }

  // Sicaklik bilgisi
  lcd.setCursor(0, 0);
  lcd.print("Sicaklik: ");
  lcd.print(sicaklik);
  lcd.print(" C");
  delay(3000);
  lcd.clear();

  if (sicaklik > 37) {
    digitalWrite(fan, HIGH);
    digitalWrite(buzzerPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("sicaklik yuksek");
    lcd.setCursor(0, 1);
    
    
  } else if (sicaklik < 25) {
    digitalWrite(fan, LOW);
    digitalWrite(buzzerPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Sicaklik cok");
    lcd.setCursor(0, 1);
    lcd.print("dusuk");
  } else {
    digitalWrite(fan, LOW);
    digitalWrite(buzzerPin, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Fan ve buzzer");
    lcd.setCursor(0, 1);
    lcd.print("kapali");
  }
  delay(3000);
  lcd.clear();

  // Nem bilgisi
  if (sensorValue < 205) {  // Bu değeri toprak nem sensörünün kalibrasyonuna göre ayarlayın
    digitalWrite(buzzerPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Nem dusuk:");
    lcd.setCursor(0, 1);
    lcd.print("Nem yuzdesi: ");
    lcd.print(yuzde_nem);
  } else if (sensorValue > 500) {
    digitalWrite(buzzerPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Nem fazla:");
    lcd.setCursor(0, 1);
    lcd.print("Nem yuzdesi: ");
    lcd.print(yuzde_nem);
  } else {
    digitalWrite(buzzerPin, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Nem uygun:");
    lcd.setCursor(0, 1);
    lcd.print("Nem yuzdesi: ");
    lcd.print(yuzde_nem);
  }
  delay(3000);
  lcd.clear();

  delay(1000);
}

