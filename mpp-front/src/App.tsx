import React from "react";
import { Routes, Route } from "react-router-dom";
import "./App.css";

import CPUTable from './pages/CPUTable'; 

function App() {
  return (
    <div className="App">
      <h1>Welcome to React Router!</h1>
      <Routes>
        <Route path="/" element={<CPUTable />} />
        {/* <Route path="about" element={<About />} /> */}
      </Routes>
    </div>
  );
}

export default App;
