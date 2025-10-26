from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_marshmallow import Marshmallow
from flask_jwt_extended import JWTManager
from dotenv import load_dotenv
import os

# Load environment variables
load_dotenv()

# Initialize Flask app
app = Flask(__name__)

# Configuration
app.config['SECRET_KEY'] = os.getenv('SECRET_KEY', 'your-secret-key-here')
app.config['SQLALCHEMY_DATABASE_URI'] = os.getenv('DATABASE_URL', 'sqlite:///healthcare.db')
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config['JWT_SECRET_KEY'] = os.getenv('JWT_SECRET_KEY', 'jwt-secret-string')

# Initialize extensions
db = SQLAlchemy(app)
ma = Marshmallow(app)
jwt = JWTManager(app)

# Import models
from models.user import User
from models.patient import Patient
from models.doctor import Doctor
from models.bed import Bed
from models.medicine import Medicine

# Import routes
from routes.auth import auth_bp
from routes.patients import patients_bp
from routes.doctors import doctors_bp
from routes.beds import beds_bp
from routes.medicines import medicines_bp

# Register blueprints
app.register_blueprint(auth_bp, url_prefix='/api/auth')
app.register_blueprint(patients_bp, url_prefix='/api/patients')
app.register_blueprint(doctors_bp, url_prefix='/api/doctors')
app.register_blueprint(beds_bp, url_prefix='/api/beds')
app.register_blueprint(medicines_bp, url_prefix='/api/medicines')

@app.route('/')
def index():
    return {
        'message': 'Cloud-Based Healthcare and Hospital Resource Management System API',
        'version': '1.0.0',
        'status': 'running'
    }

@app.route('/api/health')
def health_check():
    return {
        'status': 'healthy',
        'database': 'connected' if db.engine.execute('SELECT 1').fetchone() else 'disconnected'
    }

if __name__ == '__main__':
    with app.app_context():
        db.create_all()
        print("Database tables created successfully!")
    
    app.run(debug=True, host='0.0.0.0', port=5000)







