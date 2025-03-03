#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

#include <map>
#include <string>

namespace esphome {
namespace vaillantx6 {

class VaillantX6Component : public PollingComponent, public uart::UARTDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;

  // Setters for sensor pointers from YAML
  void set_hot_water(binary_sensor::BinarySensor *sensor);
  void set_central_heating_set_temp(sensor::Sensor *sensor);
  void set_hot_water_temp(sensor::Sensor *sensor);
  void set_boiler_status(text_sensor::TextSensor *sensor);
  void set_flow_temp_set(sensor::Sensor *sensor);
  void set_flow_temp_actual(sensor::Sensor *sensor);
  void set_return_temp(sensor::Sensor *sensor);

  // Additional update functions for new read commands
  void update_burner_modulation();
  void update_ch_pump_speed();
  void update_gas_consumption();
  void update_boiler_pressure();
  void update_operating_hours();

 protected:
  // Pointers for existing sensors
  binary_sensor::BinarySensor *hot_water_{nullptr};
  sensor::Sensor *central_heating_set_temp_{nullptr};
  sensor::Sensor *hot_water_temp_{nullptr};
  text_sensor::TextSensor *boiler_status_{nullptr};
  sensor::Sensor *flow_temp_set_{nullptr};
  sensor::Sensor *flow_temp_actual_{nullptr};
  sensor::Sensor *return_temp_{nullptr};

  // Original command arrays (read commands)
  uint8_t hw[7] = {0x07, 0x00, 0x00, 0x00, 0x58, 0x01, 0x51};            // Hot Water Status
  uint8_t chSetTemp[7] = {0x07, 0x00, 0x00, 0x00, 0x19, 0x02, 0xD0};       // CH Set Temp
  uint8_t hwTemp[7] = {0x07, 0x00, 0x00, 0x00, 0x16, 0x03, 0xCF};          // Hot Water Temp
  uint8_t boiler_status_command[7] = {0x07, 0x00, 0x00, 0x00, 0xAB, 0x01, 0xAE}; // Boiler Status
  uint8_t flowTempSet[7] = {0x07, 0x00, 0x00, 0x00, 0x19, 0x00, 0xD2};      // Flow Temp Set
  uint8_t flowTempActual[7] = {0x07, 0x00, 0x00, 0x00, 0x18, 0x00, 0xD0};   // Flow Temp Actual
  uint8_t returnTemp[7] = {0x07, 0x00, 0x00, 0x00, 0x98, 0x00, 0xC9};       // Return Temp

  // Additional read command arrays (example values)
  uint8_t burner_modulation[7] = {0x07, 0x00, 0x00, 0x00, 0x16, 0x04, 0xD1};  // Burner Modulation
  uint8_t ch_pump_speed[7] = {0x07, 0x00, 0x00, 0x00, 0x15, 0x01, 0xD5};      // CH Pump Speed
  uint8_t gas_consumption[7] = {0x07, 0x00, 0x00, 0x00, 0x20, 0x01, 0xA0};      // Gas Consumption

  uint8_t boiler_pressure[7] = {0x07, 0x00, 0x00, 0x00, 0x55, 0x01, 0xD3};     // Boiler Pressure (example)
  uint8_t operating_hours[7] = {0x07, 0x00, 0x00, 0x00, 0x60, 0x01, 0xF0};       // Operating Hours (example)

  // Helper function to send a command and read a parameter
  int getParm(uint8_t *cmd, int lcmd);
  std::string getBoilerStatusString(int status);
};

}  // namespace vaillantx6
}  // namespace esphome
