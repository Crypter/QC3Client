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
    uint16_t voltageLevel = 50;
    uint32_t initTimer = 0;
    uint8_t continuousMode = 0;

    void setPinVoltage(QC3Pins pin, int8_t milliVolts) {

      if (pin != dp && pin != dn) return;

      QC3Pins dX10k = static_cast<QC3ClientClass::QC3Pins>(pin - 4);
      QC3Pins dX2k2 = static_cast<QC3ClientClass::QC3Pins>(pin - 2);

      if (milliVolts == 0) {
        pinMode(dX2k2, OUTPUT);
        pinMode(dX10k, OUTPUT);
        digitalWrite(dX2k2, LOW);
        digitalWrite(dX10k, LOW);
      } else if (milliVolts == 33) {
        pinMode(dX2k2, OUTPUT);
        pinMode(dX10k, OUTPUT);
        digitalWrite(dX2k2, HIGH);
        digitalWrite(dX10k, HIGH);
      } else if (milliVolts == 6) {
        pinMode(dX2k2, OUTPUT);
        pinMode(dX10k, OUTPUT);
        digitalWrite(dX2k2, LOW);
        digitalWrite(dX10k, HIGH);
      } else if (milliVolts == 27) {
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

    uint8_t begin(uint8_t blocking = 1, uint8_t QC2Mode = 0) {
      if (pinsInit) {
        if (initTimer == 0) {
          for (uint8_t i = 0; i < 4; i++) {
            pinMode(pins[i], OUTPUT);
          }
          setPinVoltage(dp, 6);
          setPinVoltage(dn, -1);
          if (blocking) {
            delay(1300);
            setPinVoltage(dn, 0);
            qcInit = 3 - (!!QC2Mode);
            initTimer = 1;
            continuousMode = 0;
            return 0;
          }
          initTimer = millis() + 1; // +1 to avoid millis() = 0 bug on super fast startup
          return 1;
        } else if (blocking == 0 && millis() - initTimer > 1300) {
          setPinVoltage(dp, 6);
          setPinVoltage(dn, 0);
          qcInit = 1;
          continuousMode = 0;
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
      continuousMode = 0;
    }

    float setVoltage(float volts) {
      if (qcInit && volts <= 12.0 && volts >= 3.6) {
        uint8_t integerVolts = volts * 10;
        if (integerVolts % 2) {
          integerVolts --;
        }

        if (integerVolts == voltageLevel) return (float)voltageLevel / 10.0;

        if (qcInit == 3) {
          if (continuousMode == 0) { //handshake
            setPinVoltage(dp, 6);
            setPinVoltage(dn, 33);
            delay(1);
            continuousMode = 1;
            voltageLevel = 50;
          }

          while (voltageLevel < integerVolts ) {
            setPinVoltage(dp, 33);
            setPinVoltage(dn, 33);
            delay(1);
            setPinVoltage(dp, 6);
            setPinVoltage(dn, 33);
            voltageLevel += 2;
          }
          while (voltageLevel > integerVolts) {
            setPinVoltage(dp, 6);
            setPinVoltage(dn, 6);
            delay(1);
            setPinVoltage(dp, 6);
            setPinVoltage(dn, 33);
            voltageLevel -= 2;
          }

        } else if (qcInit == 2 && (integerVolts == 120 || integerVolts == 90 || integerVolts == 50)) {
          switch (integerVolts) {
            case 120:
              setPinVoltage(dp, 6);
              setPinVoltage(dn, 6);
              delay(1);
              voltageLevel = integerVolts;
              break;
            case 90:
              setPinVoltage(dp, 33);
              setPinVoltage(dn, 6);
              delay(1);
              voltageLevel = integerVolts;
              break;
            case 50:
              setPinVoltage(dp, 6);
              setPinVoltage(dn, 0);
              delay(1);
              voltageLevel = integerVolts;
              break;
          }
        } else {
          return -1;
        }
        return (float)voltageLevel / 10.0;
      }
      return -1;
    }

};


extern QC3ClientClass QC3Client = QC3ClientClass();
#endif
