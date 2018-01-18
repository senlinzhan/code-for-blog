import time

from flask import Flask, Response, request
from prometheus_client import (
    Counter, Histogram, CONTENT_TYPE_LATEST, generate_latest
)

APP_NAME = 'hello-app'

REQUEST_COUNT = Counter(
    'request_count', 'App Request Count',
    ['app_name', 'method', 'endpoint', 'http_status']
)

REQUEST_LATENCY = Histogram(
    'request_latency_seconds', 'Request latency',
    ['app_name', 'endpoint']
)


def start_timer():
    request.start_time = time.time()


def stop_timer(response):
    resp_time = time.time() - request.start_time
    REQUEST_LATENCY.labels(APP_NAME, request.path).observe(resp_time)
    return response


def record_request_data(response):
    REQUEST_COUNT.labels(
        APP_NAME, request.method, request.path, response.status_code
    ).inc()
    return response


def setup_metrics(app):
    app.before_request(start_timer)

    # The order here matters since we want stop_timer to be executed first
    app.after_request(record_request_data)
    app.after_request(stop_timer)


app = Flask(__name__)


@app.route('/')
def index():
    return 'Hello, World!'


@app.route('/error')
def error():
    1 / 0
    return 'Hello, World!'


@app.errorhandler(500)
def handle_500(error):
    return str(error), 500


@app.route('/metrics')
def metrics():
    return Response(
        generate_latest(),
        mimetype=CONTENT_TYPE_LATEST
    )


if __name__ == '__main__':
    app.run()
