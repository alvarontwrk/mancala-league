# -*- coding: utf-8 -*-

from werkzeug.contrib.fixers import ProxyFix
from app.constants import *
from flask import *

# App Initialization
app = Flask(__name__)
app.config.from_object('config')
app.wsgi_app = ProxyFix(app.wsgi_app)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER
app.config['ALLOWED_EXTENSIONS'] = set(['h', 'hpp', 'cpp'])

# Jinja2 Setup
app.jinja_env.trim_blocks = True


from app import views
