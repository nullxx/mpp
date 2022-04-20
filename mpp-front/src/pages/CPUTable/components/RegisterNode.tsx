import React, { memo } from "react";
import { Text, Input, Row, Col } from "atomize";

export const DEFAULT_REGISTER_VALUE_HEX = '0x00';

export default memo(({ data, isConnectable }: any) => {
  return (
    <div
      style={{
        border: "1px solid #1a192b",
        borderRadius: 3,
        padding: 5,
        background: "#fff",
      }}
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
          <Input h="2rem" w="60px" readOnly={data.readOnly} defaultValue={DEFAULT_REGISTER_VALUE_HEX} />
        </Col>
      </Row>
    </div>
  );
});
