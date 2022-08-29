import React from "react";
import {
  subscribeToUIUpdates,
  unsubscribeToUIUpdates,
} from "../../../../lib/core";
import { clockCycleTime } from "../RunButtons";
import { Tooltip } from "antd";
import "./Progress.css";
import { Progress } from "antd";
import { red, green, orange } from "@ant-design/colors";
import I18n from "../../../../components/i18n";

const maxTimeMS = 3;

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
      <h4><I18n k={data.labelKey} /></h4>
      <Tooltip
        title={
          lastCycleTime === -1
            ? null
            : `It took ${lastCycleTime}ms to complete the last clock cycle`
        }
      >
        <Progress
          percent={(lastCycleTime / maxTimeMS) * 100}
          steps={4}
          format={() => `${lastCycleTime}ms`}
          success={{ percent: 1000 }}
          strokeColor={[green[6], orange[3], orange[5], red[6]]}
        />
      </Tooltip>
    </div>
  );
}
