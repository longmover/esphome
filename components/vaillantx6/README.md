An ESPHome external component to allow the reading of various sensor values/states using the X6 interfaces on an old Vaillant boiler.

There are many potential values to read but not all will be supported by all boilers. This one returns:

Boiler Mode: Text sensor describing the current mode of the boiler
Burner Status: Heat or normal
Flow Temperature Set: Heating set temperature
Flow Temperature Actual
Return Temperature
Hot Water Status: Heat or normal
Hot Water Temperature

You will need to make up an interface cable using an RJ11 connector as follows:

```
 Heater Board from top       ESP32
    6P4C RJ connector
    
       +---------+
    6  ---       |
GND 5  ---       +--+        GND
TXD 4  ---          |        GPIO17
RXD 3  ---          |        GPIO16
    2  ---       +--+
    1  ---       |
       +---------+

(Credit: martin3000: https://github.com/martin3000/ESPhome)
```
See the example YAML to get it set up in ESPHome
