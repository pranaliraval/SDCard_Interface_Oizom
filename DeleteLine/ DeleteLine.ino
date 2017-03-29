#include "SparkJson.h"
#include "SdFat.h"

STARTUP(cellular_credentials_set("isafe","","",NULL));
// Primary SPI with DMA
// SCK => A3, MISO => A4, MOSI => A5, SS => A2 (default)
SdFat sd;
const uint8_t chipSelect = SS;

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
File myFile;
char str[256];                //To store Json String
char line[256];              //To store last line of file
uint32_t position;           //Stores current position in file
uint32_t finalPosition;     //Stores position for rewrite
uint32_t letters;           //Stores num of letters in last line
size_t n;


void setup() {
  Serial.begin(9600);
  // Wait for USB Serial
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


  // open the file for write at end like the "Native SD library"
   if (!myFile.open("jdata.txt", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening jdata.txt for write failed");
  }
  // if the file opened okay, write to it:
  Serial.print("Writing to jdata.txt...");
  myFile.println(str);
  myFile.println(str);
  myFile.println(str);
  // close the file:
  myFile.close();
  Serial.println("done.");

  // re-open the file for reading:
  if (!myFile.open("jdata.txt", O_RDWR)) {
    sd.errorHalt("opening jdata.txt for read failed");
  }
  Serial.println("jdata.txt content:");

  // read from the file until there's nothing else in it:
  int jsondata;
  while ((jsondata = myFile.read()) >= 0) {
    Serial.write(jsondata);
  }

  myFile.rewind();             //Take cursor back to beginning of file

  //Read text file line by line and update cursor position
  while((n = myFile.fgets(line, sizeof(line))) > 0) {
    position = myFile.curPosition();
  }

  //Check how many letters last line has
  int i=0;
  while(line[i] != '\n') {
    i++;
    letters = i;

  }

  //Take cursor at beginning of last line
  finalPosition = position - (letters + 2);

  if(!myFile.seekSet(finalPosition)) {
    sd.errorHalt("error in seekSet");
  }

  //Replace line with "*"
  for(int j=1; j<=letters; j++) {
    myFile.write("*");
  }

  //Take cursor at beginning of file
  myFile.rewind();
  // close the file:

  Serial.println();
  Serial.println("After rewrite");
  int rewrite;
  while ((rewrite = myFile.read()) >= 0) {
    Serial.write(rewrite);
  }
  
  myFile.close();
}

void loop() {
  // nothing happens after setup
}
