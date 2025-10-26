#!/usr/bin/env python3
"""
Pharmacy Management System - Web Access
This creates a simple web page that redirects to your local system
"""

import http.server
import socketserver
import webbrowser
import threading
import time

class RedirectHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            
            html = """
<!DOCTYPE html>
<html>
<head>
    <title>Pharmacy Management System</title>
    <style>
        body { 
            font-family: Arial, sans-serif; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            margin: 0; padding: 0; min-height: 100vh;
            display: flex; align-items: center; justify-content: center;
        }
        .container { 
            background: white; padding: 40px; border-radius: 20px; 
            box-shadow: 0 20px 40px rgba(0,0,0,0.1); text-align: center;
            max-width: 500px;
        }
        h1 { color: #2c3e50; margin-bottom: 20px; }
        .btn { 
            background: #3498db; color: white; padding: 15px 30px; 
            border: none; border-radius: 10px; font-size: 18px; 
            cursor: pointer; margin: 10px; text-decoration: none;
            display: inline-block;
        }
        .btn:hover { background: #2980b9; }
        .status { margin: 20px 0; padding: 15px; border-radius: 10px; }
        .success { background: #d4edda; color: #155724; }
        .info { background: #d1ecf1; color: #0c5460; }
    </style>
</head>
<body>
    <div class="container">
        <h1>🏥 Pharmacy Management System</h1>
        <p>Your system is running successfully!</p>
        
        <div class="status success">
            ✅ System Status: RUNNING<br>
            🌐 URL: http://localhost:8000<br>
            🔑 Login: admin / admin123
        </div>
        
        <a href="http://localhost:8000" class="btn" target="_blank">
            🚀 Open Pharmacy System
        </a>
        
        <div class="status info">
            <strong>Test Credentials:</strong><br>
            Admin: admin / admin123<br>
            Doctor: doctor / doctor123<br>
            Patient: patient / patient123
        </div>
        
        <p><small>If the link doesn't work, copy this URL: http://localhost:8000</small></p>
    </div>
    
    <script>
        // Auto-redirect after 3 seconds
        setTimeout(function() {
            window.open('http://localhost:8000', '_blank');
        }, 3000);
    </script>
</body>
</html>
            """
            
            self.wfile.write(html.encode())
        else:
            self.send_error(404, "Not Found")

def main():
    PORT = 3000
    
    print("🌐 Pharmacy System Web Access")
    print("=" * 30)
    print(f"📱 Access URL: http://localhost:{PORT}")
    print("🚀 This will redirect you to your pharmacy system")
    print("⏹️ Press Ctrl+C to stop")
    
    with socketserver.TCPServer(("", PORT), RedirectHandler) as httpd:
        try:
            # Auto-open browser
            def open_browser():
                time.sleep(1)
                webbrowser.open(f'http://localhost:{PORT}')
            
            browser_thread = threading.Thread(target=open_browser)
            browser_thread.daemon = True
            browser_thread.start()
            
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n👋 Web access stopped!")

if __name__ == "__main__":
    main()



