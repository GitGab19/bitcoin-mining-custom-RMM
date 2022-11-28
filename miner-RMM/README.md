# miner-RMM

In this README I want to explain every detail of the contents of this subfolder, in order to help the understanding of the system and the possible troubleshooting.
<p align="left">
    <img align="center" style="width:50%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/docker-components.png">
</p> 

### docker-compose.yml <img align="top" style="width:4%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/docker.webp">
This is the [**core file**](docker-compose.yml) of the entire system, so it's important to look at it first, and understand it well.
As any other *docker-compose*, it contains the set of services that will be virtualized through docker containers, in order to work inside docker. <br>
> **Warning** <br>
> The **main configuration parameters** of the following listed services are present in [.env](.env) file, have a look at it!

In this case (as represented in the image above), the services used in my system are:
- mosquitto <img align="center" style="width:5%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/mosquitto.png">
  
  This is the **MQTT broker**, which collects every MQTT messages that arrives from the sensor board (ESP32) and the Sonoff (power meter) in its local network.
  - it's pre-configured (see the config file [here](mosquitto/mosquitto.conf)) to listen MQTT connections on standard port **1883**.
- telegraf <img align="top" style="width:3%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/telegraf.svg">
  
    This is the **agent** who is in charge of extracting measurements data which arrives to **mosquitto** through MQTT messages (in different MQTT topics) and writing them to database (**influxdb**). 
    - as you can see in its [**config file**](telegraf/telegraf.conf), it's configured to take as input MQTT messages which arrives through 2 topics:
      - "**sensors/#**"
        - they are the sensors measurements sent by the **ESP32 board**
      - "**tele/tasmota/SENSOR**"
        - they are the power measurements sent by the **"tasmotized" Sonoff**
  - in order to write data into *influxdb*, an **admin_token** is pre-configured at line 88 of its config file (have a look [here](telegraf/telegraf.conf))
    - if you want to change it, <ins>**after creating a new one from influxdb admin panel**</ins>, you will need to change this line, inserting your fresh token there
    - the same operation is needed for the "**organizationName**" of influxdb
-  influxdb <img align="top" style="width:5%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/influxdb.png">
  
    This is the **database** which will contain every measurements that will be written by the **telegraf agent**
   - there are two default buckets:
     - "<ins>myMeasurements</ins>": it will contain the **sensors measurements** (temperature, humidity, air quality) + **power measurements** of the Sonoff
      - "<ins>minerStats</ins>": it will contain the **hashrate measurements** that will be read from miner itself (through the automatic execution of [this script](python_backend/miner_stats.py)) 
    - it's pre-configured to be running on its standard port **8086**
   - you can have a look at its structure going to http://localhost:8086 (**after that docker containers are running**, of course)
     - the default username is: <ins>**admin**</ins>
     - the default password is: <ins>**admin_password**</ins>
     - if you want to change the credentials, you can do it changing lines 9-10 of [docker-compose.yml](docker-compose.yml) file
    - if you want to change the default token that telegraf uses to write into db, once logged into influxdb admin panel, you can go to **"Load data" --> "API tokens"** and generate a new one (<ins>REMEMBER</ins> to **change the telegraf configuration** as explained above!)
- grafana <img align="top" style="width:5%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/grafana.png">
  
  This is the service which is in charge of **plotting all data** extracted from influxdb, and **it will be the main interface for the monitoring and management activity**.
  - you can access it going to http://localhost:3000
    - the default username is: **admin**
    - the default password (*if it's the first time that you access Grafana and you didn't change the password*) is: **admin**
  - if during the **setup guide** present [here](../README.md) **you set a IP address different from 192.168.1.170**
    - you have to put your static IP chosen into **ON/OFF button** configuration:
      - click onto ON/OFF title --> **"Edit"**
      - expand the **"Server"** options on the right
      - write the static **IP address of your Raspberry into "Server" field**
  
  <br>

  If you didn't know Grafana before, you can start by reading its [documentation](https://grafana.com/docs).
- miner-stats
  
  This is the service which is in charge of retrieving data about Hashrate from the miner.
  The operation is managed from the relative script called ["miner_stats.py"](python_backend/miner_stats.py).

  - in case of modifications about influxdb admin_token, buckets names or organization name: 
    - you need to modify the relative lines into the above linked script
  - this script will be automatically executes every minute, as you can see in the [cron](../miner-RMM/python_backend/mycron) file
      >**Warning**<br>
      > You have to set the STATIC IP ADDRESS of **your miner** in this cron file, substituting it with the current 192.168.1.192!

<br>
In case you have doubts or any problems about the RMM system, feel free to open issue or DM me!
