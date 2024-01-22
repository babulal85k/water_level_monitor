from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import logging

app = Flask(__name__)

# Externalize configuration
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///waterlevel.db'

db = SQLAlchemy(app)

class WaterLevelReading(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    level = db.Column(db.Float, nullable=False)
    timestamp = db.Column(db.DateTime, default=datetime.utcnow)

    def __repr__(self):
        return f"<WaterLevel {self.level} recorded at {self.timestamp}>"

@app.route('/postdata', methods=['POST'])
def post_data():
    try:
        data = request.json
        # Validate incoming data
        if 'level' not in data or not isinstance(data['level'], (float, int)):
            return jsonify({"error": "Invalid data format"}), 400
        new_reading = WaterLevelReading(level=data['level'])
        db.session.add(new_reading)
        db.session.commit()
        return jsonify({"status": "success"}), 200
    except Exception as e:
        logging.error(f"Error posting data: {e}")
        return jsonify({"error": "Internal server error"}), 500

@app.route('/readings', methods=['GET'])
def get_readings():
    readings = WaterLevelReading.query.all()
    return jsonify([repr(reading) for reading in readings]), 200

if __name__ == '__main__':
    with app.app_context():
        db.create_all()  # Initialize database schema
    app.run()
