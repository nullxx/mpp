import React, { memo } from "react";
import { Handle, Position } from "../../../lib/ReactFlow";
import { Text, Row, Col } from "atomize";
import NumberBaseInput from "../../../components/NumberBaseInput";
import {
  execute,
  subscribeToUIUpdates,
  unsubscribeToUIUpdates,
} from "../../../lib/core";

const DEFAULT_BUS_VALUE = 0;

export default memo(({ data, isConnectable, id }: any) => {
  const [value, setValue] = React.useState(DEFAULT_BUS_VALUE);
  const [, setChanged] = React.useState(false);

  function onUIUpdate() {
    if (!data.getFunction)
      return console.warn(
        `Not updating ${data.label}. Missing data.getFunction`
      );

    const out = execute(data.getFunction);

    setValue((prevValue) => {
      let hasChanged = false;
      if (prevValue !== out) {
        hasChanged = true;
      }
      setChanged(hasChanged);
      return out;
    });
  }

  React.useEffect(() => {
    subscribeToUIUpdates(onUIUpdate);
    return () => {
      unsubscribeToUIUpdates(onUIUpdate);
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  const handleSourceBottomPositions = [10, 40, 80, 100];
  const handleTargetBottomPositions = [10, 20, 50, 70, 90];

  const handleSourceTopPositions = [5, 20, 37, 55, 72, 90];
  const handleTargetTopPositions = [30, 45, 60, 77, 95];

  return (
    <div
      style={{
        width: data.width,
        padding: 5,
      }}
      className="bus pretty-shadow"
    >
      <Row>
        <Col>
          <Text tag="p" textSize="display5">
            {data.label}
          </Text>
        </Col>
      </Row>
      <Row>
        <Col>
          <NumberBaseInput
            initialBase="HEX"
            number={value}
            width={180}
            readOnly
          />
        </Col>
      </Row>

      {handleSourceTopPositions.map((position) => (
        <Handle
          key={`${id}-top-source-${position}`}
          id={`${id}-top-source-${position}`}
          type="source"
          position={Position.Top}
          style={{
            background: "#555",
            position: "absolute",
            left: `${position}%`,
          }}
          isConnectable={false}
        />
      ))}

      {handleTargetTopPositions.map((position) => (
        <Handle
          key={`${id}-top-target-${position}`}
          id={`${id}-top-target-${position}`}
          type="target"
          position={Position.Top}
          style={{
            background: "#555",
            position: "absolute",
            left: `${position}%`,
          }}
          isConnectable={false}
        />
      ))}

      {handleSourceBottomPositions.map((position) => (
        <Handle
          key={`${id}-bottom-source-${position}`}
          id={`${id}-bottom-source-${position}`}
          type="source"
          position={Position.Bottom}
          style={{
            background: "#555",
            position: "absolute",
            left: `${position}%`,
          }}
          isConnectable={false}
        />
      ))}

      {handleTargetBottomPositions.map((position) => (
        <Handle
          key={`${id}-bottom-target-${position}`}
          id={`${id}-bottom-target-${position}`}
          type="target"
          position={Position.Bottom}
          style={{
            background: "#555",
            position: "absolute",
            left: `${position}%`,
          }}
          isConnectable={false}
        />
      ))}
    </div>
  );
});
