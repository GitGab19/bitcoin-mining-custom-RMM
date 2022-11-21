# Bitcoin Mining Custom RMM (Remote Monitoring and Management) System
<p align="center">
<img align="center" src="https://media.tenor.com/_mpI-aHxu6gAAAAC/bitcoin-bitcoin-coaster.gif" width="250"> 
</p>

## **Introduction**
<div align="justify">

Repo contains a personal implementation of a custom ***Remote Monitoring and Management*** system for Bitcoin mining, which I developed during a prototyping phase project, with a <ins>single</ins> ASIC miner. Monitoring of the activity is managed through different tools like Docker, Grafana, InfluxDB, Telegraf, Mosquitto, Raspberry PI, ESP32 and various sensors. <br><br>

Repo is divided into ***3 subfolders***:

- <ins>***/miner-RMM***</ins> &nbsp; <img align="top" style="width:3vw" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/docker.webp"> &nbsp;
<img align="top" style="width:4vw" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/grafana.png"> &nbsp;
<img align="top" style="width:3vw" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/telegraf.svg"> &nbsp;
<img align="top" style="width:4vw" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/influxdb.png"> &nbsp; <img align="center" style="width:4vw" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/mosquitto.png"> 
    
    It contains the **core part** of this project, including all the docker containers and their relative configuration files.\
    Every *env* variable used in the initial setup of the containers are placed in the ***.env*** file, such as Grafana initial username and password, InfluxDB admin credentials, etc. <ins>You need</ins> to edit the [.env](https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/miner-RMM/.env) file to setup the credentials/configurations that you prefer.\
    In order to give a general explanation about the behaviour of the services wrapped into [docker-compose.yml](https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/miner-RMM/docker-compose.yml) and the interaction between each other, have a look at the schema below.
    <p align="left">
    <img align="center" style="width:75%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/docker-components.png">
    </p>
    
    *More details about the subfolder contents are explained in a deeper way into its relative [<ins>README</ins>](./miner-RMM/README.md)*.
    
- <ins>***/sensors***</ins> &nbsp; <img align="top" style="width:4vw" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/esp32.png"> &nbsp; <img align="top" style="width:4vw" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/arduino.png">

    It contains *sensors_management.ino*: this is the file that needs to be **flashed** into the [ESP32](https://www.espressif.com/en/products/socs/esp32) in order to let it communicating sensors measurements by **MQTT messages** to the **MQTT broker** (*mosquitto*) that is running into Raspberry PI. <br>
    <ins>Before</ins> flashing it, you need to change some lines of code, such as:
    ``` 
    #define WIFI_SSID       // line 7
    #define WIFI_PASSWORD   // line 8
    ``` 
    ``` 
    #define MQTT_HOST IPAddress(192, 168, 1, 170)   // line 14
    ``` 
    Other specific settings can be customized directly into code, in which there are already comments to better understand it. <br>
    In order to flash the ESP32 with this code, you can do it using the [Arduino IDE](https://www.arduino.cc/en/software), following [this guide](https://medium.com/@pauljoegeorge/setup-arduino-ide-to-flash-a-project-to-esp32-34db014a7e65).<br><br>
    *More details about the subfolder contents are explained in a deeper way into its relative [<ins>README</ins>](./sensors/README.md)*.

- <ins>***/tasmota_custom_firmware***</ins> &nbsp; <img align="top" style="width:7vw" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/tasmota.png"> &nbsp; <img align="top" style="width:2vw" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/tg.png">
  
    It contains my custom [Tasmota](https://tasmota.github.io/docs/) firmware binary, customized by me in order to add the Telegram functionality to the original firmware version. This customization, once firmware is **flashed** into the [Sonoff](https://sonoff.tech/product-document/diy-smart-switches-doc/powr2-doc/), permits to link a personal Telegram bot to the smart switch, and power on/off the miner connected to it, **remotely**, using your smartphone.

    In order to not trusting me and my customized firmware, if you want to enable the above descripted functionality, have a look at https://github.com/benzino77/tasmocompiler for compiling your customized version of the firmware to install into your Sonoff switch. <br> 
    All you have to do is to add the following strings in the ***"Custom parameters"*** section (4) of Tasmocompiler (or directly into *user_config_override.h* file if you want to compile it by hand):
    ```
    #define USE_TELEGRAM 
    #define USE_MQTT_TLS_CA_CERT   
    ``` 
    If you want to read more about compiling tools for Tasmota firmware, you can find it at https://tasmota.github.io/docs/Compile-your-build/. <br>
    **Flashing** the customized Tasmota firmware into Sonoff it's not a trivial operation, pay attention in this phase. Different flashing solutions are available at https://tasmota.github.io/docs/Getting-Started/#hardware-preparation, depending on platform you use to flash your Sonoff. <br>
    In order to create the **Telegram bot** and configure it to talk with your "tasmotized" Sonoff, you can follow [this guide](https://minomodding.blogspot.com/2020/08/tasmota-integrazione-con-telegram.html). <br><br>
    *More details about the subfolder contents are explained in a deeper way into its relative [<ins>README</ins>](./tasmota_custom_firmware/README.md)*.
</div>


## **Functional requirements**
| Functionality | Component used | Role | Status |
| --- | --- | --- |:-:|
| ***Remote switch*** on/off |[Sonoff powr2](https://sonoff.tech/product-document/diy-smart-switches-doc/powr2-doc/) (*tasmotized*) | Smart switch | :white_check_mark: |
| ***Remote access*** to RMM system | [Raspberry PI 4](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/) | **VNC** server | :white_check_mark: |
| Power consumption and data(voltage/current) monitoring |[Sonoff powr2](https://sonoff.tech/product-document/diy-smart-switches-doc/powr2-doc/) (*tasmotized*) | Power meter | :white_check_mark: |
| ***Temp/Hum*** measurement (environmental) | [DHT22](https://lastminuteengineers.com/dht11-dht22-arduino-tutorial/) | Temp/Hum sensor | :white_check_mark: |
| ***Smoke*** detection | [MQ2](https://lastminuteengineers.com/mq2-gas-senser-arduino-tutorial/) | Gas sensor | :white_check_mark: |
| Sensors data **management** | [ESP32](https://www.espressif.com/en/products/socs/esp32) | Sensors control board and **MQTT client** | :white_check_mark: |
| ***Hashrate*** visualization | [*miner_stats.py* script](https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/miner-RMM/python_backend/miner_stats.py) | Socket ***reader*** | :white_check_mark: |


## **System Design**
In order to understand in a better way **how** the RMM system is built, you can have a look at the *system design* that I've chosen to use in my context. <br><br>
<img style="width:75%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/system-design.png">
<br>
## **Conclusion**
I've decided to publish my personal project for anyone who is interested in experimenting with mining, especially for the ones who are willing to try starting with just a <ins>single miner</ins>. 
As illustrated in the *system design*, my solution was built around an *Antminer S19J pro*, but it can be easily adapted for any other miner on the market.<br><br>
Feel free to open issues, comment it or contribute to the project!
