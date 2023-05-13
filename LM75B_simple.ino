#include <Wire.h>

#define WRITE_THEN_READ

class LM75B {
public:
  LM75B(TwoWire& w, uint8_t i2c_addr = 0x48)
    : wire(w), addr(i2c_addr) {}

  ~LM75B() {}

  float read(void) {
    uint8_t tx[1] = { 0x00 };
    uint8_t rx[2];
    
#ifdef WRITE_THEN_READ
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
  Wire.begin();
  Serial.begin(9600);
  Serial.println("\r***** Hello, LM75B! *****");
}

void loop() {
  Serial.println(sensor.read());
  sensor.read();
  delay(1000);
}
