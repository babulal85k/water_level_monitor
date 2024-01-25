# Water Level Monitor Project

## Project Description
This project is a water level monitoring system built with a Flask backend and a frontend displaying water level data in real-time. It utilizes an ESP8266 with an ultrasonic sensor to measure the water level and uses Blynk for IoT connectivity.

## File Structure
```
/YourProject
│
├── backend                     # Directory for Flask backend
│   ├── app.py                  # Main Flask application file
│   ├── static                  # Directory for static files
│   │   ├── css                 # Subdirectory for CSS files
│   │   │   └── style.css       # CSS file for styling your webpage
│   │   ├── js                  # Subdirectory for JavaScript files
│   │   │   └── script.js       # JavaScript file to fetch and display data
│   │   └── images              # Subdirectory for images (if any)
│   └── templates               # Directory for HTML templates
│       └── index.html          # HTML template for your webpage
│
├── venv                        # Virtual environment directory (optional)
|
├── nodeMCU
|    └──ESP8266_main
|         ├──BlynkConfig.h      # Used to store configuration details
|         └──ESP8266_main.ino   # Arduino sketch file
│
└── requirements.txt            # File listing Python dependencies
```

## Setup and Installation

### Python Environment Setup
1. **Create a Virtual Environment**:
   - Navigate to your project directory and create a virtual environment:
     ```bash
     python -m venv venv
     ```
2. **Activate the Virtual Environment**:
   - On Windows:
     ```cmd
     .\venv\Scripts\activate
     ```
   - On macOS/Linux:
     ```bash
     source venv/bin/activate
     ```

### Install Required Packages
- Install the necessary Python packages:
  ```bash
  pip install -r requirements.txt
  ```

### Running the Flask Application
- Navigate to the `backend` directory:
  ```bash
  cd backend
  ```
- Start the Flask server:
  ```bash
  python app.py
  ```

## Usage
- Access the web interface at `http://127.0.0.1:5000/` to view the real-time water level data.
- The ESP8266 should be set up and connected to Blynk with the correct virtual pin configurations.

## Contributing
- Contributions to this project are welcome. Please fork the repository and submit a pull request.
