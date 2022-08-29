import React, { memo } from "react";
import { Text, Row, Col } from "atomize";
import { Handle, Position } from "react-flow-renderer";
import useUpdateEdges from "../../../hook/useUpdateEdges";
import I18n from "../../../components/i18n";

export default memo(({ data, id }: any) => {
  useUpdateEdges({ data, id });

  return (
    <div
      style={{
        borderRadius: 5,
        padding: 5,
        background: "#fff",
        width: data.width,
        height: data.height,
      }}
      className="loadable-node"
    >
      <Handle
        type="source"
        position={Position.Top}
        style={{ background: "#555", position: "absolute", left: "70%" }}
        onConnect={(params) => console.log("handle onConnect", params)}
        isConnectable={true}
      />
      <Handle
        type="target"
        position={Position.Top}
        style={{ background: "#555", position: "absolute", left: "30%" }}
        onConnect={(params) => console.log("handle onConnect", params)}
        isConnectable={true}
      />
      <Row>
        <Col size="100%">
          <Text tag="h4" textSize="display4">
            <I18n k={data.labelKey} />
          </Text>
        </Col>
      </Row>
    </div>
  );
});
