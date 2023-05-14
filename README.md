# LM75B_simple
Just a very simple code to check I²C operation using an [LM75B](https://www.nxp.com/products/sensors/ic-digital-temperature-sensors/digital-temperature-sensor-and-thermal-watchdog:LM75B) compatible temperature sensor. 

## What is this? 
I²C operation problem found on an Arduino platform. A sample code has been made to reproduce the problems.  
To operate the I²C, a temperature sensor: LM75B is used as a target.  


### Code
[`LM75B_simple.ino`](https://github.com/teddokano/LM75B_simple_Arduino/blob/main/LM75B_simple.ino) is the code for reproducing the problem.  


### Problems
There are 3 problems on that code. 
1. `Serial.println()` message in `setup()` function does not appear on the termial. <br/>Result → [Picture 1](https://github.com/teddokano/LM75B_simple_Arduino/blob/main/problem1.png)  
1. I2C behavior is not consistent to a reference platform: Uno-R3. It seems read-transaction must have leading write-transaction. <br/>Enabling an option in the code:`READ_TRANSACTION_ONLY` can reprodice the problem <br/>Result → [picture 2](https://github.com/teddokano/LM75B_simple_Arduino/blob/main/difference_by_READ_TRANSACTION_ONLY.png)
1. `delay(1000)` function call in `setup()` function affects I²C behavior. <br/>Enabling `WAIT_END_OF_SETUP` for reproducing <br/>Result → [Picture 3](https://github.com/teddokano/LM75B_simple_Arduino/blob/main/enabling_WAIT_END_OF_SETUP.png)

### Fixed version
After poising the code above, I could figure out to fix the problem.  
The fixed version code is available as [`fixed_LM75B_simple.ino`](https://github.com/teddokano/LM75B_simple_Arduino/blob/main/fixed_LM75B_simple.ino). 

In the code with problem, the `setup()` function was looked like this.  
```cpp
void setup() {
    Wire.begin();
    Serial.begin(9600);
    Serial.println("\r***** Hello, LM75B! *****");
    ...
}
```

The `setup()` function was modified to wait the `Serial` available and call `Wire.begin()` after that.  
With this modification, I²C is made working fine. 
```cpp
void setup() {
  Serial.begin(9600);
  while (!Serial)    // <== Added to wait Serial available
    ;
  
  Wire.begin();      // <== Moved after Serial is available

  Serial.println("\r***** Hello, LM75B! *****");
  ...
}
```

