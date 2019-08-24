# QC3Client
QuickCharge 3.0 Client for Arduino

This library is intended for devices with 3.3v logic, such as ESP8266, ESP32, or Arduino boards powered at 3.3v. Usage with 5v logic is not tested.

Try not to power your device from the same charger without voltage regulator, you might damage it!

LM1117 and AMS1117 regulators can usually handle 9V if the device is not power hungry, and 12V if you are really careful.

### Components needed

* 2 x 10kOhm resistors
* 2 x 2.2kOhm resistors

### Wiring Configuration
```
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
```
### Usage
```
#include <QC3Client.h>

void setup(){
  QC3Client.configure(Dp_10k, Dp_2k2, Dn_10k, Dn_2k2);
  QC3Client.begin();
  QC3Client.setMillivolts(12000);
}

void loop(){
}
```
### Credits
Made with the documentation and info provided at:

* http://blog.deconinck.info/post/2017/08/09/Turning-a-Quick-Charge-3.0-charger-into-a-variable-voltage-power-supply
* https://www.hackster.io/PSoC_Rocks/hacking-qualcomm-quick-charge-qc-2-0-3-0-with-attiny85-b7627d
