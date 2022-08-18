import React from "react";
import {
  subscribeToUIUpdates,
  unsubscribeToUIUpdates,
} from "../../../lib/core";
import { clockCycleTime } from "./RunButtons";
import { Tooltip } from "antd";
export default function CycleTimeNode({ data }: any) {
  const [lastCycleTime, setLastCycleTime] = React.useState(clockCycleTime);

  function onUIUpdate() {
    setLastCycleTime(clockCycleTime * 1000); // s to ms
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
        paddingTop: 10,
        paddingBottom: 10,
        backgroundColor: "#f5f5f5",
      }}
      className="pretty-shadow"
    >
      <h4>{data.label}</h4>
      <Tooltip
        title={
          lastCycleTime === -1
            ? null
            : `It took ${lastCycleTime}ms to complete the last clock cycle`
        }
      >
        <h4>{lastCycleTime === -1 ? "-" : lastCycleTime + "ms"}</h4>
      </Tooltip>
    </div>
  );
}
