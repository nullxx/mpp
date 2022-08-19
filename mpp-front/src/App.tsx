import React from "react";
import "./App.css";
import { connectBackend } from "./lib/core";

import CPUTable from "./pages/CPUTable";
import Welcome from "./pages/Welcome";

function App() {
  const [coreLoaded, setCoreLoaded] = React.useState(false);
  const [ready, setReady] = React.useState(false);
  const [showWelcome, setShowWelcome] = React.useState(false);

  React.useEffect(() => {
    if (!coreLoaded) return;
    setTimeout(() => setShowWelcome(true), 100);
    setTimeout(() => setReady(true), 1000);
  }, [coreLoaded]);

  React.useEffect(() => {
    connectBackend()
      .then(() => {
        setCoreLoaded(true);
      })
      .catch(() => {
        setCoreLoaded(false);
      });
  }, []);

  return (
    <div className="App">
      {showWelcome && !ready && <Welcome />}
      {coreLoaded && <CPUTable show={!ready} />}
    </div>
  );
}

export default App;
