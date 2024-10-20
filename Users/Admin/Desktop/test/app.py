from flask import Flask
import os
import time
import subprocess

app = Flask(_name_)

@app.route('/htop')
def htop():
    
    full_name = "Your Full Name"  # Replace with your full name
    username = os.getlogin()
    server_time = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())
    
    
    ist_time = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time() + 19800))  # IST is UTC +5:30

    
    top_output = subprocess.getoutput('top -b -n 1')

    
    response = f"""
    <html>
        <head><title>HTop Info</title></head>
        <body>
            <h1>System Information</h1>
            <p><b>Full Name:</b> {full_name}</p>
            <p><b>Username:</b> {username}</p>
            <p><b>Server Time (IST):</b> {ist_time}</p>
            <pre>{top_output}</pre>
        </body>
    </html>
    """
    return response

if _name_ == '_main_':
    app.run(host='0.0.0.0', port=8080)