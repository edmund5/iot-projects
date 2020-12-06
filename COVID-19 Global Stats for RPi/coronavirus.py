#!/usr/bin/python3
import time
import datetime
import digitalio
import board
import adafruit_character_lcd.character_lcd as characterlcd
import socket
import urllib3
import json

# instance
http = urllib3.PoolManager()

# lcd size = 16x2
lcd_columns = 16
lcd_rows = 2

# supported RPi v1 - v3B+
lcd_rs = digitalio.DigitalInOut(board.D22)
lcd_en = digitalio.DigitalInOut(board.D17)
lcd_d4 = digitalio.DigitalInOut(board.D25)
lcd_d5 = digitalio.DigitalInOut(board.D24)
lcd_d6 = digitalio.DigitalInOut(board.D23)
lcd_d7 = digitalio.DigitalInOut(board.D18)

# initialise the lcd class
lcd = characterlcd.Character_LCD_Mono(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7, lcd_columns, lcd_rows)

# REST
baseUrl = 'https://2019ncov.asia/api'

def start_now():

    # don't stop running
    while True:

        IP_address = socket.gethostbyname(socket.gethostname())

        if IP_address == "127.0.0.1":

            lcd.message = "No Internet\nConnection"
            wait()

        else:

            lcd.message = "COVID-19 Global\nStats for RPi"
            wait()

            r = http.request('GET', baseUrl + '/cdr')
            d = json.loads(r.data.decode('utf-8'))

            timestamp = d['last_updated']

            # convert timestamp to human readable
            value = datetime.datetime.fromtimestamp(int(timestamp) / 1000)

            # date / time
            lcd.message = "As of " + f"{value:%Y-%m-%d\n%H:%M:%S}" + " (UTC)"
            wait()

            # confirmed
            lcd.message = "Total confirmed" + "\n" + format(d['results'][0]['confirmed'], ',d')
            wait()

            # deaths
            lcd.message = "Total deaths" + "\n" + format(d['results'][1]['deaths'], ',d')
            wait()

            # recovered
            lcd.message = "Total recovered" + "\n" + format(d['results'][2]['recovered'], ',d')
            wait()

            lcd.message = "Stay Home\nSave Lives"
            wait()

            lcd.message = "Thank you\nFrontliners for"
            wait()

            lcd.message = "Keeping us safe\n    <3 <3 <3    "
            wait()

def wait():

    # wait for 5 secs
    time.sleep(5.0)

    # clear display
    lcd.clear()

    # wait for 1 sec
    time.sleep(1.0)

if __name__ == '__main__':

    start_now()
