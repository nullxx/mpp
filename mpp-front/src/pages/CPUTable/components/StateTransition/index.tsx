import React, { useState } from "react";
import { execute, unsubscribeToUIUpdates } from "../../../../lib/core";

import { subscribeToUIUpdates } from "../../../../lib/core/index";
import stateDetails from "./constants.json";

import { Steps } from "antd";
import usePrev from "../../../../hook/usePrev";
import IconButton from "../../../../components/IconButton";
import { DeleteOutlined } from "@ant-design/icons";
const { Step } = Steps;

interface State {
  state: number;
  op: string;
}

export default function StateTransition() {
  const [states, setStates] = useState<State[]>([]);
  const [currentState, setCurrentState] = useState(0);
  const [nextState, setNextState] = useState(0);
  const [currentStep, setCurrentStep] = useState(0);

  const divRef = React.useRef<HTMLDivElement>(null);
  const prevCurrentState = usePrev(currentState);
  const prevNextState = usePrev(nextState);

  React.useEffect(() => {
    if (divRef.current) {
      divRef.current.scrollTop = divRef.current.scrollHeight;
    }
  }, [states.length]);

  function onUIUpdate() {
    const nextStateNumber = execute("get_next_state");
    const currentStateNumber = execute("get_state");

    if (currentStateNumber === prevCurrentState) return;
    if (nextStateNumber === prevNextState) return;

    const nextState = stateDetails[nextStateNumber];
    const currentState = stateDetails[currentStateNumber];

    setStates((prev) => {
      const nnew = [...prev];

      if (prev.length === 0) {
        nnew.push(currentState, nextState);
      } else {
        nnew.push(nextState);
      }
      setCurrentStep(nnew.length - 1);

      return nnew;
    });

    setCurrentState(currentStateNumber);
    setNextState(nextStateNumber);
  }

  function handleClear() {
    setStates([]);
    setCurrentState(0);
    setNextState(0);
    setCurrentStep(0);
  }

  React.useEffect(() => {
    subscribeToUIUpdates(onUIUpdate);
    return () => {
      unsubscribeToUIUpdates(onUIUpdate);
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  return (
    <div
      ref={divRef}
      style={{
        border: "1px solid black",
        backgroundColor: "white",
        maxHeight: 300,
        overflow: "auto",
        width: 220,
        padding: 8,
      }}
    >
      <h4>States graph</h4>
      <Steps direction="vertical" size="small" current={currentStep}>
        {states.map((state: State, index: number, arr) => {

          const title =
            index === arr.length - 1 ? "Next State" : "✅ Already executed";
          return (
            <Step
              key={index}
              title={title}
              description={state.op}
              progressDot={() => "S" + state.state}
            />
          );
        })}
      </Steps>
      {states.length > 0 ? (
          <IconButton
            icon={<DeleteOutlined />}
            title="Clear"
            onClick={handleClear}
          />
        ) : <sub>No states</sub>
      }
    </div>
  );
}
