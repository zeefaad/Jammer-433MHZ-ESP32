#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <ezButton.h>

byte sck = 18;
byte miso = 19;
byte mosi = 23;
byte ss = 5;
int gdo0 = 2;
int gdo2 = 4;

ezButton buttonPause(0);
bool paused = true;

float frequency = 433.92;

byte ccsendingbuffer[60];

void cc1101initialize() {
    ELECHOUSE_cc1101.setSpiPin(sck, miso, mosi, ss);
    ELECHOUSE_cc1101.setGDO(gdo0, gdo2);
    ELECHOUSE_cc1101.Init();
    ELECHOUSE_cc1101.setGDO0(gdo0);
    ELECHOUSE_cc1101.setCCMode(1);
    ELECHOUSE_cc1101.setModulation(2);
    ELECHOUSE_cc1101.setMHZ(frequency);
    ELECHOUSE_cc1101.setDeviation(47.60);
    ELECHOUSE_cc1101.setChannel(0);
    ELECHOUSE_cc1101.setChsp(199.95);
    ELECHOUSE_cc1101.setRxBW(812.50);
    ELECHOUSE_cc1101.setDRate(9.6);
    ELECHOUSE_cc1101.setPA(10);
    ELECHOUSE_cc1101.setSyncMode(2);
    ELECHOUSE_cc1101.setSyncWord(211, 145);
    ELECHOUSE_cc1101.setAdrChk(0);
    ELECHOUSE_cc1101.setAddr(0);
    ELECHOUSE_cc1101.setWhiteData(0);
    ELECHOUSE_cc1101.setPktFormat(0);
    ELECHOUSE_cc1101.setLengthConfig(1);
    ELECHOUSE_cc1101.setPacketLength(0);
    ELECHOUSE_cc1101.setCrc(0);
    ELECHOUSE_cc1101.setCRC_AF(0);
    ELECHOUSE_cc1101.setDcFilterOff(0);
    ELECHOUSE_cc1101.setManchester(0);
    ELECHOUSE_cc1101.setFEC(0);
    ELECHOUSE_cc1101.setPRE(0);
    ELECHOUSE_cc1101.setPQT(0);
    ELECHOUSE_cc1101.setAppendStatus(0);
}

void sendRandomData() {
    randomSeed(0);
    for (int i = 0; i < 60; i++) {
        ccsendingbuffer[i] = (byte)random(255);
    }
    ELECHOUSE_cc1101.SendData(ccsendingbuffer,60);
    Serial.print(F("Jamming frequency : "));
    Serial.print(frequency);
    Serial.println(F(" MHz"));
}

void setup() {
    Serial.begin(115200);
    buttonPause.setDebounceTime(30);
    cc1101initialize();

    if (ELECHOUSE_cc1101.getCC1101()) {
        Serial.println(F("CC1101 initialized. Connection OK"));
    } else {
        Serial.println(F("CC1101 connection error! Check wiring."));
        while (1);
    }
}

void loop() {
    buttonPause.loop();

    if (buttonPause.isPressed()) {
      paused = !paused;
      if (paused) {
        Serial.println(F("STOP"));
      } else {
        Serial.println(F("Play"));
      }
    }

    if (!paused) {
      sendRandomData();
    }
}