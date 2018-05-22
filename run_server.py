# -*- coding: utf-8 -*-
import logging
from app import create_app, check_directories, start_scheduling
from app.constants import LOG_FILE
from app.views import bp


if __name__ == '__main__':
    app = create_app()
    app.register_blueprint(bp)
    check_directories()
    start_scheduling()
    logging.basicConfig(filename=LOG_FILE, level=logging.DEBUG,
                        format='%(asctime)s %(message)s',
                        datefmt='%m/%d/%Y %I:%M:%S %p')
    app.run(host='0.0.0.0', port=8080, debug=True)
