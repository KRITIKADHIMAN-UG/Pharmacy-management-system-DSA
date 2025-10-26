#!/usr/bin/env python3
"""
Pharmacy Management System - Web Launcher
Automatically starts the project and provides a web interface
"""

import http.server
import socketserver
import webbrowser
import subprocess
import os
import sys
import time
import threading
import json
from urllib.parse import urlparse, parse_qs

class ProjectLauncherHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.path = '/launcher.html'
        elif self.path == '/launch':
            self.handle_launch()
            return
        elif self.path == '/status':
            self.handle_status()
            return
        return super().do_GET()
    
    def handle_launch(self):
        """Launch the Pharmacy Management System"""
        try:
            # Check if we're on Windows or Unix-like system
            if os.name == 'nt':  # Windows
                script_path = 'project_official.bat'
            else:  # Unix-like (Linux, Mac)
                script_path = './project_official.sh'
            
            # Start the project in a separate thread
            def run_project():
                try:
                    if os.name == 'nt':
                        subprocess.run([script_path], shell=True)
                    else:
                        subprocess.run([script_path], shell=True)
                except Exception as e:
                    print(f"Error running project: {e}")
            
            # Start project in background
            project_thread = threading.Thread(target=run_project)
            project_thread.daemon = True
            project_thread.start()
            
            # Wait a moment for project to start
            time.sleep(2)
            
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            
            response = {
                'success': True,
                'message': 'Project launched successfully!',
                'url': 'http://localhost:8000',
                'status': 'running'
            }
            
            self.wfile.write(json.dumps(response).encode())
            
        except Exception as e:
            self.send_error(500, f"Failed to launch project: {str(e)}")
    
    def handle_status(self):
        """Check if the project is running"""
        try:
            import urllib.request
            try:
                response = urllib.request.urlopen('http://localhost:8000', timeout=2)
                status = 'running'
                message = 'Project is running successfully!'
            except:
                status = 'stopped'
                message = 'Project is not running'
            
            self.send_response(200)
            self.send_header('Content-type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            
            response = {
                'status': status,
                'message': message,
                'url': 'http://localhost:8000'
            }
            
            self.wfile.write(json.dumps(response).encode())
            
        except Exception as e:
            self.send_error(500, f"Failed to check status: {str(e)}")

def main():
    PORT = 3000
    
    print("🏥 Pharmacy Management System - Web Launcher")
    print("=" * 50)
    print(f"🌐 Launcher running at http://localhost:{PORT}")
    print("📱 Open your browser and navigate to the URL above")
    print("🚀 Click 'Launch Project' to start the system")
    print("\n⏹️ Press Ctrl+C to stop the launcher")
    
    Handler = ProjectLauncherHandler
    
    with socketserver.TCPServer(("", PORT), Handler) as httpd:
        try:
            # Auto-open browser
            time.sleep(1)
            webbrowser.open(f'http://localhost:{PORT}')
            
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n👋 Launcher stopped!")

if __name__ == "__main__":
    main()



