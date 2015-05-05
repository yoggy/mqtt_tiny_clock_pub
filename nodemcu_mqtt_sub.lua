--
-- nodemcu_mqtt_sub.lua - MQTT subscribe script for NodeMCU
--
-- github:
--     https://github.com/yoggy/mqtt_tiny_clock_pub
--
-- license:
--     Copyright (c) 2015 yoggy <yoggy0@gmail.com>
--     Released under the MIT license
--     http://opensource.org/licenses/mit-license.php
--

-- load configuration
require("config")

-- pin assign : https://github.com/nodemcu/nodemcu-firmware/wiki/nodemcu_api_en#new_gpio_map
-- led_pin = 3 -- GPIO0
led_pin = 4 -- GPIO2

-- setup gpio
gpio.mode(led_pin, gpio.OUTPUT)
gpio.write(led_pin, gpio.LOW)

tmr.delay(1000 * 1000)

-- wifi
print("connect to ssid=" .. wifi_ssid)

wifi.setmode(wifi.STATION)
wifi.sta.config(wifi_ssid, wifi_pass)

for i = 1, 10 do
  gpio.write(led_pin, gpio.LOW)
  tmr.delay(1000 * 1000)
  if wifi.sta.status() == 5 then
    break
  end
  print(i);
  if i == 20 then
    print("wifi connecting failed...rebooting")
    node.restart()
    tmr.delay(5000 * 1000)
  end
  
  print("wifi connection waiting...");
  gpio.write(led_pin, gpio.HIGH)
  tmr.delay(1000 * 1000)
end

print("wifi connected!")
print(wifi.sta.getip())
gpio.write(led_pin, gpio.HIGH)

-- MQTT
-- see also... http://www.nodemcu.com/docs/mqtt-module/
m = mqtt.Client(mqtt_client_id, 60, mqtt_username, mqtt_password)

m:on("offline", function(con) print ("offline...rebooting");  node.restart(); tmr.delay(5000 * 1000) end)

m:on("message", function(conn, topic, data) 
  print(topic .. ":" ) 
  if data ~= nil then
    print(data)
  end
end)

print("mqtt:connect() start")
m:connect(mqtt_host, mqtt_port, 0, function(conn)
  print("connected") 
  m:subscribe(mqtt_subscribe_topic, 0, function(conn)
  end)
end)
