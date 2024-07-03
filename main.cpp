#include <Arduino.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Ultrasonic.h>
#include <ESP32Servo.h>

#define R1 "7924d697"
#define B1 "292fe397"
#define G2 "19ece197"
#define B3 "9942dc97"
#define G3 "59f76d98"
#define R4 "b9307f99"
#define G5 "89dae597"
#define R5 "95ad98"
#define B6 "19292099"

#define RID1 "29dc6199"
#define RID2 "6997e397"

const char *ssid = "Discovery Driven";
const char *password = "123456789";

Servo myservo;

int servoPin = 15;

// Define pins for RC522
#define SS_PIN 21
#define RST_PIN 22

#define INDUCTIVE_SENSOR_PIN 27
#define IR_SENSOR_PIN 35

// Create an instance of the server
WebServer server(80);

int randomNumberLvl1;
int randomNumberLvl2;
int randomNumberLvl3;
int randomNumberLvl4;
int randomNumberLvl5;

int currentLevel = 1;

int countLevel1 = 0;
int countLevel2 = 0;
int countLevel3 = 0;
int countLevel4 = 0;
int countLevel5 = 0;

int level = 1;
String instructions = "";
String feedbackMsg = "";

String instructionsL1[] = {"Choose the red color box", "Choose the green color box", "Choose the blue color box"};
String instructionsL2[] = {"Choose the number 1 box", "Choose the number 2 box", "Choose the number 3 box", "Choose the number 4 box", "Choose the number 5 box", "Choose the number 6 box"};
String instructionsL3[] = {"Choose the metal object", "Choose the plastic object", "Choose the wood object", "Choose the rigidform object"};
String instructionsL4[] = {"Choose the red color number 1 box", "Choose the blue color number 1 box", "Choose the green color number 3 box", "Choose the blue color number 3 box", "Choose the green color number 5 box", "Choose the red color number 5 box"};
String instructionsL5[] = {"Pick the answer 12 - 6?", "Pick the answer 1 + 2?", "Pick the answer 3 + 2?", "Pick the answer 7 - 2?", "Pick the answer 5 - 3?", "Pick the answer 2 + 2?", "Pick the answer 6 - 1?", "Pick the answer 7 - 4?", "Pick the answer 4 + 2?", "Pick the answer 10 - 8?"};

void handleRoot()
{
  String html = "<!DOCTYPE html><html><head><title>ESP32 Sensor</title></head><body>";
  html += "<h1>Discovery Driven</h1>";
  html += "<div><h2>Level: <span id='value1'>" + String(level) + "</span></h2>";
  html += "<h3>Instructions: <span id='value2'>" + instructions + "</span></h3>";
  html += "<h2><span id='value3'>" + feedbackMsg + "</span></h2></div>";
  html += "<script>setInterval(() => {";
  html += "fetch('/values').then(response => response.json()).then(data => {";
  html += "document.getElementById('value1').innerText = data.value1;";
  html += "document.getElementById('value2').innerText = data.value2;";
  html += "document.getElementById('value3').innerText = data.value3;";
  html += "});";
  html += "}, 2000);</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleValues()
{
  String jsonResponse = "{\"value1\":\"" + String(level) + "\", \"value2\":\"" + instructions + "\", \"value3\":\"" + feedbackMsg + "\"}";
  server.send(200, "application/json", jsonResponse);
}

void level1();
void level2();
void level3();
void level4();
void level5();
void loop2(void *pvParameters);
long measureDistance();
bool isRigidForm();
bool isBlackPlastic();
bool objectDetected();
void loop2(void *pvParameters);
String check_rfid_available();
int getRandomExcluding(int lowerBound, int upperBound, int excludedValue)
{
  int randomValue;
  do
  {
    randomValue = random(lowerBound, upperBound);
  } while (randomValue == excludedValue);
  return randomValue;
}

void closeGate()
{
  myservo.write(180);
}

void openGate()
{
  myservo.write(0);
}

void gate()
{
  delay(2000);
  openGate();
  delay(2000);
  closeGate();
}

MFRC522 mfrc522_1(SS_PIN, RST_PIN);
Ultrasonic ultrasonic(4, 5);

void setup()
{
  Serial.begin(115200);
  // Connect to Wi-Fi as an AP
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Define routes
  server.on("/", handleRoot);
  server.on("/values", handleValues);

  // Start the server
  server.begin();

  // Pin to the task for Core 0 in esp32
  xTaskCreatePinnedToCore(
      loop2,   // Function to implement the task
      "loop2", // Name of the task
      20000,   // Stack size in bytes
      NULL,    // Task input parameter
      1,       // Priority of the task
      NULL,    // Task handle.
      0        // Core where the task should run
  );
  SPI.begin();
  mfrc522_1.PCD_Init();

  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);          // standard 50 hz servo
  myservo.attach(servoPin, 500, 2500); // attaches the servo on pin 18 to the servo object

  closeGate();
}
void loop2(void *pvParameters)
{
  while (1)
  {
    // Handle client requests
    server.handleClient();
  }
}

