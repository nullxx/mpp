import React from "react";
import "./App.css";
import { StepType, TourProvider } from "@reactour/tour";

import { connectBackend } from "./lib/core";

import CPUTable from "./pages/CPUTable";
import Welcome from "./pages/Welcome";
import I18n from "./components/i18n";

const tourSteps: StepType[] = [
  {
    selector: ".onboarding-docs",
    content: <I18n k="tour.docs" />,
  },
  {
    selector: ".onboarding-mainTable",
    content: <I18n k="tour.mainTable" />,
  },
  {
    selector: ".onboarding-editor",
    content: <I18n k="tour.coder" />,
  },
  {
    selector: ".onboarding-runButtons",
    content: <I18n k="tour.runButtons" />,
    position: 'center' // avoid overpassing the screen
  },
  {
    selector: ".onboarding-settings",
    content: <I18n k="tour.settings" />,
  },
  {
    selector: ".onboarding-info",
    content: <I18n k="tour.info" />,
  },
  {
    selector: ".onboarding-opensource",
    content: <I18n k="tour.openSource" />,
  },
];

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
      {coreLoaded && (
        <TourProvider steps={tourSteps}>
          <CPUTable hidden={!ready} />
        </TourProvider>
      )}
    </div>
  );
}

export default App;
