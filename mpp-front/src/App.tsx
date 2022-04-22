import React from "react";
import { Routes, Route } from "react-router-dom";
import "./App.css";
import "@infinite-table/infinite-react/index.css"
import { connectBackend, execute } from "./lib/core";

import CPUTable from "./pages/CPUTable";
import { getCore } from "./lib/core/index";
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
    // return <Welcome />;
    return <h1>hola</h1>;
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