void loop()
{

  switch (currentLevel)
  {
  case 1:
    level1();
    break;
  case 2:
    level2();
    break;
  case 3:
    level3();
    break;
  case 4:
    level4();
    break;
  case 5:
    level5();
    break;
  default:
    feedbackMsg = "You Win! Game Completed.";
    break;
  }
  // level3();
}

void level1()
{
  int countLevel1 = 0;
  level = 1;
  int randomNumberLvl1 = random(0, 3); // Generates a number between 0 and 2
  while (countLevel1 < 2)
  {
    switch (randomNumberLvl1)
    {
    case 0:
      Serial.println("Please place a Red RFID card.");
      instructions = instructionsL1[randomNumberLvl1];
      break;
    case 1:
      Serial.println("Please place a Green RFID card.");
      instructions = instructionsL1[randomNumberLvl1];
      break;
    case 2:
      Serial.println("Please place a Blue RFID card.");
      instructions = instructionsL1[randomNumberLvl1];
      break;
    }
    if (objectDetected())
    {
      String uid = check_rfid_available(); // Check for RFID card

      if (!uid.isEmpty())
      {
        bool success = false;

        switch (randomNumberLvl1)
        {
        case 0:
          if (uid == R1 || uid == R4 || uid == R5)
          {
            Serial.println("Red detected, Well done!");
            feedbackMsg = "Well Done!";
            //randomNumberLvl1 = random(0, 3);
            randomNumberLvl1 = getRandomExcluding(0,3,0);
            success = true;
            gate();
          }
          break;
        case 1:
          if (uid == G2 || uid == G3 || uid == G5)
          {
            Serial.println("Green detected, Well done!");
            feedbackMsg = "Well Done!";
            //randomNumberLvl1 = random(0, 3);
            randomNumberLvl1 = getRandomExcluding(0,3,1);
            success = true;
            gate();
          }
          break;
        case 2:
          if (uid == B1 || uid == B3 || uid == B6)
          {
            Serial.println("Blue detected, Well done!");
            feedbackMsg = "Well Done!";
            //randomNumberLvl1 = random(0, 3);
            randomNumberLvl1 = getRandomExcluding(0,3,2);
            success = true;
            gate();
          }
          break;
        }

        if (success)
        {
          countLevel1++;
        }
        else
        {
          Serial.println("Try Again");
          feedbackMsg = "Try Again!";
          gate();
        }
      }
      else
      {
        Serial.println("No RFID card detected.");
        Serial.println("Try Again");
        feedbackMsg = "Try Again!";
        gate();
      }
    }

    delay(100); // Delay to prevent rapid checking
  }

  Serial.println("Level 1 completed.");
  feedbackMsg = "Level 1 completed";
  currentLevel++; // Increment currentLevel after completing level 1
  delay(2000);    // Optional delay before moving to the next level
  // Increment currentLevel or call another function to proceed to the next level
}
void level2()
{
  int countLevel2 = 0;
  level = 2;
  int randomNumberLvl2 = random(1, 7); // Generates a number between 1 and 6
  while (countLevel2 < 2)
  {

    switch (randomNumberLvl2)
    {
    case 1:
      Serial.println("Please place an RFID card for Number 1.");
      instructions = instructionsL2[randomNumberLvl2 - 1];
      break;
    case 2:
      Serial.println("Please place an RFID card for Number 2.");
      instructions = instructionsL2[randomNumberLvl2 - 1];
      break;
    case 3:
      Serial.println("Please place an RFID card for Number 3.");
      instructions = instructionsL2[randomNumberLvl2 - 1];
      break;
    case 4:
      Serial.println("Please place an RFID card for Number 4.");
      instructions = instructionsL2[randomNumberLvl2 - 1];
      break;
    case 5:
      Serial.println("Please place an RFID card for Number 5.");
      instructions = instructionsL2[randomNumberLvl2 - 1];
      break;
    case 6:
      Serial.println("Please place an RFID card for Number 6.");
      instructions = instructionsL2[randomNumberLvl2 - 1];
      break;
    }

    String uid = check_rfid_available(); // Check for RFID card

    if (objectDetected())
    {

      if (!uid.isEmpty())
      {
        bool success = false;

        switch (randomNumberLvl2)
        {
        case 1:
          if (uid == R1 || uid == B1)
          {
            Serial.println("RFID card for Number 1 detected, Well done!");
            feedbackMsg = "Well done!";
            //randomNumberLvl2 = random(1, 7); // Generates a number between 1 and 6
            randomNumberLvl2 = getRandomExcluding(1,7,1);
            success = true;
            gate();
          }
          break;
        case 2:
          if (uid == G2)
          {
            Serial.println("RFID card for Number 2 detected, Well done!");
            feedbackMsg = "Well done!";
            //randomNumberLvl2 = random(1, 7); // Generates a number between 1 and 6
            randomNumberLvl2 = getRandomExcluding(1,7,2);
            success = true;
            gate();
          }
          break;
        case 3:
          if (uid == G3 || uid == B3)
          {
            Serial.println("RFID card for Number 3 detected, Well done!");
            feedbackMsg = "Well done!";
            //randomNumberLvl2 = random(1, 7); // Generates a number between 1 and 6
            randomNumberLvl2 = getRandomExcluding(1,7,3);
            success = true;
            gate();
          }
          break;
        case 4:
          if (uid == R4)
          {
            Serial.println("RFID card for Number 4 detected, Well done!");
            feedbackMsg = "Well done!";
            //randomNumberLvl2 = random(1, 7); // Generates a number between 1 and 6
            randomNumberLvl2 = getRandomExcluding(1,7,4);
            success = true;
            gate();
          }
          break;
        case 5:
          if (uid == R5 || uid == G5)
          {
            Serial.println("RFID card for Number 5 detected, Well done!");
            feedbackMsg = "Well done!";
            //randomNumberLvl2 = random(1, 7); // Generates a number between 1 and 6
            randomNumberLvl2 = getRandomExcluding(1,7,5);
            success = true;
            gate();
          }
          break;
        case 6:
          if (uid == B6)
          {
            Serial.println("RFID card for Number 6 detected, Well done!");
            feedbackMsg = "Well done!";
            //randomNumberLvl2 = random(1, 7); // Generates a number between 1 and 6
            randomNumberLvl2 = getRandomExcluding(1,7,6);
            success = true;
            gate();
          }
          break;
        }

        if (success)
        {
          countLevel2++;
        }
        else
        {
          Serial.println("Try Again");
          feedbackMsg = "Try Again";
          gate();
        }
      }
      else
      {
        Serial.println("No RFID card detected.");
        Serial.println("Try Again");
        feedbackMsg = "Try Again!";
        gate();
      }
    }

    delay(100); // Delay to prevent rapid checking
  }

  Serial.println("Level 2 completed.");
  feedbackMsg = "Level 2 completed";
  currentLevel++; // Increment currentLevel after completing level 2
  delay(2000);    // Optional delay before moving to the next level
}

