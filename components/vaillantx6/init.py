import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID, CONF_UART_ID
from esphome.components import uart, binary_sensor, sensor, text_sensor

DEPENDENCIES = ['uart']
AUTO_LOAD = ['binary_sensor', 'sensor', 'text_sensor']

vaillantx6_ns = cg.esphome_ns.namespace('vaillantx6')
VaillantX6Component = vaillantx6_ns.class_(
    'VaillantX6Component', cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(VaillantX6Component),
    cv.GenerateID(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required("hot_water"): cv.use_id(binary_sensor.BinarySensor),
    cv.Required("central_heating_set_temp"): cv.use_id(sensor.Sensor),
    cv.Required("hot_water_temp"): cv.use_id(sensor.Sensor),
    cv.Required("boiler_status"): cv.use_id(text_sensor.TextSensor),
    cv.Required("flow_temp_set"): cv.use_id(sensor.Sensor),
    cv.Required("flow_temp_actual"): cv.use_id(sensor.Sensor),
    cv.Required("return_temp"): cv.use_id(sensor.Sensor),
}).extend(cv.polling_component_schema("2000")).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    cg.add(var.set_hot_water(config["hot_water"]))
    cg.add(var.set_central_heating_set_temp(config["central_heating_set_temp"]))
    cg.add(var.set_hot_water_temp(config["hot_water_temp"]))
    cg.add(var.set_boiler_status(config["boiler_status"]))
    cg.add(var.set_flow_temp_set(config["flow_temp_set"]))
    cg.add(var.set_flow_temp_actual(config["flow_temp_actual"]))
    cg.add(var.set_return_temp(config["return_temp"]))
