# -*- coding: utf-8 -*-

import os
import logging
from app import league, utilities
from app.constants import BOTS_FOLDER, UPLOAD_FOLDER
from werkzeug import secure_filename
from flask import render_template, request, Blueprint
from app.utilities import render_dataframe


bp = Blueprint('bp', __name__)


def alert_page(title, content):
    return render_template('page.html', title=title, content=content)


@bp.route('/subirbot')
def subirbot():
    if league.is_running_competition:
        return alert_page('Error',
                          'Se está ejecutando una competicion, no se pueden subir bots.')
    else:
        return render_template('upload.html')


@bp.route("/")
def index():
    try:
        (exec_date, ranking, matches) = league.get_current_data()
        next_exec = league.get_next_execution()
        ranking_render = render_dataframe(df=ranking, html_id='ranking')
        matches_render = render_dataframe(df=matches, html_id='matches')
        return render_template('liga.html',
                               tables=[ranking_render, matches_render],
                               titles=['na', 'Ranking', 'Partidos'],
                               exec_date=exec_date, next_exec=next_exec,
                               is_running=league.is_running_competition)
    except FileNotFoundError as ex:
        logging.error(ex)
        return alert_page('No hay datos',
                          'No existen datos actuales de la liga.')


@bp.route("/ejecutar1234")
def ejecutar():
    if league.run_competition(block_thread=False):
        return alert_page('Ejecutando competicion',
                          'Ejecutando competición... en unos minutos se actualizarán los resultados')
    else:
        return alert_page('Competicion en curso',
                          'Competición en curso, espera a que termine para ejecutar otra.')


@bp.route('/lista')
def lista():
    return render_template('lista.html', bot_list=os.listdir(BOTS_FOLDER))


@bp.route('/upload', methods=['POST'])
def upload():
    uploaded_files = request.files.getlist("file[]")
    cond1 = len(uploaded_files) == 2
    cond2 = all(utilities.allowed_file(x.filename) for x in uploaded_files)
    cond3 = all(not os.path.exists(os.path.join(UPLOAD_FOLDER, x.filename))
                for x in uploaded_files)
    if not (cond1 and cond2 and cond3):
        return alert_page('Error',
                          'Sube únicamente los archivos .h y .cpp de tu bot')
    filenames = []
    for file in uploaded_files:
        filename = secure_filename(file.filename)
        file.save(os.path.join(UPLOAD_FOLDER, filename))
        filenames.append(filename)
    if not utilities.legal_files(filenames):
        return alert_page('Error',
                          'Se han detectado llamadas al sistema ilegales')
    elif utilities.compile_bot(filenames):
        return alert_page('Completado', 'Bot compilado con exito.')
    else:
        return alert_page('Error', 'No se puede compilar el bot')


@bp.route('/ejecutar_partido/', methods=['POST'])
def ejecutar_partido():
    bot1 = request.form.get('bot1_select')
    bot2 = request.form.get('bot2_select')
    file1 = os.path.join(BOTS_FOLDER, bot1)
    file2 = os.path.join(BOTS_FOLDER, bot2)
    if os.path.isfile(file1) and os.path.isfile(file2):
        if request.form.getlist('ida_y_vuelta'):
            m1 = league.run_match(file1, file2)
            m2 = league.run_match(file2, file1)
            matches = [m1, m2]
        else:
            m = league.run_match(file1, file2)
            matches = [m]
        t = render_dataframe(league.create_matches_table(matches), 'ranking')
        return render_template('liga.html', tables=[t], titles=['na', 'Partido'])
    else:
        return alert_page('Error', 'No existen los bots seleccionados')


@bp.route('/partido/')
def partido():
    if league.is_running_competition:
        return alert_page('Error',
                          'Se está ejecutando una competicion, no se pueden realizar partidos individuales.')
    else:
        return render_template('partido.html', bot_list=os.listdir(BOTS_FOLDER))
