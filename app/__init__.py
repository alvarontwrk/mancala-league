# -*- coding: utf-8 -*-

import os
import time
from threading import Thread
import schedule
from werkzeug.contrib.fixers import ProxyFix
from app.constants import *
from flask import *
from app.league import run_competition


def scheduling():
    while True:
        schedule.run_pending()
        time.sleep(1)


def check_directories():
    if not os.path.isdir(BOTS_FOLDER):
        os.mkdir(BOTS_FOLDER)
        if not os.path.isdir(DATA_FOLDER):
            os.mkdir(DATA_FOLDER)


def start_scheduling():
    for x in range(EXECUTIONS_PER_DAY):
        h = int(x * 24 / EXECUTIONS_PER_DAY)
        t = '{}:00'.format(str(h).zfill(2))
        schedule.every().day.at(t).do(run_competition)
        Thread(target=scheduling).start()


def create_app():
    # App Initialization
    app = Flask(__name__)
    app.config.from_object('config')
    app.wsgi_app = ProxyFix(app.wsgi_app)
    app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
    app.config['ALLOWED_EXTENSIONS'] = set(['h', 'hpp', 'cpp'])
    app.jinja_env.trim_blocks = True
    return app
