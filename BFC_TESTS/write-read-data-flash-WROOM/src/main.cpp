#include <Arduino.h>
#include <LittleFS.h>

void writeString(String data);
void readStrings();
void clearEntireFlashMemory();

const char* filePath = "/data.txt";

void setup() {
  Serial.begin(115200);

  if (LittleFS.begin()) {
    Serial.println("LittleFS mounted successfully");
  } else {
    Serial.println("Error mounting LittleFS");
  }
    // Uncomment the following line to clear the filesystem - ONLY THE FIRST TIME OF USE (use it carefully)
  //LittleFS.format();
    
    // Uncomment the following line to erase the flash memory (use it carefully)
  //clearEntireFlashMemory(); 



 
  Serial.println("Starting to write data");

for (int i = 0; i < 10; i++) {
    String data = "RTU VIP,18.04,-50,40.123456,-75.987654,123.45,25.5,10,25.67,950.20,100.0" + String(i);
    writeString(data);
}

  readStrings();
}

void loop() {
  // Your main loop code here
}

void writeString(String data) {
  File file = LittleFS.open("/data.txt", "a"); // Open in append mode
  if (file) {
    file.println(data);
    file.close();
    Serial.println("Write complete.");
  } else {
    Serial.println("Failed to open file for writing.");
  }
}

void readStrings() {
  File file = LittleFS.open(filePath, "r");
  if (file) {
    Serial.println("Reading strings from file...");
    while (file.available()) {
      String data = file.readStringUntil('\n');
      Serial.println("String: " + data);
    }
    file.close();
    Serial.println("Read complete.");
  } else {
    Serial.println("Failed to open file for reading.");
  }
}

// Function to clear the entire flash memory
void clearEntireFlashMemory() {
    LittleFS.format();
  if (LittleFS.begin()) {
    Serial.println("LittleFS mounted successfully");
  } else {
    Serial.println("Error mounting LittleFS");
  }
  Serial.println("Erasing the entire flash memory...");
  
  File root = LittleFS.open("/");
  File file = root.openNextFile();

  while (file) {
    Serial.printf("Deleting file: %s\n", file.name());
    file.close();
    LittleFS.remove(file.name());
    file = root.openNextFile();
  }

  Serial.println("Flash memory erasing complete");
}