void level3()
{
  int countLevel3 = 0;
  level = 3;
  int randomMaterial = random(0, 4); // Generates a number between 0 and 3
  while (countLevel3 < 3)
  {

    switch (randomMaterial)
    {
    case 0:
      Serial.println("Please place a metal object.");
      instructions = instructionsL3[0];
      break;
    case 1:
      Serial.println("Please place a black plastic object.");
      instructions = instructionsL3[1];
      break;
    case 2:
      Serial.println("Please place a rigid form object.");
      instructions = instructionsL3[3];
      break;
    case 3:
      Serial.println("Please place a wood object.");
      instructions = instructionsL3[2];
      break;
    }

    bool success = false;
    while (!success)
    {
      if (objectDetected())
      { // Ensure object is detected using ultrasonic sensor
        switch (randomMaterial)
        {
        case 0:
          if (digitalRead(INDUCTIVE_SENSOR_PIN) == 0)
          {
            Serial.println("Metal detected, Well done!");
            feedbackMsg = "Well done!";
            gate();
            randomMaterial = getRandomExcluding(0,4,0);
            success = true;
          }
          break;
        case 1:
          if (isBlackPlastic())
          {
            Serial.println("Black plastic detected, Well done!");
            feedbackMsg = "Well done!";
            gate();
            randomMaterial = getRandomExcluding(0,4,1);
            success = true;
          }
          break;
        case 2:
          if (isRigidForm())
          {
            Serial.println("Rigid Form detected, Well done!");
            feedbackMsg = "Well done!";
            gate();

            randomMaterial = getRandomExcluding(0,4,2);
            success = true;
          }
          break;
        case 3:
          String uid = check_rfid_available();
          if (uid == RID1 || uid == RID2)
          {
            Serial.println("Wood detected, Well done!");
            feedbackMsg = "Well done!";
            gate();
            randomMaterial = getRandomExcluding(0,4,3);
            success = true;
          }
          break;
        }

        if (!success)
        {
          Serial.println("Try Again");
          feedbackMsg = "Try Again!";
          gate();
        }
      }

      delay(100); // Delay to prevent rapid checking
    }

    countLevel3++;
  }

  Serial.println("Level 3 completed.");
  feedbackMsg = "Level 3 completed";
  currentLevel++; // Increment currentLevel after completing level 3
  delay(2000);    // Optional delay before moving to the next level
}

