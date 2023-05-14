#include <Wire.h>

#define READ_TRANSACTION_ONLY
//#define WAIT_END_OF_SETUP

class LM75B {
public:
  LM75B(TwoWire& w, uint8_t i2c_addr = 0x48)
    : wire(w), addr(i2c_addr) {}

  ~LM75B() {}

  float read(void) {
    uint8_t rx[2];

#ifndef READ_TRANSACTION_ONLY
    uint8_t tx[1] = { 0x00 };

    wire.beginTransmission(addr);
    wire.write(tx, sizeof(tx));
    wire.endTransmission(false);
#endif

    wire.requestFrom(addr, sizeof(rx));
    for (unsigned int i = 0; i < sizeof(rx); i++) {
      while (!wire.available())
        ;
      rx[i] = wire.read();
    }
    return (rx[0] << 8 | rx[1]) / 256.0;
  }
private:
  TwoWire& wire;
  const uint8_t addr;
};

LM75B sensor(Wire);

void setup() {
  Serial.begin(9600);
  while (!Serial) // <== Added to wait Serial available
    ;
  
  Wire.begin(); // <== Moved after Serial is available

  Serial.println("\r***** Hello, LM75B! *****");

#ifdef WAIT_END_OF_SETUP
  delay(1000);
#endif
}

void loop() {
  Serial.println(sensor.read());
  delay(1000);
}
