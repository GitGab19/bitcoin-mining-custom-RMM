#!/usr/bin/env python3
# Usage:
# cgminer-api-pull-data.py [command] [ip] [port]
# eg. cgminer-api-pull-data.py summary 192.168.1.10 4028
#

import socket
import json
import sys
from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

bucket = "minerStats"
my_token = "admin_token" # insert here the influxdb token 
my_organization = "myOrganization" # insert here your organization name
client = InfluxDBClient(url="http://localhost:8086", token=my_token, org=my_organization)
write_api = client.write_api(write_options=SYNCHRONOUS)

def linesplit(socket):
    buffer = socket.recv(4096).decode()
    done = False
    while not done:
        more = socket.recv(4096).decode()
        if not more:
            done = True
        else:
            bufferdecode = buffer + more
    if buffer:
        return buffer

api_command = sys.argv[1].split('|')

if len(sys.argv) < 3:
    api_ip = '127.0.0.1'
    api_port = 4028
else:
    api_ip = sys.argv[2]
    api_port = sys.argv[3]

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((api_ip, int(api_port)))
if len(api_command) == 2:
    s.send(json.dumps({"command": api_command[0], "parameter": api_command[1]}))
else:
    resp = s.send(json.dumps({"command": api_command[0]}).encode())

response = linesplit(s)
response = response.replace('\x00', '')
response = json.loads(response)
print(response)

# Write operation in InfluxDB
p1 = Point("hashrate").field("ghs", response["SUMMARY"][0]["GHS 5s"])
write_api.write(bucket=bucket, record=p1)
p2 = Point("hashrate_avg").field("ghs", response["SUMMARY"][0]["GHS av"])
write_api.write(bucket=bucket, record=p2)

s.close()