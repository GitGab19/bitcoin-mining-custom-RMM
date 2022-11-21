# Bitcoin Mining Custom RMM (Remote Monitoring and Management) system
<p align="center">
<img align="center" src="https://media.tenor.com/_mpI-aHxu6gAAAAC/bitcoin-bitcoin-coaster.gif" width="250"> 
</p>

## **Introduction**

Repo contains a personal implementation of a custom ***Remote Monitoring and Management*** system for Bitcoin mining, which I developed during a prototyping phase project, with a single ASIC miner. <br> Monitoring of the activity is managed through different tools like Docker, Grafana, InfluxDB, Telegraf, Mosquitto, Raspberry PI, ESP32 and various sensors.

Repo is divided into 3 subfolders:

- ***/miner-RMM*** &nbsp; <img align="center" style="width:50px" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/docker.webp"> &nbsp;
<img align="center" style="width:70px" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/grafana.png"> &nbsp;
<img align="center" style="width:55px" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/telegraf.svg"> &nbsp;
<img align="center" style="width:70px" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/influxdb.png"> &nbsp; <img align="center" style="width:70px" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/mosquitto.png">

    It contains the **core part** of the project, including all the docker containers and their relative configuration files.

    Every *env* variable used in the initial setup of the containers are placed in the ***.env*** file, such as grafana initial username and password, influxDB admin credentials, etc. **You need to edit this file** to set the credentials/configurations that you prefer.

    In order to give an abstract explanation about how the components that I chose for my RMM system work and communicate to each other, I've realized this conceptual schema.
    
    <p align="center">
    <img align="center" style="width:50%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/docker-components.png">
    </p>

    <br><br>

    ***More details about the subfolder contents are explained in a deeper way into its relative [README](./miner-RMM/README.md)***.
<br>
    
- ***/sensors*** &nbsp; <img align="center" style="width:90px" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/esp32.png"> &nbsp; <img align="center" style="width:60px" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/arduino.png">
  
    It contains *sensors_management.ino*: this is the file that needs to be **flashed** into the <a href="https://www.espressif.com/en/products/socs/esp32">ESP32</a> in order to let it communicating sensors measurements by **MQTT** messages to the MQTT broker (*mosquitto*) that will be installed into Raspberry PI. <br>
    Before flashing it, you need to change some lines of code, such as:
    ``` 
    #define WIFI_SSID       // line 7
    #define WIFI_PASSWORD   // line 8
    ``` 
    ``` 
    #define MQTT_HOST IPAddress(192, 168, 1, 170)   // line 14
    ``` 
    Other specific settings can be customized directly into code, in which there are already comments to better understand it. <br><br>
    In order to flash the ESP32 with this code, you can do it using the Arduino IDE (https://www.arduino.cc/en/software) following <a href="https://medium.com/@pauljoegeorge/setup-arduino-ide-to-flash-a-project-to-esp32-34db014a7e65">this guide</a>.<br><br>
    ***More details about the subfolder contents are explained in a deeper way into its relative [README](./sensors/README.md)***.
<br>

- ***/tasmota_custom_firmware*** &nbsp; <img align="center" style="width:100px" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/tasmota.png"> &nbsp; <img align="center" style="width:80px" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/telegram.png">
  
    It contains my custom <a href="https://tasmota.github.io/docs/">Tasmota</a> firmware binary, customized by me in order to add the Telegram functionality to the original firmware version. This customization, once firmware is **flashed** into the <a href="https://sonoff.tech/product-document/diy-smart-switches-doc/powr2-doc/">Sonoff</a>, permits to link a personal Telegram bot to the smart switch, and power on/off the miner connected to it, **remotely**, using your smartphone.

    In order to not trusting me and my customized firmware, if you want to enable the above descripted functionality, have a look at https://github.com/benzino77/tasmocompiler for compiling your customized version of the firmware to install into your Sonoff switch. <br> 
    All you have to do is to add the following strings in the ***"Custom parameters"*** section (4) of Tasmocompiler (or directly into *user_config_override.h* file if you want to compile it by hand):
    ```
    #define USE_TELEGRAM 
    #define USE_MQTT_TLS_CA_CERT   
    ``` 
    If you want to read more about compiling tools for Tasmota firmware, you can find it at https://tasmota.github.io/docs/Compile-your-build/. <br>
    **Flashing** the customized Tasmota firmware into Sonoff it's not a trivial operation, pay attention in this phase. Different flashing solutions are available at https://tasmota.github.io/docs/Getting-Started/#hardware-preparation, depending on platform you use to flash your Sonoff. <br>
    In order to create the **Telegram bot** and configure it to talk with your "tasmotized" Sonoff, you can follow <a href="https://minomodding.blogspot.com/2020/08/tasmota-integrazione-con-telegram.html">this guide</a>. <br><br>
    ***More details about the subfolder contents are explained in a deeper way into its relative [README](./tasmota_custom_firmware/README.md)***.
<br>

## **System Design**
In order to clarify in a better way how the RMM system is built, you can have a look at the system design that I've chosen to define.  <br><br>
<img src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/system-design.png">
