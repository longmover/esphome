#include "vaillantx6.h"
#include "esphome/core/log.h"

namespace esphome {
namespace vaillantx6 {

static const char *TAG = "vaillantx6.component";

void VaillantX6Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up VaillantX6Component (non-blocking mode)...");
  current_command_ = CommandIndex::HOT_WATER;
  command_start_time_ = millis();
}

void VaillantX6Component::update() {
  // Non-blocking state machine for polling commands
  if (available() < 4) {
    // If this is the first call for the current command, send it:
    if (command_start_time_ == 0) {
      command_start_time_ = millis();
      switch (current_command_) {
        case CommandIndex::HOT_WATER:
          write_array(hw, sizeof(hw));
          break;
        case CommandIndex::CH_SET_TEMP:
          write_array(chSetTemp, sizeof(chSetTemp));
          break;
        case CommandIndex::HW_TEMP:
          write_array(hwTemp, sizeof(hwTemp));
          break;
        case CommandIndex::BOILER_STATUS:
          write_array(boiler_status_command, sizeof(boiler_status_command));
          break;
        case CommandIndex::FLOW_TEMP_SET:
          write_array(flowTempSet, sizeof(flowTempSet));
          break;
        case CommandIndex::FLOW_TEMP_ACTUAL:
          write_array(flowTempActual, sizeof(flowTempActual));
          break;
        case CommandIndex::RETURN_TEMP:
          write_array(returnTemp, sizeof(returnTemp));
          break;
        default:
          break;
      }
    }
    // Check if response is available or timeout elapsed:
    if (available() >= 4) {
      int len = read();
      int flag = read();
      int value = read() * 16 + (read() >> 4);
      // Clear any extra bytes.
      while (available()) { read(); }
      process_response(current_command_, value);
      // Advance to next command.
      current_command_ = static_cast<CommandIndex>( (static_cast<int>(current_command_) + 1) % 7 );
      command_start_time_ = 0;
    } else if (millis() - command_start_time_ > COMMAND_TIMEOUT_) {
      // Timeout: process as error (-1) and advance.
      process_response(current_command_, -1);
      current_command_ = static_cast<CommandIndex>( (static_cast<int>(current_command_) + 1) % 7 );
      command_start_time_ = 0;
    }
  }
}

void VaillantX6Component::process_response(CommandIndex cmd_idx, int value) {
  ESP_LOGD(TAG, "Command %d got value: %d", static_cast<int>(cmd_idx), value);
  switch (cmd_idx) {
    case CommandIndex::HOT_WATER:
      if (hot_water_) {
        hot_water_->publish_state(value == 17);
      }
      break;
    case CommandIndex::CH_SET_TEMP:
      if (central_heating_set_temp_ && value >= 0) {
        central_heating_set_temp_->publish_state(value);
      }
      break;
    case CommandIndex::HW_TEMP:
      if (hot_water_temp_ && value >= 0) {
        hot_water_temp_->publish_state(value);
      }
      break;
    case CommandIndex::BOILER_STATUS:
      if (boiler_status_ && value >= 0) {
        std::string status_str = getBoilerStatusString(value);
        boiler_status_->publish_state(status_str.c_str());
      }
      break;
    case CommandIndex::FLOW_TEMP_SET:
      if (flow_temp_set_ && value >= 0) {
        flow_temp_set_->publish_state(value);
      }
      break;
    case CommandIndex::FLOW_TEMP_ACTUAL:
      if (flow_temp_actual_ && value >= 0) {
        flow_temp_actual_->publish_state(value);
      }
      break;
    case CommandIndex::RETURN_TEMP:
      if (return_temp_ && value >= 0) {
        return_temp_->publish_state(value);
      }
      break;
    default:
      break;
  }
}

int VaillantX6Component::getParm(uint8_t *cmd, int lcmd) {
  // This helper is retained for backward compatibility but is not used
  // in the non-blocking state machine.
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

void VaillantX6Component::dump_config() {
  ESP_LOGCONFIG(TAG, "VaillantX6 Component:");
  ESP_LOGCONFIG(TAG, "  Hot Water Sensor: %s", (hot_water_ ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  CH Set Temp Sensor: %s", (central_heating_set_temp_ ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  HW Temp Sensor: %s", (hot_water_temp_ ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  Boiler Status Sensor: %s", (boiler_status_ ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  Flow Temp Set Sensor: %s", (flow_temp_set_ ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  Flow Temp Actual Sensor: %s", (flow_temp_actual_ ? "configured" : "not configured"));
  ESP_LOGCONFIG(TAG, "  Return Temp Sensor: %s", (return_temp_ ? "configured" : "not configured"));
}

// Setters for sensor pointers:
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
