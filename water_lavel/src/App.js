import React, { useEffect, useState } from 'react';
import axios from 'axios';
import './App.css';

function App() {
  const [waterLevel, setWaterLevel] = useState(0);

  useEffect(() => {
    const interval = setInterval(() => {
      axios.get('http://localhost:5000/get_latest')
        .then(response => {
          setWaterLevel(response.data.level);
        })
        .catch(error => console.error('Error fetching data:', error));
    }, 2000);

    return () => clearInterval(interval);
  }, []);

  return (
    <div className="App">
      <header className="App-header">
        <h1>Water Level Monitoring Dashboard</h1>
      </header>
      <div className="water-level-display">
        <h2>Current Water Level:</h2>
        <p>{waterLevel} meters</p>
      </div>
    </div>
  );
}

export default App;
