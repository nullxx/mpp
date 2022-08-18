import React, { memo } from "react";
import { Text, Row, Col } from "atomize";
import NumberBaseInput from "../../../components/NumberBaseInput";
import {
  execute,
  subscribeToUIUpdates,
  unsubscribeToUIUpdates,
} from "../../../lib/core/index";
import { Tooltip } from "antd";
import { Handle, Position } from "react-flow-renderer";
import useUpdateEdges from "../../../hook/useUpdateEdges";

export const DEFAULT_REGISTER_VALUE = 0;

export default memo(({ data, id }: any) => {
  const [value, setValue] = React.useState(DEFAULT_REGISTER_VALUE);
  const [changed, setChanged] = React.useState(false);

  useUpdateEdges({ data, id });

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

  return (
    <div
      className={`${changed ? "blob" : ""} pretty-shadow`}
      style={{ padding: 6, backgroundColor: "#f5f5f5" }}
    >
      {data.handlePos === "top" && (
        <>
          <Handle
            type="target"
            position={Position.Top}
            style={{ background: "#555", position: "absolute", left: "30%" }}
            onConnect={(params) => console.log("handle onConnect", params)}
            isConnectable={false}
          />

          <Handle
            type="source"
            position={Position.Top}
            style={{ background: "#555", position: "absolute", left: "70%" }}
            onConnect={(params) => console.log("handle onConnect", params)}
            isConnectable={false}
          />
        </>
      )}

      {data.handlePos === "bottom" && (
        <>
          <Handle
            type="target"
            position={Position.Bottom}
            style={{ background: "#555", position: "absolute", left: "30%" }}
            onConnect={(params) => console.log("handle onConnect", params)}
            isConnectable={false}
          />

          <Handle
            type="source"
            position={Position.Bottom}
            style={{ background: "#555", position: "absolute", left: "70%" }}
            onConnect={(params) => console.log("handle onConnect", params)}
            isConnectable={false}
          />
        </>
      )}

      <Row>
        <Col>
          <Tooltip
            overlay={
              (data.helpInfo || data.docLink) && (
                <>
                  {data.helpInfo}{" "}
                  {data.docLink && (
                    <a href={data.docLink} target="_blank" rel="noreferrer">
                      Check docs
                    </a>
                  )}
                </>
              )
            }
          >
            <Text tag="p" textSize="display5">
              {data.label}
            </Text>
          </Tooltip>
        </Col>
      </Row>
      <Row>
        <Col>
          <NumberBaseInput
            initialBase="HEX"
            number={value}
            width={153}
            readOnly
          />
        </Col>
      </Row>
    </div>
  );
});
