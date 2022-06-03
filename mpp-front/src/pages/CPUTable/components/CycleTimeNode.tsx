import React from "react";
import {
  subscribeToUIUpdates,
  unsubscribeToUIUpdates,
} from "../../../lib/core";
import { clockCycleTime } from "./RunButtons";
import { Tooltip } from "antd";
export default function CycleTimeNode() {
  const [lastCycleTime, setLastCycleTime] = React.useState(clockCycleTime);

  function onUIUpdate() {
    setLastCycleTime(clockCycleTime);
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
      style={{
        paddingLeft: 20,
        paddingRight: 20,
        border: "1px solid black",
        backgroundColor: "white",
      }}
    >
      <h4>Clock cycle time</h4>
      <Tooltip
        title={
          lastCycleTime === -1
            ? null
            : `It took ${lastCycleTime}s to complete the last clock cycle`
        }
      >
        <h4>{lastCycleTime === -1 ? "-" : lastCycleTime + "s"}</h4>
      </Tooltip>
    </div>
  );
}
