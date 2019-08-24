# QC3Client
QuickCharge 3.0 Client for Arduino

Try not to power your device from the same charger without voltage regulator, you might damage it!

LM1117 and AMS1117 regulators can usually handle 9V if the device is not power hungry, and 12V if you are really careful.

### Components needed:

* 2 x 10kOhm resistors
* 2 x 2.2kOhm resistors

### Wiring Configuration:
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
