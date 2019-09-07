-- 
--   Project name: Your First Lua Script Flash on ESP8266!!!
--   20 #arduSerie - Arduino &ESP8266
--   Flavour I - 
--   Hex File: _20_arduSerie_lua_01.HelloWorld.lua
--   Revision History:
--   20161012:       
--   Description:
--       This program lights on & off a LED on pin 4 (GPIO 2)
--       of your ESP8266 ESP-01 Model
--   MCU:                  Arduino 1.6.12 - @16MHz       http://www.arduino.cc/
--   IC Chip:              ESP8266 -  ESP-01             https://goo.gl/E0Eq4l
--   Connections:
--      See Official youtube channel vids: https://youtu.be/u1xqW6qtFXs
--   Based on:    LUA PROGRAMMING USING ESPLORER IDE TUTORIAL GUIDE
--   Website:     http://www.14core.com/programming-your-esp8266-using-esplorer-guide/
--   Code by:     http://www.14core.com/
--   
--   This program is free software: you can redistribute it and/or modify
--   it under the terms of the GNU General Public License v3 as published by
--   the Free Software Foundation
lighton=0
pin=4
gpio.mode(pin,gpio.OUTPUT)            -- Assign GPIO to Output
tmr.alarm(1,500,1,function()
    if lighton==0 then
        lighton=1
        gpio.write(pin,gpio.HIGH)     -- Assign GPIO On
    else
        lighton=0
         gpio.write(pin,gpio.LOW)     -- Assign GPIO off
    end
end)
