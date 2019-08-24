#include <QC3Client.h>

/*
Components needed:
2 x 10kOhm resistors
2 x 2.2kOhm resistors

Wiring Configuration:
GPIO(Dp_2k2) - [2.2kOhm] -
                           \
                            > - USB Green wire
                           /
GPIO(Dp_10k) - [10kOhm]  -

GPIO(Dn_2k2) - [2.2kOhm] -
                           \
                            > - USB White wire
                           /
GPIO(Dn_10k) - [10kOhm]  -
GND --------------------------- USB Black wire
*/

#define Dp_10k 14 //USB green wire, data positive
#define Dp_2k2 12 //USB green wire, data positive
#define Dn_10k 13 //USB white wire, data negative
#define Dn_2k2 4  //USB white wire, data negative

void setup() {
  Serial.begin(115200);
  Serial.println("START");
  
  Serial.print("Configure pins: ");
  if (QC3Client.configure(Dp_10k, Dp_2k2, Dn_10k, Dn_2k2)==0){
    Serial.println("OK");
  } else {
    Serial.println("Failed");
  }
  
  Serial.print("Initiate QuickCharge 3.0: ");
  if (QC3Client.begin()==0){
    Serial.println("OK");
  } else {
    Serial.println("Failed");
  }
  delay(1000);
}

void loop() {
  Serial.print("Voltage: ");
  Serial.println(QC3Client.setMillivolts(5000)) / 1000.0); //USB 5.0V
  delay(2000);
  
  Serial.print("Voltage: ");
  Serial.println(QC3Client.setMillivolts(5200)) / 1000.0); //USB 5.2V, little boost for some devices
  delay(2000);
  
  Serial.print("Voltage: ");
  Serial.println(QC3Client.setMillivolts(7400)) / 1000.0);  //7.4V Lithium 2S battery
  delay(2000);
  
  Serial.print("Voltage: ");
  Serial.println(QC3Client.setMillivolts(9000)) / 1000.0); //9V Adapter
  delay(2000);
  
  Serial.print("Voltage: ");
  Serial.println(QC3Client.setMillivolts(12000)) / 1000.0); //12V Car battery
  delay(2000);
}
