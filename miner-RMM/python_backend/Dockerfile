FROM ubuntu:latest

RUN apt-get update && apt-get -y install cron
RUN apt-get -y install python3 python3-pip
RUN apt-get install -y influxdb-client
RUN pip install influxdb_client
# Copy hello-cron file to the cron.d directory
COPY mycron /etc/cron.d/mycron
 
# Give execution rights on the cron job
RUN chmod 0644 /etc/cron.d/mycron

# Apply cron job
RUN crontab /etc/cron.d/mycron
 
# Create the log file to be able to run tail
RUN touch /var/log/cron.log
 
# Run the command on container startup
CMD cron && tail -f /var/log/cron.log