# M-THERMO 2.0

This is a very simple firmware for a ESP8266 (WEMOS D1 mini) based thermostat, using the excellent project [Homie for ESP8266](https://github.com/marvinroger/homie-esp8266). The whole firmware was created in less than a day, so don't expect production quality, though I use it without any issues every day on 5 devices in my home.


## Hardware

- WEMOS D1 mini
- SHT21 temperature & humidity sensor
- WEMOS Relay shield
- WEMOS OLED shield
- 220V AC to 5V DC power supply module (not mandatory for the project)

There are no PCB schematics, as its pretty straightforward using the WEMOS shields. The only possible place for improvisation are the pins for the two buttons. They are defined on lines 76 and 77.

### Software

There is a simple Mosca MQTT broker in the "mosca" folder. You can run it by first running `npm install` (to install node modules) and then `node index`. It expects MongoDB installed and running.

### OTA

Edit `update.sh` to replace `deviceid` with the targeted Homie ID. Then run `update.sh` in console. The update script expects a compiled `bin` binary file inside M-Thermo folder (`esp_firmware/M-Thermo/M-Thermo.ino.d1_mini.bin`). You can build & export that file using Arduino IDE: `Sketch -> Export compiled Binary`.


### Shiny video of how I did my thermostats

Because why not?
<a href="http://www.youtube.com/watch?feature=player_embedded&v=8D_6tFGS0JA" target="_blank"><img src="http://img.youtube.com/vi/8D_6tFGS0JA/0.jpg" alt="M-THERMO 2.0 Video" width="240" height="180" border="10" /></a>
