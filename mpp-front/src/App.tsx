import React from "react";
import { Routes, Route } from "react-router-dom";
import "./App.css";
import { connectBackend } from "./lib/core";

import CPUTable from "./pages/CPUTable";
import Welcome from './pages/Welcome';

function App() {
  const [coreLoaded, setCoreLoaded] = React.useState(false);

  React.useEffect(() => {
    connectBackend()
      .then(() => {
        setCoreLoaded(true);
      })
      .catch(() => {
        setCoreLoaded(false);
      });
  }, []);

  if (!coreLoaded) {
    return <Welcome />;
  }

  return (
    <div className="App">
      <Routes>
        <Route path="/" element={<CPUTable />} />
        {/* <Route path="about" element={<About />} /> */}
      </Routes>
    </div>
  );
}

export default App;
