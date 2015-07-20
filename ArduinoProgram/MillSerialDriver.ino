#include <SPI.h>
#include <SD.h>


char fileName[] = "file.txt";
char data;

int chipSelect = 4;
int RTS = 3;
int CTS = 2;

void setup() {
	pinMode(RTS,OUTPUT);
	pinMode(CTS,INPUT);
	
	digitalWrite(RTS,LOW); //Keep ready to send high, so mill knows data is available
	
  // Open serial communications and wait for port to open:
	Serial.begin(9600);//For usb comms
	Serial1.begin(9600);//For serial comms to mill
	
	Serial.println("Type G to begin");
	
}

void loop() {
	if (Serial.available() > 0){//Read serial data if available
		data = Serial.read();
		
		if (data == 'G'){
			Serial.println("Print started");
			
			if(SD.begin(chipSelect) == 0){
				Serial.println("SD INITIALIZATION ERROR");
				return;
			}else{
				Serial.println("Card initialised successfully");
				
				//Open file from SD card
				File sdFile = SD.open(fileName);
		
				if(sdFile == 1){
					Serial.print(fileName);
					Serial.println(" opened successfully. Beginning print");
		
					while(sdFile.available()>0){//file.available() returns number of bytes available				
					//Write file to serial if clear to send, and file not finished
						if (digitalRead(CTS) == 0){
							Serial1.write(sdFile.read());
						}
						//delay(100);
					}
					Serial.println("Print complete");
					sdFile.close(); //Close file when finished
				}else{
					Serial.println("ERROR, file not found");
				}
			}	
		}
	}	
}