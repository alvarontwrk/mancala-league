# -*- coding: utf-8 -*-

import time
import os
import logging
from app import app, league, utilities
from app.constants import BOTS_FOLDER, UPLOAD_FOLDER
from werkzeug import secure_filename
from flask import render_template, request


@app.route('/subirbot')
def subirbot():
    return render_template('upload.html')


@app.route("/")
def index():
    try:
        (exec_date, ranking, matches) = league.get_current_data()
        return render_template('liga.html', tables=[ranking, matches],
                               titles=['na', 'Ranking', 'Partidos'],
                               exec_date=exec_date)
    except FileNotFoundError as ex:
        logging.error(ex)
        return """No existen datos actuales de la liga.
                Ejecuta la competición en /ejecutar"""


@app.route("/ejecutar1234")
def ejecutar():
    if league.run_competition():
        return 'Ejecutando competición... en unos minutos se actualizarán los resultados'
    else:
        return 'Competición en curso, espera a que termine para ejecutar otra.'


@app.route('/lista')
def lista():
    return render_template('lista.html', bot_list=os.listdir(BOTS_FOLDER))


@app.route('/upload', methods=['POST'])
def upload():
    uploaded_files = request.files.getlist("file[]")
    cond1 = len(uploaded_files) == 2
    cond2 = all(utilities.allowed_file(x.filename) for x in uploaded_files)
    cond3 = all(not os.path.exists(os.path.join(UPLOAD_FOLDER, x.filename))
                for x in uploaded_files)
    if not (cond1 and cond2 and cond3):
        return 'Sube únicamente los archivos .h y .cpp de tu bot'
    filenames = []
    for file in uploaded_files:
        filename = secure_filename(file.filename)
        file.save(os.path.join(UPLOAD_FOLDER, filename))
        filenames.append(filename)
    if utilities.compile_bot(filenames):
        return 'Bot compilado con exito.'
    else:
        return 'No se puede compilar el bot'


@app.route('/partido/<bot1>/<bot2>')
def partido(bot1, bot2):
    file1 = os.path.join(BOTS_FOLDER, bot1)
    file2 = os.path.join(BOTS_FOLDER, bot2)
    if os.path.isfile(file1) and os.path.isfile(file2):
        m = league.run_match(file1, file2)
        t = league.create_matches_table([m]).to_html(classes='matches')
        return render_template('liga.html', tables=[t], titles=['na', 'Partido'])
    else:
        return "No existen los bots seleccionados"
