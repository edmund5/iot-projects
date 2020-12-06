#!/usr/bin/python3
import time
import datetime
import socket
import urllib3
import json

# instance
http = urllib3.PoolManager()

# platform: REST
baseUrl = 'https://2019ncov.asia/api'
    
def start_now():

    # don't stop running
    while True:

        IP_address = socket.gethostbyname(socket.gethostname())

        if IP_address == "127.0.0.1":

            print("No Internet Connection")

        else:

            r = http.request('GET', baseUrl + '/cdr')
            d = json.loads(r.data.decode('utf-8'))

            timestamp = d['last_updated']

            # convert timestamp to human readable
            value = datetime.datetime.fromtimestamp(int(timestamp) / 1000)

            # date / time
            print(f"{value:Date: %Y-%m-%d\nTime: %H:%M:%S}")
            wait()

            # confirmed
            print("Total confirmed: " + format(d['results'][0]['confirmed'], ',d'))
            wait()

            # deaths
            print("Total deaths: " + format(d['results'][1]['deaths'], ',d'))
            wait()

            # recovered
            print("Total recovered: " + format(d['results'][2]['recovered'], ',d'))
            wait()

def wait():

    # wait for 5 secs
    time.sleep(5.0)

if __name__ == '__main__':

    start_now()
