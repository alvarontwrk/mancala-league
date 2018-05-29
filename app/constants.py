# Constants
BOTS_FOLDER = 'bots'
DATA_FOLDER = 'data'
RANKING_CSV = '{}/ranking.csv'.format(DATA_FOLDER)
MATCHES_CSV = '{}/matches.csv'.format(DATA_FOLDER)
MANCALA_COMMAND = 'java -jar ./bin/MancalaNoGUI.jar -p1 {} -p2 {} -t 2'
LOG_FILE = 'info.log'
UPLOAD_FOLDER = 'compilation_enviroment'
MAKE_COMMAND = 'make -C {}'.format(UPLOAD_FOLDER)
EXECUTIONS_PER_DAY = 12
ALLOWED_EXTENSIONS = set(['hpp', 'h', 'cpp'])
BLACK_LIST_FUN = ['system', 'fork', 'exec', 'popen', 'execl', 'execlp', 'cout',
                  'printf']
