#!/usr/bin/python

import socket
import urllib
from http.server import HTTPServer
from http.server import SimpleHTTPRequestHandler

PORT = 8000

class HTTPServerV4(HTTPServer):
    address_family = socket.AF_INET

class RequestHandler(SimpleHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        self.wfile.write(b'Hello, World!')

    def do_POST(self):
        # Check if Content-Length is provided
        content_length = self.headers.get('Content-Length')

        # Parse the content length from the headers
        if content_length is not None:
            content_length = int(content_length)
            post_data = self.rfile.read(content_length)
        else:
            # If Content-Length is not provided, read until the connection is closed
            post_data = b""
            while True:
                chunk = self.rfile.read(1024)  # Read in chunks
                if not chunk:
                    break
                post_data += chunk

        # Optionally decode the data if it is URL-encoded
        decoded_data = urllib.parse.parse_qs(post_data.decode('utf-8'))

        # For demonstration, we'll just send back the received data
        response = f"Received POST data: {decoded_data}".encode('utf-8')

        # Send response headers
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()

        # Send the response back to the client
        self.wfile.write(response)

def main():
    httpd = HTTPServerV4(("", PORT), RequestHandler)
    print("Serving at port", PORT)
    httpd.serve_forever()

if __name__ == '__main__':
    main()
