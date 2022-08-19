import React from "react";
import "./App.css";
import { connectBackend } from "./lib/core";

import CPUTable from "./pages/CPUTable";
import Welcome from "./pages/Welcome";

function App() {
  const [coreLoaded, setCoreLoaded] = React.useState(false);
  const [ready, setReady] = React.useState(false);

  React.useEffect(() => {
    if (!coreLoaded) return;
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
      {!ready && <Welcome />}
      {coreLoaded && <CPUTable />}
    </div>
  );
}

export default App;
