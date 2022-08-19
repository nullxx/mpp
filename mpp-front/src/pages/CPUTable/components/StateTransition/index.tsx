import React, { useState } from "react";
import { unsubscribeToUIUpdates } from "../../../../lib/core";
import { Row, Col, Text } from "atomize";
import { CheckCircleFilled } from "@ant-design/icons";
import { subscribeToUIUpdates, getCore } from '../../../../lib/core/index';
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

const maxItems = 2;

export default function StateTransition({ data }: { data: any }) {
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
    const nextStateNumber = getCore().get_next_state();
    const currentStateNumber = getCore().get_state();

    if (currentStateNumber === prevCurrentState) return;
    if (nextStateNumber === prevNextState) return;

    const nextState = stateDetails[nextStateNumber];
    const currentState = stateDetails[currentStateNumber];

    setStates((prev) => {
      const arr = [...prev];

      if (arr.length >= maxItems) {
        arr.splice(0, arr.length - maxItems);
      }

      if (prev.length === 0) {
        arr.push(currentState, nextState);
      } else {
        arr.push(nextState);
      }
      setCurrentStep(arr.length - 1);

      return arr;
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
        maxHeight: 300,
        overflow: "auto",
        width: 220,
        padding: 8,
        backgroundColor: "#f5f5f5",
      }}
      className="pretty-shadow"
    >
      <Row>
        <Col size="100%">
          <Text tag="h4" textSize="display4">
            {data.label}
          </Text>
        </Col>
      </Row>
      <Row>
        <Col size="100%">
          <Steps direction="vertical" size="small" current={currentStep}>
            {states.map((state: State, index: number, arr) => {
              const title =
                index === arr.length - 1 ? (
                  "Next State"
                ) : (
                  <>
                    <CheckCircleFilled style={{ color: "green" }} /> Executed
                  </>
                );
              return (
                <Step
                  className="state-transition-step"
                  key={index}
                  title={title}
                  description={state.op}
                  progressDot={() => "S" + state.state}
                />
              );
            })}
          </Steps>
        </Col>
      </Row>
      <Row>
        <Col>
          {states.length > 0 ? (
            <IconButton
              icon={<DeleteOutlined />}
              title="Clear"
              onClick={handleClear}
            />
          ) : (
            <sub>No states</sub>
          )}
        </Col>
      </Row>
    </div>
  );
}