void level4()
{
  int countLevel4 = 0;
  level = 4;
  int randomNumberLvl4 = random(1, 7); // Generates a number between 1 and 6
  while (countLevel4 < 2)
  {

    switch (randomNumberLvl4)
    {
    case 1:
      Serial.println(instructionsL4[randomNumberLvl4 - 1]);
      instructions = instructionsL4[randomNumberLvl4 - 1];
      break;
    case 2:
      Serial.println(instructionsL4[randomNumberLvl4 - 1]);
      instructions = instructionsL4[randomNumberLvl4 - 1];
      break;
    case 3:
      Serial.println(instructionsL4[randomNumberLvl4 - 1]);
      instructions = instructionsL4[randomNumberLvl4 - 1];
      break;
    case 4:
      Serial.println(instructionsL4[randomNumberLvl4 - 1]);
      instructions = instructionsL4[randomNumberLvl4 - 1];
      break;
    case 5:
      Serial.println(instructionsL4[randomNumberLvl4 - 1]);
      instructions = instructionsL4[randomNumberLvl4 - 1];
      break;
    case 6:
      Serial.println(instructionsL4[randomNumberLvl4 - 1]);
      instructions = instructionsL4[randomNumberLvl4 - 1];
      break;
    }

    if (objectDetected())
    {

      String uid = check_rfid_available(); // Check for RFID card

      if (!uid.isEmpty())
      {
        bool success = false;
        switch (randomNumberLvl4)
        {
        case 1:
          if (uid == R1)
          {
            Serial.println("RFID card for R1 detected, Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl4 = random(1, 7); // Generates a number between 1 and 6
            success = true;
            gate();
          }
          break;
        case 2:
          if (uid == B1)
          {
            Serial.println("RFID card for B1 detected, Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl4 = random(1, 7); // Generates a number between 1 and 6
            success = true;
            gate();
          }
          break;
        case 3:
          if (uid == G3)
          {
            Serial.println("RFID card for G3 detected, Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl4 = random(1, 7); // Generates a number between 1 and 6
            success = true;
            gate();
          }
          break;
        case 4:
          if (uid == B3)
          {
            Serial.println("RFID card for B3 detected, Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl4 = random(1, 7); // Generates a number between 1 and 6
            success = true;
            gate();
          }
          break;
        case 5:
          if (uid == G5)
          {
            Serial.println("RFID card for G5 detected, Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl4 = random(1, 7); // Generates a number between 1 and 6
            success = true;
            gate();
          }
          break;
        case 6:
          if (uid == R5)
          {
            Serial.println("RFID card for R5 detected, Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl4 = random(1, 7); // Generates a number between 1 and 6
            success = true;
            gate();
          }
          break;
        }

        if (success)
        {
          countLevel4++;
        }
        else
        {
          Serial.println("Try Again");
          feedbackMsg = "Try Again";
          gate();
        }
      }
      else
      {
        Serial.println("No RFID card detected.");
        Serial.println("Try Again");
        feedbackMsg = "Try Again!";
        gate();
      }
    }

    delay(100); // Delay to prevent rapid checking
  }

  Serial.println("Level 4 completed.");
  feedbackMsg = "Level 4 completed";
  currentLevel++; // Increment currentLevel after completing level 2
  delay(2000);    // Optional delay before moving to the next level
}

void level5()
{
  int countLevel5 = 0;
  level = 5;
  int randomNumberLvl5 = random(1, 11);
  while (countLevel5 < 2)
  {

    switch (randomNumberLvl5)
    {
    case 1:
      Serial.println(instructionsL5[randomNumberLvl5 - 1]);
      instructions = instructionsL5[randomNumberLvl5 - 1];
      break;
    case 2:
      Serial.println(instructionsL5[randomNumberLvl5 - 1]);
      instructions = instructionsL5[randomNumberLvl5 - 1];
      break;
    case 3:
      Serial.println(instructionsL5[randomNumberLvl5 - 1]);
      instructions = instructionsL5[randomNumberLvl5 - 1];
      break;
    case 4:
      Serial.println(instructionsL5[randomNumberLvl5 - 1]);
      instructions = instructionsL5[randomNumberLvl5 - 1];
      break;
    case 5:
      Serial.println(instructionsL5[randomNumberLvl5 - 1]);
      instructions = instructionsL5[randomNumberLvl5 - 1];
      break;
    case 6:
      Serial.println(instructionsL5[randomNumberLvl5 - 1]);
      instructions = instructionsL5[randomNumberLvl5 - 1];
      break;
    case 7:
      Serial.println(instructionsL5[randomNumberLvl5 - 1]);
      instructions = instructionsL5[randomNumberLvl5 - 1];
      break;
    case 8:
      Serial.println(instructionsL5[randomNumberLvl5 - 1]);
      instructions = instructionsL5[randomNumberLvl5 - 1];
      break;
    case 9:
      Serial.println(instructionsL5[randomNumberLvl5 - 1]);
      instructions = instructionsL5[randomNumberLvl5 - 1];
      break;
    case 10:
      Serial.println(instructionsL5[randomNumberLvl5 - 1]);
      instructions = instructionsL5[randomNumberLvl5 - 1];
      break;
    }
    if (objectDetected())
    {

      String uid = check_rfid_available(); // Check for RFID card

      if (!uid.isEmpty())
      {
        bool success = false;
        switch (randomNumberLvl5)
        {

        case 1:
          if (uid == B6)
          {
            Serial.println("Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl5 = random(1, 11); // Generates a number between 1 and 6
            success = true;
            gate();
          }
          break;
        case 2:
          if (uid == B3 || uid == G3)
          {
            Serial.println(" Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl5 = random(1, 11);
            success = true;
            gate();
          }
          break;

        case 3:
          if (uid == R5 || uid == G5)
          {
            Serial.println(" Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl5 = random(1, 11);
            success = true;
            gate();
          }
          break;
        case 4:
          if (uid == R5 || uid == G5)
          {
            Serial.println(" Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl5 = random(1, 11);
            success = true;
            gate();
          }
          break;
        case 5:
          if (uid == G2)
          {
            Serial.println(" Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl5 = random(1, 11);
            success = true;
            gate();
          }
          break;

        case 6:
          if (uid == R4)
          {
            Serial.println(" Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl5 = random(1, 11);
            success = true;
            gate();
          }
          break;
        case 7:
          if (uid == R5 || uid == G5)
          {
            Serial.println(" Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl5 = random(1, 11);
            success = true;
            gate();
          }
          break;
          // 6 3 5 5 2 4 5 3 5 2
        case 8:
          if (uid == B3 || uid == G3)
          {
            Serial.println(" Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl5 = random(1, 11);
            success = true;
            gate();
          }
          break;
        case 9:
          if (uid == B6)
          {
            Serial.println(" Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl5 = random(1, 11);
            success = true;
            gate();
          }
          break;
        case 10:
          if (uid == G2)
          {
            Serial.println(" Well done!");
            feedbackMsg = "Well done!";
            randomNumberLvl5 = random(1, 11);
            success = true;
            gate();
          }
          break;
        }

        if (success)
        {
          countLevel5++;
        }
        else
        {
          Serial.println("Try Again");
          feedbackMsg = "Try Again";
          gate();
        }
      }
      else
      {
        Serial.println("No RFID card detected.");
        Serial.println("Try Again");
        feedbackMsg = "Try Again!";
        gate();
      }
    }

    delay(100); // Delay to prevent rapid checking
  }

  Serial.println("Level 5 completed.");
  feedbackMsg = "Level 5 completed";
  currentLevel++; // Increment currentLevel after completing level 2
  delay(2000);    // Optional delay before moving to the next level
}

String check_rfid_available()
{
  if (mfrc522_1.PICC_IsNewCardPresent() && mfrc522_1.PICC_ReadCardSerial())
  {
    String rfid = "";
    for (byte i = 0; i < mfrc522_1.uid.size; i++)
    {
      rfid += String(mfrc522_1.uid.uidByte[i], HEX);
    }
    mfrc522_1.PICC_HaltA();
    return rfid;
  }
  return ""; // Return empty string if no card detected
}

bool isBlackPlastic()
{
  // Implement logic to detect black plastic using ultrasonic and IR sensors
  // Return true if black plastic is detected, otherwise return false
  // For example, using a threshold distance and IR sensor value
  long distance = measureDistance();
  int irValue = analogRead(IR_SENSOR_PIN);
  if (distance < 10 && irValue > 100)
  { // Adjust threshold values as needed
    return true;
  }
  return false;
}

bool isRigidForm()
{
  // Implement logic to detect wood using ultrasonic and IR sensors
  // Return true if wood is detected, otherwise return false
  // For example, using a different threshold distance and IR sensor value
  long distance = measureDistance();
  int irValue = analogRead(IR_SENSOR_PIN);
  int metal = digitalRead(INDUCTIVE_SENSOR_PIN);
  String uid = check_rfid_available();
  if (distance < 8 && irValue < 100 && metal == 1 && uid.isEmpty())
  { // Adjust threshold values as needed
    return true;
  }
  return false;
}

long measureDistance()
{
  return ultrasonic.read();
}

bool objectDetected()
{
  // Implement logic to detect if any object is present using ultrasonic sensor
  long distance = measureDistance();
  if (distance < 5)
  { // Adjust threshold value as needed
    return true;
  }
  return false;
}