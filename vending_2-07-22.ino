/*This code will display the product details and respective amount on the OLED display and will wait for the user to press button1 or button2 to select the product. 
When the user presses button1, servo1 will rotate to dispense the first product and when the user presses button2, servo2 will rotate to dispense the second product. The code uses the Servo library to control the servo motors and the Adafruit SSD1306 library to control the OLED display. It also uses digitalRead() function to check the state of the buttons and the servo.write() function to rotate the servos to dispense the products. Additionally, 
it also has a pre-introduction slide that will be displayed on the OLED at the start of the program, asking the user to insert a 5 Rupee coin to continue*/
//This is unlikely to be of much use in a practical applications
//ID OF USER: Rakeshtsg
//INTERFACE: Arduino 
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_ADDR 0x3C
#define TRIGGER_PIN 12
#define ECHO_PIN 11

Servo servo1;
Servo servo2;
Adafruit_SSD1306 display(OLED_ADDR);
int coin_pin = 8;
int button1 = 2;
int button2 = 3;
int servo1_pos = 0;
int servo2_pos = 0;
int distance;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
  pinMode(coin_pin, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servo1.attach(9);
  servo2.attach(10);
  
  // Introduction slide
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Welcome to our Vending Machine");
  display.setCursor(0,1);
  display.println("Insert a 5 Rupee coin to continue");
  display.display();
  delay(3000);
}

void loop() {
  if (digitalRead(coin_pin) == HIGH) {
    int coin_value = checkCoin();
    if (coin_value == 5) {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Coin Accepted");
      display.display();
      delay(1000);
      checkStock();
    } else {
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("Invalid Coin");
      display.display();
      delay(1000);
    }
  }
}

int checkCoin() {
  int coin_value = 0;
  int coin_count = 0;
  while (digitalRead(coin_pin) == HIGH) {
    coin_count++;
  }
  if (coin_count > 100 && coin_count < 200) {
    coin_value = 5;
  }
  return coin_value;
}

void checkStock() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  distance = pulseIn(ECHO_PIN, HIGH);
  distance = distance / 2 / 29.1;
  if (distance > 0 && distance < 10) {
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Out of Stock");
    display.display();
    delay(1000);
  } else {
    displayProduct();
  }
}

void displayProduct() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Product 1 - Rs. 5");
  display.setCursor(0,1);
  display.println("Product 2 - Rs. 5");
  display.setCursor(0,2);
  display.println("Please press button 1 or 2");
  display.display();
  while (digitalRead(button1) == LOW && digitalRead(button2) == LOW) {
    if (digitalRead(button1) == HIGH) {
      servo1.write(servo1_pos);
      delay(1000);
      break;
    }
    if (digitalRead(button2) == HIGH) {
      servo2.write(servo2_pos);
      delay(1000);
      break;
    }
  }
}
