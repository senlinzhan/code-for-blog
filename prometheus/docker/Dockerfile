from ubuntu:latest

RUN apt-get update && apt-get install -y
RUN apt-get install -y gunicorn python-pip python-dev build-essential

WORKDIR /app
COPY ./requirements.txt /app
COPY ./app.py /app
COPY ./gunicorn_config.py /app

RUN pip install -r requirements.txt
CMD ["/usr/bin/gunicorn", "--config", "gunicorn_config.py", "app:app"]

EXPOSE 5000
