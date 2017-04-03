#include "SparkJson.h"
#include "SdFat.h"

STARTUP(cellular_credentials_set("isafe","","",NULL));

SdFat sd;
const uint8_t chipSelect = SS;

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
File myFile;
char str[256];
char rd[256];
char line[256];
int r = 0;
size_t n;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    SysCall::yield();
  }

  Serial.println("Type any character to start");
  while (Serial.read() <= 0) {
    SysCall::yield();
  }

  //Create Json string
  root["sensor"] = "gps";
  root["time"] = 1351824120;

  JsonArray& data = root.createNestedArray("data");
  data.add(48.756080, 6);  // 6 is the number of decimals to print
  data.add(2.302038, 6);   // if not specified, 2 digits are printed

  root.printTo(str, sizeof(str));

  // Initialize SdFat or print a detailed error message and halt
  // Use half speed like the native library.
  // Change to SPI_FULL_SPEED for more performance.
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }
}



bool CheckNet() {
  r = random(10);
  Serial.println(r);
  if(r%2 == 0) {
    Serial.println("Net available");
    return TRUE;
  }
  else {
    Serial.println("Net not available");
    return FALSE;
  }
}

void WriteLine() {
  if(!myFile.open("logfile.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("Opening logfile.txt for write failed");
  }
  myFile.seekEnd();
  myFile.println(str);
  myFile.println("1234");
  myFile.println("5678");
  myFile.close();
}

void SendLine() {
  if(!myFile.open("logfile.txt", O_RDWR)) {
    sd.errorHalt("Opening logfile.txt for read failed");
  }
  while((n = myFile.fgets(rd, sizeof(rd))) > 0){
    line = rd;
  }
}
void loop() {
/*  bool n = CheckNet();
  if(n == 0){
    WriteLine();
  }
  else {
    SendLine();
  }
  Serial.println(n);

  delay(2000); */
}
