# miner-RMM

In this README I want to explain every detail of the contents of this subfolder, in order to help the understanding of the system and the possible troubleshooting.
<p align="left">
    <img align="center" style="width:50%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/docker-components.png">
</p> 

### docker-compose.yml <img align="top" style="width:4%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/docker.webp">
This is the [**core file**](docker-compose.yml) of the entire system, so it's important to look at it first, and understand it well.
As any other *docker-compose*, it contains the set of services that will be virtualized through docker containers, in order to work inside docker. <br>
In this case (as represented in the image above), the services used in my system are:
- mosquitto <img align="center" style="width:5%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/mosquitto.png">
  - this is the MQTT broker, which collects every MQTT messages that arrives from the sensor board (ESP32) and the Sonoff (power meter) in its local network
  - it's pre-configured (see the config file [here](mosquitto/mosquitto.conf)) to listen MQTT connections on standard port 1883.
- telegraf <img align="top" style="width:3%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/telegraf.svg">
  - this is the agent who is in charge of extracting measurements data which arrives to **mosquitto** through MQTT messages (in different MQTT topics) and writing them to database (**influxdb**). 
  - as you can see in its [**config file**](telegraf/telegraf.conf), it's configured to take as input MQTT messages which arrives through 2 topics:
    - "**sensors/#**"
      - they are the sensors measurements sent by the **ESP32 board**
    - "**tele/tasmota/SENSOR**"
      - they are the power measurements sent by the **"tasmotized" Sonoff**
- influxdb <img align="top" style="width:5%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/influxdb.png">
- grafana <img align="top" style="width:5%" src="https://github.com/GitGab19/bitcoin-mining-custom-RMM/blob/main/docs/images/logos/grafana.png">
- miner-stats
