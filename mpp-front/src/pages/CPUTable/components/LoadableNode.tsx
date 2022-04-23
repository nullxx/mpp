import React, { memo } from "react";
import { Text, Row, Col } from "atomize";



export default memo(({ data, isConnectable }: any) => {
  return (
    <div
      style={{
        border: "1px solid #1a192b",
        borderRadius: 3,
        padding: 5,
        background: "#fff",
        width: data.width,
        height: data.height,
      }}
    >
      {/* <Handle
        type="target"
        position={Position.Top}
        style={{ background: "#555" }}
        onConnect={(params) => console.log("handle onConnect", params)}
        isConnectable={isConnectable}
      /> */}
      <Row>
        <Col size="100%">
          <Text tag="h4" textSize="display4">
            {data.label}
          </Text>
        </Col>
      </Row>

      {/* {data.loadBit && (
        <LoadBitComponent
          lb={data?.loadBit}
          totalWidth={data?.width}
          onChange={data?.onChange}
        />
      )} */}
    </div>
  );
});
