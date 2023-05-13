#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd_1(0x3f,16,2);

int BUZZER_LED_FREQ_MAX = 1000;
int BUZZER_LED_FREQ_MIN = 10;

int DIST_MAX = 100;
int DIST_MIN = 10;

int CURRENT_DIST = 0;
int CURRENT_FREQ = 0;

int UPDATE_FREQ_BUZZER = 100;
int UPDATE_FREQ_LCD = 200;

int trigPin = 3;
int echoPin = 4;
int ledPin1 = 8;
int buzzerPin = 12;
bool flag = false;
double distance;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

int current_freq(double cur_dist)
{
  if(DIST_MAX - DIST_MIN == 0){
    return 500;
  }
    return ((cur_dist-DIST_MIN)/(DIST_MAX - DIST_MIN)) * 
  	 (BUZZER_LED_FREQ_MAX - BUZZER_LED_FREQ_MIN) + BUZZER_LED_FREQ_MIN;
  
}

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
  lcd_1.init();
  lcd_1.backlight();
}


void lcd_print(int distance,int curr_freq){
  String freq = String(curr_freq);
    
  
  if(distance>DIST_MIN && distance<DIST_MAX){
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Distance :");
    lcd_1.setCursor(11, 0);
    flag=true;
    lcd_1.print(distance);
  }
  else if(distance>DIST_MAX){
    if(flag)
      lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Distance :");
    lcd_1.setCursor(11, 0);
    lcd_1.print(DIST_MAX);
    flag=false;
  }
  else{
    if(flag)
      lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Distance :");
    lcd_1.setCursor(11, 0);
    lcd_1.print(DIST_MIN);
    flag=false;
  }
  lcd_1.print("cm");
  
  lcd_1.setCursor(0, 1);
  lcd_1.print("Frequency:");
  lcd_1.setCursor(11, 1);
  if(distance>DIST_MIN && distance<DIST_MAX){
    lcd_1.print(freq);
  }
  else if(distance>DIST_MAX){
    lcd_1.print(BUZZER_LED_FREQ_MAX);
  }
  else{
    lcd_1.print(BUZZER_LED_FREQ_MIN);
  }
  lcd_1.print("ms");
}

void buzzer(double distance,int curr_freq){
  
	if(distance <= DIST_MIN)
  {
    digitalWrite(ledPin1, HIGH);
    tone(buzzerPin, 1000, 100);
  }
  else if(distance <= DIST_MAX)
  {
    digitalWrite(ledPin1, HIGH);
    tone(buzzerPin, 1000, 100);
    delay(curr_freq/2);
    digitalWrite(ledPin1, LOW);
    noTone(buzzerPin);
    delay(curr_freq/2);
  }  
  else
  {
    digitalWrite(ledPin1, LOW);
    digitalWrite(buzzerPin, LOW);
  }
  
  
  if(distance > DIST_MIN){
    delay(UPDATE_FREQ_BUZZER);

  }
}

void loop()
{
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  distance = 0.01723 * readUltrasonicDistance(3, 4);
  Serial.println(distance);
  int curr_freq = current_freq(distance);
  lcd_print(distance, curr_freq);
  if(distance > DIST_MIN){
      delay(UPDATE_FREQ_LCD);

  }

  buzzer(distance, curr_freq);
  
}

