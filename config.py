import os

basedir = os.path.abspath(os.path.dirname(__file__))
LOG_FILE = os.path.join(basedir, 'log/app_logger.log')
CSRF_ENABLED = True
SECRET_KEY = '1c9db07b9343e8d0fb3bde4f691790b215a1e59cf60a430863b59d8408aee6c2'
DEBUG = True