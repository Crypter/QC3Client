#ifndef QC3CLIENT
#define QC3CLIENT

class QC3ClientClass {
  private:
    enum QC3Pins { //order is important!!!
      dp10k = 0,
      dn10k = 1,
      dp2k2 = 2,
      dn2k2 = 3,
      dp = 4,
      dn = 5,
      MAX
    };
    uint8_t pinsInit = 0, qcInit = 0;
    uint8_t pins[4];
    uint16_t voltageLevel = 5000;
    uint32_t initTimer = 0;
    uint8_t continuousMode = 0;

    void setPinVoltage(QC3Pins pin, int8_t milliVolts) {

      if (pin != dp && pin != dn) return;

      uint8_t dX10k = pins[(uint8_t)pin - 4];
      uint8_t dX2k2 = pins[(uint8_t)pin - 2];

      if (milliVolts == 0) {
        pinMode(dX2k2, OUTPUT);
        pinMode(dX10k, OUTPUT);
        digitalWrite(dX2k2, LOW);
        digitalWrite(dX10k, LOW);
      } else if (milliVolts == 3300) {
        pinMode(dX2k2, OUTPUT);
        pinMode(dX10k, OUTPUT);
        digitalWrite(dX2k2, HIGH);
        digitalWrite(dX10k, HIGH);
      } else if (milliVolts == 600) {
        pinMode(dX2k2, OUTPUT);
        pinMode(dX10k, OUTPUT);
        digitalWrite(dX2k2, LOW);
        digitalWrite(dX10k, HIGH);
      } else if (milliVolts == 2700) { //never used
        pinMode(dX2k2, OUTPUT);
        pinMode(dX10k, OUTPUT);
        digitalWrite(dX2k2, HIGH);
        digitalWrite(dX10k, LOW);
      } else {
        pinMode(dX2k2, INPUT);
        pinMode(dX10k, INPUT);
      }
    }

  public:
    QC3ClientClass() {}

    uint8_t configure(uint8_t DataPositive10k, uint8_t DataPositive2k2, uint8_t DataNegative10k, uint8_t DataNegative2k2) {
      if ( qcInit == 0 ) {
        pins[(int)QC3Pins::dp10k] = DataPositive10k;
        pins[(int)QC3Pins::dp2k2] = DataPositive2k2;
        pins[(int)QC3Pins::dn10k] = DataNegative10k;
        pins[(int)QC3Pins::dn2k2] = DataNegative2k2;
        pinsInit = 1;
        return 0;
      } else {
        return -1;
      }
    }

    uint8_t begin(uint8_t QC2Mode = 0, uint8_t blocking = 1) {
      if (pinsInit) {
        if (initTimer == 0) {
          setPinVoltage(dp, 600);
          setPinVoltage(dn, -1);

          if (blocking) {
            delay(1300);
            setPinVoltage(dp, 600);
            setPinVoltage(dn, 0);
            delay(100);
            initTimer = 1;
            qcInit = 3 - (!!QC2Mode);
            if (qcInit == 3) { //QC3 handshake
              setPinVoltage(dp, 600);
              setPinVoltage(dn, 3300);
              delay(100); //stabilize
              voltageLevel = 5000;
            }
            return 0;
          }
          initTimer = millis() + 1; // +1 to avoid millis() = 0 bug on super fast startup
          return 1;
        } else if (blocking == 0 && millis() - initTimer > 1300) {
          setPinVoltage(dp, 600);
          setPinVoltage(dn, 0);
          delay(100);
          qcInit = 3 - (!!QC2Mode);
          if (qcInit == 3) { //QC3 handshake
            setPinVoltage(dp, 600);
            setPinVoltage(dn, 3300);
            delay(100); //stabilize
            voltageLevel = 50;
          }
          return 0;
        }
      } else if (qcInit) {
        return 0;
      } else {
        return -1;
      }
    }

    void end() {
      setPinVoltage(dp, 0);
      setPinVoltage(dn, 0);
      initTimer = 0;
      qcInit = 0;
    }

    uint16_t setMillivolts(uint16_t millivolts) {
      if (qcInit && millivolts <= 12000 && millivolts >= 3600) {
        uint16_t normalizedMillivolts = millivolts / 100;

        if (normalizedMillivolts % 2) {
          normalizedMillivolts --;
        }
        normalizedMillivolts *= 100;

        if (normalizedMillivolts == voltageLevel) return voltageLevel;

        if (qcInit == 3) {
          if (continuousMode == 0) { //handshake
            setPinVoltage(dp, 600);
            setPinVoltage(dn, 3300);
            delay(1);
            continuousMode = 1;
            voltageLevel = 5000;
          }

          while (voltageLevel < normalizedMillivolts && voltageLevel < 12000 ) {
            setPinVoltage(dp, 3300);
            setPinVoltage(dn, 3300);
            delay(1);
            setPinVoltage(dp, 600);
            setPinVoltage(dn, 3300);
            delay(1);
            voltageLevel += 200;
          }
          while (voltageLevel > normalizedMillivolts && voltageLevel > 3600) {
            setPinVoltage(dp, 600);
            setPinVoltage(dn, 600);
            delay(1);
            setPinVoltage(dp, 600);
            setPinVoltage(dn, 3300);
            delay(1);
            voltageLevel -= 200;
          }
        } else if (qcInit == 2 && (normalizedMillivolts == 12000 || normalizedMillivolts == 9000 || normalizedMillivolts == 5000)) {
          switch (normalizedMillivolts) {
            case 12000:
              setPinVoltage(dp, 600);
              setPinVoltage(dn, 600);
              delay(1);
              voltageLevel = normalizedMillivolts;
              break;
            case 9000:
              setPinVoltage(dp, 3300);
              setPinVoltage(dn, 600);
              delay(1);
              voltageLevel = normalizedMillivolts;
              break;
            case 5000:
              setPinVoltage(dp, 600);
              setPinVoltage(dn, 0);
              delay(1);
              voltageLevel = normalizedMillivolts;
              break;
          }
        } else {
          return 0;
        }
        return voltageLevel;
      }
      return 0;
    }

  uint16_t getMillivolts(){
    return voltageLevel;
  }
};


extern QC3ClientClass QC3Client = QC3ClientClass();
#endif
