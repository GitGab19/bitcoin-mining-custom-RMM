# Bitcoin Mining Custom RMM (Remote Monitoring and Management) System
<p align="center">
<img align="center" src="https://media.tenor.com/_mpI-aHxu6gAAAAC/bitcoin-bitcoin-coaster.gif" width="250"> 
</p>

## **Introduction**
<div align="justify">

Repo contains a personal implementation of a custom ***Remote Monitoring and Management*** system for Bitcoin mining, which I developed during a prototyping phase project, with a <ins>single</ins> ASIC miner. Monitoring of the activity is managed through different tools like Docker, Grafana, InfluxDB, Telegraf, Mosquitto, Raspberry PI, ESP32 and various sensors. 
</div>

## **Functional requirements**
These are the **requirements** that I had for my project: they can be useful in order to have a generic idea of what the RMM system **can do and how it's built**.  
| Functionality | Component used | Role | Status |
| --- | --- | --- |:-:|
| ***Remote switch*** on/off |[Sonoff powr2](https://sonoff.tech/product-document/diy-smart-switches-doc/powr2-doc/) (*tasmotized*) | Smart switch | :white_check_mark: |
| ***Remote access*** to RMM system | [Raspberry PI 4](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/) | **VNC** server | :white_check_mark: |
| ***Power consumption*** and data(voltage/current) monitoring |[Sonoff powr2](https://sonoff.tech/product-document/diy-smart-switches-doc/powr2-doc/) (*tasmotized*) | Power meter | :white_check_mark: |
| ***Temp/Hum*** measurement (environmental) | [DHT22](https://lastminuteengineers.com/dht11-dht22-arduino-tutorial/) | Temp/Hum sensor | :white_check_mark: |
| ***Smoke*** detection | [MQ2](https://lastminuteengineers.com/mq2-gas-senser-arduino-tutorial/) | Gas sensor | :white_check_mark: |
| Sensors data ***management*** | [ESP32](https://www.espressif.com/en/products/socs/esp32) | Sensors control board and **MQTT client** | :white_check_mark: |
| ***Hashrate*** visualization | [*miner_stats.py* script](https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/miner-RMM/python_backend/miner_stats.py) | Socket ***reader*** | :white_check_mark: |

<div align="justify">

## **System Design**
In order to understand in a better way the **RMM system architecture**, you can have a look at the **system design** that I've chosen to use in my context. <br><br>
<img style="width:75%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/system-design.png">

## **Repository structure**
Repo is divided into ***3 subfolders***:

- <ins>***/miner-RMM***</ins> &nbsp; <img align="top" style="width:3%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/docker.webp"> &nbsp;
<img align="top" style="width:5%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/grafana.png"> &nbsp;
<img align="top" style="width:4%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/telegraf.svg"> &nbsp;
<img align="top" style="width:5%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/influxdb.png"> &nbsp; <img align="center" style="width:5%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/mosquitto.png"> <br>
    It contains the **core part** of this project, including all the docker containers and their relative configuration files.\
    Every *env* variable used in the initial setup of the containers are placed in the ***.env*** file, such as Grafana initial username and password, InfluxDB admin credentials, etc. <br>
    > **Warning** <ins>You need</ins> to edit the [.env](https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/miner-RMM/.env) file to setup the default credentials/configurations that you prefer.
    
    In order to give a general explanation about the behaviour of the services wrapped into [docker-compose.yml](https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/miner-RMM/docker-compose.yml) and the interaction between each other, have a look at the schema below.
    <p align="left">
    <img align="center" style="width:75%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/docker-components.png">
    </p>
    
    *More details about the subfolder contents are explained in a deeper way into its relative [<ins>README</ins>](./miner-RMM/README.md)*.
    
- <ins>***/sensors***</ins> &nbsp; <img align="top" style="width:4%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/esp32.png"> &nbsp; <img align="top" style="width:3%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/arduino.png"> <br>
It contains [*sensors_management.ino*](https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/sensors/sensors_management.ino): this is the file that needs to be **flashed** into the [ESP32](https://www.espressif.com/en/products/socs/esp32) in order to let it communicating sensors measurements by **MQTT messages** to the **MQTT broker** (*mosquitto*) that is running into Raspberry PI. <br>
    <span id="esp32-code">

    > **Warning** <ins>Before</ins> flashing it, you need to change some lines of code, such as:
    ``` 
    #define WIFI_SSID "PUT HERE YOUR WIFI SSID"     // line 7
    #define WIFI_PASSWORD "PUT HERE YOUR WIFI PASSWORD"   // line 8
    ``` 
    ``` 
    #define MQTT_HOST IPAddress(192, 168, 1, 170)   // line 14
    ``` 
    Other specific settings can be customized directly into code, in which there are already comments to better understand it. 
    </span><br>
    <span id="esp32-flashing"> 
    In order to **flash the ESP32** with this code, you can do it using the [Arduino IDE](https://www.arduino.cc/en/software), following [this guide](https://medium.com/@pauljoegeorge/setup-arduino-ide-to-flash-a-project-to-esp32-34db014a7e65).
    </span>
- <ins id="tasmota">***/tasmota_custom_firmware***</ins> &nbsp; <img align="top" style="width:8%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/tasmota.png"> &nbsp; <img align="top" style="width:3%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/tg.png"><br>
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


## **Project setup**
First of all, depending on the Bitcoin miner that you chose to buy, pay attention to the specifics about energy requirements of the miner, such as power on wall (kW), input current (Amp) and input voltage (V).\
Once you got these specifics, you have to setup the correct power supply infrastructure, such as power supply cables (oversized) and safety breaker switches.

<ins>After having the energy system correctly managed</ins>, once you obtained the hardware components needed for this project, you can finally start following the **steps to replicate my own RMM system**:



### 1. Raspberry setup
> **Warning**<br>
> In my project I decided to use a Raspberry PI 4, but I'd like to underline that every other device can be good as well, such as a normal notebook or a generic server. **The important thing is that it can be reached someway remotely (through VNC, VPN, etc.)**, so it needs to support one of these methods.

* **Assuming to use a Raspberry PI**, first of all you have to assemble its components (cover, fans, etc.)
* **Flash the SD card** with the standard OS (you can follow [this guide](https://www.raspberrypi.com/documentation/computers/getting-started.html))
* Plug in a monitor, keyboard and mouse to the Raspberry and boot it
* Set a static local IP for your Raspberry PI: <ins>**it's important that it doesn't change**</ins> because both the Sonoff and the sensors board will be setup later in order to find the MQTT broker at this fixed IP address
    > **Warning** the **default IP address that I used in the configuration** of the services that are composing the RMM system is **192.168.1.170**: if it's available in your local network, I really suggest to use this address for your Raspberry PI. In this case you will not need to change anything to make the whole system work perfectly.<br>
    If you set a different from 192.168.1.170, have a look at [this readme](./miner-RMM/README.md) in order to make everything work correctly!
* **Install the software requirements** for the RMM system:
  * Git (guide [here](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git))
  * Docker (guide [here](https://docs.docker.com/get-docker/))
* Clone the repository locally:
    ```
    git clone https://github.com/GitGab19/bitcoin-mining-custom-RMM 
    ```
* Enter the miner-RMM folder:
    ```
    cd miner-RMM 
    ```
* Once docker is running, launch this command from terminal:
    ```
    docker-compose up -d
    ```
* Open a new browser window and go to **localhost:3000**
* You should be redirected to the **Grafana welcome screen**, like this one:

    <img width="75%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/setup/grafana-welcome-screen.png">
    
* Enter **Grafana default credentials** (which are set in [.env](https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/miner-RMM/.env) file):
  * username: **admin**
  * password: **admin**
* **Set the new password** in the following step
* You'll be redirected to the **Grafana home screen**, where you'll find the already created dashboard, called "**ASIC**":

    <img width="75%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/setup/grafana-home-screen.png">
    
* **Open that dashboard**, you'll enter this screen:

    <img width="75%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/setup/asic-dashboard.png">
    
* The **ON/OFF button** on the top-left corner of the dashboard will be able to power on/off your miner (<ins>**once correctly setup with your Sonoff!**</ins>)
  * In order to setup this button to work properly with the Sonoff, you can find more helpful info [here](./miner-RMM/README.md)
* All the **other panels are already setup** in order to show measurements data that will come from the Sonoff power meter, environental sensors, and miner itself! 
> **Warning**<br>
> **In case you chose to set 192.168.1.170** as static local IP address for your Raspberry, everything will be working. In other cases, as said before, go to [this readme](./miner-RMM/README.md) to learn how to fix the configuration of some services. 


### 2. Sonoff setup
 
<img align="right" width="25%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/setup/serial-usb.JPG"> 

* Prepare the **Tasmota firmware** that needs to be flashed into Sonoff:
    * if you <ins>want</ins> to add the **Telegram functionality**, which permits to control the Sonoff switch by sending a simple message, you can:
        * directly download the version **already customized** by me [here](https://github.com/GitGab19/bitcoin-mining-custom-RMM/tree/main/tasmota_custom_firmware);
        * add the functionality and **compile the firmware by your own**, following the advices which I already mentioned above in the [tasmota_custom_firmware section](#tasmota);
    * if you <ins>don't want</ins> to enable the **Telegram functionality**, you can go directly to the official Tasmota guide from [here](https://tasmota.github.io/docs/Getting-Started/) and follow it. 
* In both cases, in order to **flash the firmware into the Sonoff**, you need to buy a Serial-to-USB adapter (you can read more about it [here](https://tasmota.github.io/docs/Getting-Started/#serial-to-usb-adapter)).    

<img align="right" style="width:25%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/setup/sonoff.JPG">

* Once Tasmota custom firmware is **flashed**, your Sonoff is ready to be configured:
  * if it's already connected to your wi-fi router, scan your local network, find its IP address and open a browser that points to it
  * if it's not, do a rapid triple-click onto the unique physical button present on Sonoff, entering its pairing mode
    * at this point you can find the "TASMOTA-xxx" wifi network from your computer, click on it to connect. This will take you to the configuration page of Tasmota, in which you can set the wifi parameters;
  * go to **Configuration->Configure MQTT** and:
    * set the **MQTT host** (broker) to connect with, entering the <ins>**Raspberry local IP address**</ins>;
    * leave all the other fields as they are (make sure the <ins>topic</ins> field is filled with **"tasmota"**);
* Now your Sonoff is completely configured, and you can start using it:
  * if you <ins>decided to add</ins> the **Telegram functionality**, my advice is to connect the Sonoff to a very basic object that can be controlled by a switch, such as a simple lamp, in order to setup and test the Telegram interaction in a safer way.\
      To setup the Telegram bot and configure it with the "tasmotized" Sonoff, you can follow [this guide](https://minomodding.blogspot.com/2020/08/tasmota-integrazione-con-telegram.html).
* If you <ins>didn't want</ins> the **Telegram functionality**, or <ins>after having **tested**</ins> the Telegram functionality, you can directly start plugging the power cables between the wall, Sonoff and your miner.

If you don't know MQTT protocol and want to know more, you can start by reading the Tasmota documentation and follow its relative links: https://tasmota.github.io/docs/MQTT/#topic 
  
    
### 3. ESP32 and sensors setup
<img align="right" style="width:15%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/setup/my-esp32.JPG">

* **Prepare the firmware** that needs to be flashed into ESP32, which is responsible of letting **ESP32 send MQTT messages** containing the sensors measurements:
  * have a look at the [*sensors_management.ino*](https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/sensors/sensors_management.ino) file, and change the lines of code cited [here](#esp32-code), in order to tell to the ESP32 the corrrect credentials of your wifi router and the local IP address of the MQTT broker (**which is the <ins>local IP address that you chose to set to your raspberry PI**</ins>);  
  * if you have similar <ins>but not equal</ins> sensors to the ones specified ([DHT22](https://lastminuteengineers.com/dht11-dht22-arduino-tutorial/) and [MQ2](https://lastminuteengineers.com/mq2-gas-senser-arduino-tutorial/)), you **could have to modify** some other parts, uncommenting or adding some lines of code;
* **Flash the firmware to ESP32**, following [these](#esp32-flashing) advices.
* Insert the **ESP32 into the board** and connect it with the sensors above listed using different jumpers and wires.
You can follow different guides present on Internet, such as https://www.hackster.io/ricky-wijaya/temperature-humidity-and-air-quality-control-3fc819.
* Connect the ESP32 with your computer and verify from the Arduino IDE that everything is good, looking for the debug **messages sent to the serial monitor tool** of the IDE.
* At this point your ESP32 is correctly setup and it's ready to be plugged into the experimental location (as near as possible to the miner) with a generic power adapter. 


## **Conclusion**
I've decided to publish my personal project for anyone who is interested in experimenting with mining, especially for the ones who are willing to try starting with just a <ins>single miner</ins>. 
As illustrated in the *system design*, my solution was built around an *Antminer S19J pro*, but it can be easily adapted for any other miner on the market.<br><br>
Feel free to open issues, comment it or contribute to the project!

</div>
