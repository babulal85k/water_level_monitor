from flask import Flask, jsonify, request
from flask import render_template
from flask_cors import CORS

# After creating the Flask app


app = Flask(__name__)
CORS(app)

# Simulated database
water_levels = [0]

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/update', methods=['POST'])
def update_water_level():
    print(request.json)  # Debug: Print the entire JSON body
    water_level = request.json.get('value0', 'No level received')
    if water_level != 'No level received':
        water_levels.append(int(water_level))
        print(f"Updated Water Level: {water_level}")  # Debug: Log the level
    else:
        print("No valid water level received")
    return jsonify({"message": "Updated successfully"}), 200

@app.route('/get_latest', methods=['GET'])
def get_latest_water_level():
    return jsonify({"level": water_levels[-1]}), 200

if __name__ == '__main__':
    app.run(debug=True, port=5000)
