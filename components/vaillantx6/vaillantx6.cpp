#include "vaillantx6.h"
#include "esphome/core/log.h"

namespace esphome {
namespace vaillantx6 {

static const char *TAG = "vaillantx6.component";

void VaillantX6Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up VaillantX6Component...");
  // No additional setup is required.
}

void VaillantX6Component::update() {
  ESP_LOGD(TAG, "Updating VaillantX6Component...");

  if (hot_water_) {
    int val = getParm(hw, sizeof(hw));
    ESP_LOGD(TAG, "Hot Water Status raw value: %d", val);
    if (val >= 0)
      hot_water_->publish_state(val == 17);
    else
      hot_water_->publish_state(false);
  }
  
  if (central_heating_set_temp_) {
    int val = getParm(chSetTemp, sizeof(chSetTemp));
    ESP_LOGD(TAG, "CH Set Temp raw value: %d", val);
    if (val >= 0)
      central_heating_set_temp_->publish_state(val);
  }
  
  if (hot_water_temp_) {
    int val = getParm(hwTemp, sizeof(hwTemp));
    ESP_LOGD(TAG, "HW Temp raw value: %d", val);
    if (val >= 0)
      hot_water_temp_->publish_state(val);
  }
  
  if (boiler_status_) {
    int val = getParm(boiler_status_command, sizeof(boiler_status_command));
    ESP_LOGD(TAG, "Boiler Status raw value: %d", val);
    if (val >= 0) {
      std::string status_str = getBoilerStatusString(val);
      ESP_LOGD(TAG, "Boiler status string: %s", status_str.c_str());
      boiler_status_->publish_state(status_str.c_str());
    }
  }
  
  if (flow_temp_set_) {
    int val = getParm(flowTempSet, sizeof(flowTempSet));
    ESP_LOGD(TAG, "Flow Temp Set raw value: %d", val);
    if (val >= 0)
      flow_temp_set_->publish_state(val);
  }
  
  if (flow_temp_actual_) {
    int val = getParm(flowTempActual, sizeof(flowTempActual));
    ESP_LOGD(TAG, "Flow Temp Actual raw value: %d", val);
    if (val >= 0)
      flow_temp_actual_->publish_state(val);
  }
  
  if (return_temp_) {
    int val = getParm(returnTemp, sizeof(returnTemp));
    ESP_LOGD(TAG, "Return Temp raw value: %d", val);
    if (val >= 0)
      return_temp_->publish_state(val);
  }
}

void VaillantX6Component::dump_config() {
  ESP_LOGCONFIG(TAG, "VaillantX6 Component:");
  ESP_LOGCONFIG(TAG, "  Hot Water Sensor: %s", (hot_water_ != nullptr ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  CH Set Temp Sensor: %s", (central_heating_set_temp_ != nullptr ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  HW Temp Sensor: %s", (hot_water_temp_ != nullptr ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  Boiler Status Sensor: %s", (boiler_status_ != nullptr ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  Flow Temp Set Sensor: %s", (flow_temp_set_ != nullptr ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  Flow Temp Actual Sensor: %s", (flow_temp_actual_ != nullptr ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  Return Temp Sensor: %s", (return_temp_ != nullptr ? "configured" : "not configured"));
}

int VaillantX6Component::getParm(uint8_t *cmd, int lcmd) {
  write_array(cmd, lcmd);
  unsigned long start_time = millis();
  while (available() < 4 && (millis() - start_time < 500)) {
    delay(1);
  }
  if (available() >= 4) {
    int len = read();
    int flag = read();
    int val = read() * 16 + (read() >> 4);
    while (available()) {
      read();
    }
    return val;
  } else {
    ESP_LOGW(TAG, "Timeout waiting for UART response");
    return -1;
  }
}

std::string VaillantX6Component::getBoilerStatusString(int status) {
  std::map<int, std::string> boiler_status_map = {
      {17, "Pump Running"},
      {33, "Pump Running - Pre Ignition"},
      {49, "Ignition Sequence"},
      {65, "Burner Ignited"},
      {81, "Burner Shutdown"},
      {97, "Heating Fan Overrun"},
      {113, "Pump Overrun"},
      {129, "Anti Cycling Mode"},
      {145, "Hot Water Demand?"},
      {161, "Pump and Fan Running (HW)"},
      {177, "Ignition Sequence (HW)"},
      {193, "State Undefined"},
      {209, "State Undefined"},
      {225, "Burner Ignited (HW)"},
      {241, "Burner Shutdown (HW)"},
      {257, "Fan Overrun (HW)"},
      {272, "Pump Overrun (HW)"},
      {480, "No Heating Demand"}
  };
  if (boiler_status_map.find(status) != boiler_status_map.end())
    return boiler_status_map[status];
  else
    return "Unknown State";
}

void VaillantX6Component::set_hot_water(binary_sensor::BinarySensor *sensor) {
  hot_water_ = sensor;
}

void VaillantX6Component::set_central_heating_set_temp(sensor::Sensor *sensor) {
  central_heating_set_temp_ = sensor;
}

void VaillantX6Component::set_hot_water_temp(sensor::Sensor *sensor) {
  hot_water_temp_ = sensor;
}

void VaillantX6Component::set_boiler_status(text_sensor::TextSensor *sensor) {
  boiler_status_ = sensor;
}

void VaillantX6Component::set_flow_temp_set(sensor::Sensor *sensor) {
  flow_temp_set_ = sensor;
}

void VaillantX6Component::set_flow_temp_actual(sensor::Sensor *sensor) {
  flow_temp_actual_ = sensor;
}

void VaillantX6Component::set_return_temp(sensor::Sensor *sensor) {
  return_temp_ = sensor;
}

}  // namespace vaillantx6
}  // namespace esphome