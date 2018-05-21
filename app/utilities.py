import os
import subprocess
import logging
from app import app
from flask import render_template
from app.constants import UPLOAD_FOLDER, MAKE_COMMAND, BOTS_FOLDER


def extract_numbers(string):
    """Returns all digits of a string"""
    return [int(s) for s in string.split() if s.isdigit()]


def allowed_file(filename):
    split = filename.rsplit('.', 1)
    return len(split) > 1 and filename.rsplit('.', 1)[1] in app.config['ALLOWED_EXTENSIONS']


def fill_template(template, output_folder, **kwargs):
    content = render_template(template, **kwargs)
    path = os.path.join(output_folder, template)
    open(path, 'w').write(content)


def compile_bot(filenames):
    if len(filenames) != 2:
        return False
    result = True
    bot_name = os.path.splitext(filenames[0])[0]
    fill_template('main.cpp', UPLOAD_FOLDER, bot=bot_name)
    fill_template('makefile', UPLOAD_FOLDER, bot=bot_name)
    try:
        subprocess.check_output(MAKE_COMMAND, shell=True).decode('utf-8')
        # Move executable to Bots folder.
        os.rename(os.path.join(UPLOAD_FOLDER, bot_name),
                  os.path.join(BOTS_FOLDER, bot_name))
    except subprocess.CalledProcessError as e:
        logging.error(e)
        result = False
    finally:
        # Remove c++ files
        path1 = os.path.join(UPLOAD_FOLDER, filenames[0])
        path2 = os.path.join(UPLOAD_FOLDER, filenames[1])
        os.remove(path1)
        os.remove(path2)
    return result
