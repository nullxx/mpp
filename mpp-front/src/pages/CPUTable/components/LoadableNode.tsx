import React, { memo } from "react";
import { Text, Checkbox, Row, Col } from "atomize";
import RightArrow, { RIGHT_ARROW_WIDTH } from "./RightArrow";

export interface LoadBit {
  label: string;
  value?: boolean;
}

export const DEFAULT_LOADBIT_VALUE = false;

const LoadBitComponent = ({
  lb,
  totalWidth,
}: {
  lb: LoadBit;
  totalWidth: number;
}) => (
  <>
    <div
      style={{
        position: "absolute",
        right: totalWidth + 15,
        top: 20,
        width: "fit-content",
      }}
    >
      <Text tag="p" textSize="display5">
        {lb.label}
      </Text>
    </div>

    <div
      style={{
        position: "absolute",
        left: -RIGHT_ARROW_WIDTH,
        width: "fit-content",
      }}
    >
      <RightArrow />
    </div>

    <div
      style={{
        position: "absolute",
        right: totalWidth + 15,
        top: 65,
        width: "fit-content",
      }}
    >
      <Text tag="p" textSize="display5">
        <Checkbox checked={typeof lb.value === 'undefined' ? DEFAULT_LOADBIT_VALUE : lb.value} />
      </Text>
    </div>
  </>
);
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
      <Row>
        <Col size="100%">
          <Text tag="h4" textSize="display4">
            {data.label}
          </Text>
        </Col>
      </Row>

      {data.loadBit && (
        <LoadBitComponent lb={data.loadBit} totalWidth={data.width} />
      )}
    </div>
  );
});
