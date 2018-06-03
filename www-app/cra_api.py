#!/usr/bin/env python3
"""
Api description https://app.swaggerhub.com/apis/cra-iot/craiot/2.0
"""
import requests
import json
import logging

BASE_URL = 'https://api.iot.cra.cz/cxf/IOTServices/v2'
VERIFY=False

logging.captureWarnings(True)


class Api:

    def __init__(self, username, password):
        self.username = username
        self.password = password
        self.sessionId = None

    def post(self, url, payload={}):
        r = requests.post(BASE_URL + url, json=payload, headers={'sessionId': self.sessionId}, verify=VERIFY)

        if r.status_code == 401:
            self._login()
            r = requests.post(BASE_URL + url, json=payload, headers={'sessionId': self.sessionId}, verify=VERIFY)

        if r.status_code == 200:
            return r.json()

        raise Exception(r.json())

    def get(self, url):
        r = requests.get(BASE_URL + url, headers={'sessionId': self.sessionId}, verify=VERIFY)

        if r.status_code == 401:
            self._login()
            r = requests.get(BASE_URL + url, headers={'sessionId': self.sessionId}, verify=VERIFY)

        if r.status_code == 200:
            return r.json()

        raise Exception(r.json())

    def _login(self):
        response = requests.post(BASE_URL + "/Login", json={"username": self.username, "password": self.password}, verify=VERIFY).json()

        if response['status'] != 'success':
            raise Exception(response)

        self.sessionId = response['sessionId']


    def send(self, eui, data, port=1):

        payload = {"EUI":eui, "data": json.dumps({"cmd":"tx", "port": port, "data": data,"EUI": eui}) }

        response = self.post("/Message", payload)

        print('response', response)

        return response['status'] == 'success'



if __name__ == '__main__':
    api = Api("karel.blavka@hardwario.com", "")

    # tenantId = 'T201804050608215lhl'

    # print(api.get("/DeviceGroup/Tenant/{tenantId}"))

    api.send("323831397436820C", "0001")
