import time
import sys
import logging
import random

from argparse import ArgumentParser

from pymavlink import mavutil
from pymavlink.dialects.v20.UNISAT import MAVLink_sensors_message

from . import _log as _root_log

_log = _root_log.getChild("main")


def main(argv):
    parser = ArgumentParser(add_help=True)
    parser.add_argument("mavuri", nargs="?", action="store", type=str, default="udpout:0.0.0.0:12000")
    parser.add_argument("periodms", nargs="?", action="store", type=int, default="500")

    logging.basicConfig(stream=sys.stdout, level=logging.INFO)

    args = parser.parse_args(argv)
    mavuri = args.mavuri
    periodms = args.periodms

    _log.info("Запускаюсь. Отправляю данные на url: %s" % mavuri)
    connection = mavutil.mavlink_connection(mavuri)
    mav = connection.mav

    timestamp = 0
    while True:
        temp = random.uniform(0, 10)/10
        pressure = random.uniform(10, 20)/10
        height = random.uniform(20, 30)/10


        msg = MAVLink_sensors_message(temp, pressure, height, timestamp)
        _log.info("отправил сообщение: %s", msg)
        mav.send(msg)

        time.sleep(periodms/1000)
        timestamp += periodms


if __name__ == "__main__":
    exit(main(sys.argv[1:]))
