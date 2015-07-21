#!/usr/bin/python3

from http.server import HTTPServer, BaseHTTPRequestHandler
import os
import sys

class WomboHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        if self.path.startswith('/data'):
            try:
                f = open(self.path[1:])
                self.send_response(200)

                if self.path.endswith('bin'):
                    f.close()
                    f = open(self.path[1:], 'rb')
                    self.send_header('Content-type', 'application/octet-stream')
                    self.end_headers()
                    self.wfile.write(f.read())
                else:
                    self.send_header('Content-type', 'application/json')
                    self.end_headers()
                    self.wfile.write(bytes(f.read(), 'utf8'))

            except Exception as e:
                print(e)
                self.send_error(404, 'File Not Found: %s' % self.path)
        else:
            if self.path == '/':
                self.path = '/index.html'
            try:
                self.path = 'src/main/web' + self.path
                f = open(self.path)
                self.send_response(200)
                mimetype = 'text/html'
                self.send_header('Content-type', mimetype)
                self.end_headers()
                self.wfile.write(bytes(f.read(), 'utf8'))
            except Exception as e:
                print(e)
                self.send_error(404, 'File Not Found: %s' % self.path)

def run(server_class=HTTPServer, handler_class=BaseHTTPRequestHandler):
    server_address = ('', 8000)
    print('http://localhost:8000/')
    httpd = server_class(server_address, handler_class)
    httpd.serve_forever()

run(handler_class=WomboHandler)
