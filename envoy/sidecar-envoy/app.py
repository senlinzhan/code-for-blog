import socket
from flask import Flask

app = Flask(__name__)


@app.route('/')
def hello_world():
    return 'Hello, World!'


@app.route('/healthy_check')
def healthy_check():
    service = socket.gethostname()
    ip = socket.gethostbyname(service)
    return 'I am fine! <service_name: {}, ip: {}>'.format(service, ip)


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
