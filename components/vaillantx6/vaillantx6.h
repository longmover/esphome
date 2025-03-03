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

class VaillantX6Component : public PollingComponent, public UARTDevice {
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

 protected:
  binary_sensor::BinarySensor *hot_water_{nullptr};
  sensor::Sensor *central_heating_set_temp_{nullptr};
  sensor::Sensor *hot_water_temp_{nullptr};
  text_sensor::TextSensor *boiler_status_{nullptr};
  sensor::Sensor *flow_temp_set_{nullptr};
  sensor::Sensor *flow_temp_actual_{nullptr};
  sensor::Sensor *return_temp_{nullptr};

  // Command arrays for UART communication
  byte hw[7] = {0x07, 0x00, 0x00, 0x00, 0x58, 0x01, 0x51};
  byte chSetTemp[7] = {0x07, 0x00, 0x00, 0x00, 0x19, 0x02, 0xD0};
  byte hwTemp[7] = {0x07, 0x00, 0x00, 0x00, 0x16, 0x03, 0xCF};
  byte boiler_status_command[7] = {0x07, 0x00, 0x00, 0x00, 0xAB, 0x01, 0xAE};
  byte flowTempSet[7] = {0x07, 0x00, 0x00, 0x00, 0x19, 0x00, 0xD2};
  byte flowTempActual[7] = {0x07, 0x00, 0x00, 0x00, 0x18, 0x00, 0xD0};
  byte returnTemp[7] = {0x07, 0x00, 0x00, 0x00, 0x98, 0x00, 0xC9};

  int getParm(byte *cmd, int lcmd);
  std::string getBoilerStatusString(int status);
};

}  // namespace vaillantx6
}  // namespace esphome
