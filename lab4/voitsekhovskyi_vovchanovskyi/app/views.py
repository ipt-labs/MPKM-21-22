from app import app
from functools import wraps
from flask import render_template, request, Response
import os
from werkzeug.utils import secure_filename
from werkzeug.datastructures import  FileStorage

def check_auth(username, password):
    """This function is called to check if a username /
    password combination is valid.
    """
    return username == 'superlab' and password == 'letmein'

def authenticate():
    """Sends a 401 response that enables basic auth"""
    return Response(
    'Could not verify your access level for that URL.\n'
    'You have to login with proper credentials', 401,
    {'WWW-Authenticate': 'Basic realm="Login Required"'})

def requires_auth(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        auth = request.authorization
        if not auth or not check_auth(auth.username, auth.password):
            return authenticate()
        return f(*args, **kwargs)
    return decorated


@app.route('/')
def home():
    return render_template("index.html")

@app.route('/sign')
def sign():
    return render_template("sign.html")

@app.route('/verify')
def verify():
    return render_template("verify.html")

@app.route('/certs', methods=['GET','POST'])
def certs():
    certificates = os.listdir('./app/static/keys')
    print(certificates)
    return render_template("certs.html",certificates = certificates)

@app.route('/addcert', methods=['GET','POST'])
@requires_auth
def addcert():
    certificates = ['']
    if request.method == 'POST':
            file = request.files['file']
            filename = secure_filename(request.form['username'])
            file.save(os.path.join('./app/static/keys/', filename))
    if request.method == 'GET':
        certificates = os.listdir('./app/static/keys')
    return render_template("admin.html",certificates = certificates)

@app.route('/logout')
def Logout():
        return render_template("index.html"), 401