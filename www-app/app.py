#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import re
import logging
import flask
import requests
import time
import json
import cra_api
import config

logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

app = flask.Flask(__name__, static_url_path='/static')

cra = cra_api.Api(config.username, config.password)

@app.route('/')
def index():
    return flask.render_template('index.html')

@app.route('/send-text', methods=['POST', 'GET'])
def send_text():
    payload = None
    if flask.request.is_json:
        payload = flask.request.json
    else:
        payload = flask.request.form.to_dict()

    print(payload)

    if payload['text'] and len(payload['text']) < 16 and payload['x'] and payload['y']:

        buffer = bytes([payload.get('clear', '') == 'on', int(payload['x']), int(payload['y']) ]) + payload['text'].encode() + b'\x00'

        print(buffer.hex())

        cra.send("323831397436820C", buffer.hex())

    return flask.redirect('/')


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=int(os.environ.get('PORT', 5000)), debug=True)
