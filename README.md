# Mandala-League

Underground Mancala League's Server. Web app for running mancala bots competitions based on Decsai Mancala simulator.

## Features

* All vs All league
* 1 vs 1 Match
* Automatic compilation

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

```shell
git clone https://github.com/AlvaroGarciaJaen/mancala-league.git
cd mancala-league
```

We recommend to use [virtualenv](https://virtualenv.pypa.io/en/stable/) before install requirements:

``````shell
virtualenv env
source env/bin/activate
pip install -r requirements.dev.txt
``````

If you don't want to use it, just execute:

```shell
pip install -r requirements.dev.txt
```

#### Deployment notes

For just deploying purposes use **requirements.txt** instead

### Running

```
python3 run.py &
```

## Running the tests

We use [pytest](http://pytest.readthedocs.io/en/latest/) for testing automation. So, running tests, it's just as simple as:

```
pytest
```

We recommend to use [flake8](http://flake8.pycqa.org/en/latest/) for coding style testing:

```shell
flake8 app
```

## Built With

* [Flask](http://flask.pocoo.org/) - The web framework used
* [Pandas](https://pandas.pydata.org/) - Data processing module used
* [Schedule](https://github.com/dbader/schedule) - Job scheduling module used

## Authors

* **Álvaro García Jaén** - *Infraestructure management* - [AlvaroGarciaJaen](https://github.com/AlvaroGarciaJaen) 
* **Antonio Molner Domenech** - *Web app development* - [antoniomdk](https://github.com/antoniomdk)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Mancala simulator and interface thanks to [DECSAI](https://decsai.ugr.es/)

