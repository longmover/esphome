An ESPHome external component to allow the reading of various sensor values/states using the X6 interfaces on an old Vaillant boiler.

There are many potential values to read but not all will be supported by all boilers. This one returns:

- Boiler Mode (Text sensor describing the current mode of the boiler)
- Burner Status (Heat or normal)
- Flow Temperature Set (Heating set temperature)
- Flow Temperature Actual
- Return Temperature
- Hot Water Status: Heat or normal
- Hot Water Temperature

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
YAML:
````
external_components:
  - source: github://longmover/esphome
    components: [ vaillantx6 ]
    refresh: 0s # forces a check of this repo on each build

binary_sensor:
  - platform: template
    name: "Hot Water Status"
    id: hot_water
    icon: "mdi:water-boiler"
    device_class: heat
  - platform: template
    name: "Burner Status"
    id: burner_state
    icon: "mdi:fire"
    device_class: heat

sensor:
  - platform: template
    name: "HW Temp"
    id: hot_water_temp
    unit_of_measurement: "°C"
    icon: "mdi:thermometer"
    state_class: measurement
    device_class: temperature
  - platform: template
    name: "Return Temp"
    id: return_temp
    unit_of_measurement: "°C"
    icon: "mdi:thermometer"
    state_class: measurement
    device_class: temperature
  - platform: template
    name: "Flow Temp Set"
    id: flow_temp_set
    unit_of_measurement: "°C"
    icon: "mdi:thermometer"
    state_class: measurement
    device_class: temperature
  - platform: template
    name: "Flow Temp Actual"
    id: flow_temp_actual
    unit_of_measurement: "°C"
    icon: "mdi:thermometer"
    state_class: measurement
    device_class: temperature

text_sensor:
  - platform: template
    name: "Boiler Mode"
    id: boiler_status
    icon: "mdi:fire"

uart:
  id: uart_bus
  tx_pin: GPIO17
  rx_pin: GPIO16
  baud_rate: 9600

vaillantx6:
  id: my_vaillant
  update_interval: 2000ms
  uart_id: uart_bus
  hot_water: hot_water
  hot_water_temp: hot_water_temp
  boiler_status: boiler_status
  flow_temp_set: flow_temp_set
  flow_temp_actual: flow_temp_actual
  return_temp: return_temp
  burner_state: burner_state
````
