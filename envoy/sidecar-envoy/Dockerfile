from lyft/envoy:latest

RUN apt-get update && apt-get install -y
RUN apt-get install -y curl python-pip python-dev build-essential

WORKDIR /app
COPY ./requirements.txt /app
COPY ./app.py /app
COPY ./envoy.json /etc/envoy.json
COPY ./entrypoint.sh .

RUN pip install -r requirements.txt

RUN chmod +x entrypoint.sh
ENTRYPOINT [ "./entrypoint.sh" ]

