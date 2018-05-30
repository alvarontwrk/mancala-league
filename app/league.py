import subprocess
from multiprocessing import Pool
import os
import glob
import logging
import time
from threading import Thread
from functools import reduce
from collections import namedtuple
import pandas as pd
from app.utilities import extract_numbers
from app.constants import *
from datetime import datetime, timedelta


is_running_competition = False


def process_output(p1, p2, output):
    """ Return a MatchData object with the match information """
    lines = output.split('\n')
    digit = -1
    for line in lines:
        timeout_error = line.endswith('proporcionar una acción.')
        connection_error = line.startswith('No hay comunicación')
        if timeout_error or connection_error:
            digit = extract_numbers(line)[0]
            break
    if digit == 1:
        return [p1, p2, 0, 48, 0, 0, True, False]
    elif digit == 2:
        return [p1, p2, 48, 0, 0, 0, False, True]
    else:
        digits = [extract_numbers(line) for line in lines[-7:-1]]
        return [p1, p2, digits[0][1], digits[2][1],
                digits[1][1], digits[3][1], False, False]


def run_match(p1, p2):
    """
    Run a match between p1 and p2.
    Returns a MatchData with the relevant data of the match.
    """
    p1_name = os.path.basename(p1)
    p2_name = os.path.basename(p2)
    logging.info('{} vs {}'.format(p1_name, p2_name))
    command = MANCALA_COMMAND.format(p1, p2)
    res = subprocess.check_output(command, shell=True).decode('utf-8')
    match_data = process_output(p1_name, p2_name, res)
    return match_data


def create_matches_table(content):
    """ Returns a DataFrame based on matches data. """
    cols = ['Player 1', 'Player 2', 'Points P1',
            'Points P2', 'Time(ms) P1', 'Time(ms) P2',
            'Timeouts P1', 'Timeouts P2']
    return pd.DataFrame(content, columns=cols)


def create_ranking_table(table):
    """ Convert a matches table into a DataFrame with the ranking of bots
    based on wins and total points.
    """
    tables = []
    for a, b in [(1, 2), (2, 1)]:
        p1_col = 'Player {}'.format(a)
        p2_col = 'Player {}'.format(b)
        p1_points_col = 'Points P{}'.format(a)
        p2_points_col = 'Points P{}'.format(b)
        p1_time_col = 'Time(ms) P{}'.format(a)
        p2_time_col = 'Time(ms) P{}'.format(b)
        p1_timeouts_col = 'Timeouts P{}'.format(a)
        p2_timeouts_col = 'Timeouts P{}'.format(b)
        # Extending data for wins, ties and defeats
        df = table.set_index(p1_col).drop(
            [p2_col, p2_time_col, p2_timeouts_col], axis=1)
        df[p1_time_col] /= 1000
        df['Wins'] = (df[p1_points_col] > df[p2_points_col]).astype(int)
        df['Defeats'] = (df[p1_points_col] < df[p2_points_col]).astype(int)
        df['Ties'] = (df[p1_points_col] == df[p2_points_col]).astype(int)
        df[p1_timeouts_col] = df[p1_timeouts_col].astype(int)
        df = df.groupby(p1_col).sum()
        # Renaming columns and index
        df.index.names = ['Bot']
        df = df.rename(columns={p1_points_col: 'Seeds for',
                                p2_points_col: 'Seeds against',
                                p1_time_col: 'Total Time(s)',
                                p1_timeouts_col: 'Total Timeouts'})
        tables.append(df)
    # Merge all tables
    result = reduce(lambda x, y: x.add(y, fill_value=0), tables)
    # Create Points columns based on wins and ties.
    result['Points'] = result['Wins'] * 3 + result['Ties']
    result = result.reindex(columns=['Points', 'Wins', 'Defeats', 'Ties',
                                     'Seeds for', 'Seeds against',
                                     'Total Timeouts', 'Total Time(s)'])
    # Ranking bots
    result = result.sort_values(by=['Points', 'Seeds for', 'Total Time(s)'],
                                ascending=False)
    return result


def run_competition(block_thread=True):
    global is_running_competition
    if is_running_competition:
        return False

    def inner():
        global is_running_competition
        is_running_competition = True
        logging.info('Ejecutando competicion')
        bot_list = glob.glob('{}/*'.format(BOTS_FOLDER))
        match_list = [(p1, p2)
                      for p1 in bot_list for p2 in bot_list if p1 != p2]
        with Pool() as p:
            match_data = p.starmap(run_match, match_list)
            matches_table = create_matches_table(match_data)
            ranking = create_ranking_table(matches_table)
            matches_table.set_index(
                matches_table.columns[0]).to_csv(MATCHES_CSV)
            ranking.to_csv(RANKING_CSV)
            is_running_competition = False
            logging.info('Competición terminada')

    if block_thread:
        inner()
    else:
        Thread(target=inner).start()
    return True


def get_current_data():
    exec_date = time.ctime((os.stat(RANKING_CSV).st_mtime))
    ranking = pd.read_csv(RANKING_CSV)
    matches = pd.read_csv(MATCHES_CSV)
    return (exec_date, ranking, matches)


def get_next_execution():
    now = datetime.now()
    lapse = 24 / EXECUTIONS_PER_DAY
    next_hour = now + timedelta(hours=lapse - now.hour % lapse)
    return next_hour.strftime('%Y-%m-%d %H:00:00')
