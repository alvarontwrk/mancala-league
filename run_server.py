# -*- coding: utf-8 -*-

import logging
from app import app
from app.constants import LOG_FILE


if __name__ == '__main__':
    logging.basicConfig(filename=LOG_FILE, level=logging.DEBUG,
                        format='%(asctime)s %(message)s',
                        datefmt='%m/%d/%Y %I:%M:%S %p')
    app.run(host='0.0.0.0', port=8080, debug=True)
