#pragma once
#include "esphome.h"
#include <map>
#include <string>

namespace esphome {
namespace vaillantx6 {

class VaillantX6Component : public PollingComponent, public UARTDevice {
 public:
  VaillantX6Component();

  // Setters to assign sensor pointers from YAML
  void set_hot_water(BinarySensor *sensor);
  void set_central_heating_set_temp(Sensor *sensor);
  void set_hot_water_temp(Sensor *sensor);
  void set_boiler_status(TextSensor *sensor);
  void set_flow_temp_set(Sensor *sensor);
  void set_flow_temp_actual(Sensor *sensor);
  void set_return_temp(Sensor *sensor);

  void setup() override;
  void update() override;

 protected:
  // Sensor pointers
  BinarySensor *hot_water_;
  Sensor *central_heating_set_temp_;
  Sensor *hot_water_temp_;
  TextSensor *boiler_status_;
  Sensor *flow_temp_set_;
  Sensor *flow_temp_actual_;
  Sensor *return_temp_;

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
