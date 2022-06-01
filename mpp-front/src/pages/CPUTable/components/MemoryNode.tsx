import React, { memo } from "react";
import { execute, unsubscribeToUIUpdates } from "../../../lib/core";
import { Row, Col, Text } from "atomize";

import NumberBaseInput, {
  Base,
  getRadix,
} from "../../../components/NumberBaseInput";
import { subscribeToUIUpdates } from "../../../lib/core/index";

function MemoryComponentRow({
  offset,
  value,
  style,
}: {
  offset: string;
  value: string | number;
  style?: React.CSSProperties;
}) {
  if (Number(value) === -1) return null;
  return (
    <Row style={style}>
      <Col>{offset}</Col>
      <Col>{value}</Col>
    </Row>
  );
}

function MemoryComponent({ offset, base }: { offset: number; base: Base }) {
  const radix = getRadix(base);

  const prevOffset = Number(offset - 1);
  const currOffset = Number(offset);
  const nextOffset = Number(offset + 1);

  const prevOffsetStr = Number(offset - 1).toString(radix);
  const currOffsetStr = Number(offset).toString(radix);
  const nextOffsetStr = Number(offset + 1).toString(radix);

  const prevValue = execute("get_memory_value", prevOffset);
  const currValue = execute("get_memory_value", currOffset);
  const nextValue = execute("get_memory_value", nextOffset);

  return (
    <>
      <Row>
        <Col>Offset</Col>
        <Col>Value</Col>
      </Row>
      <MemoryComponentRow offset={prevOffsetStr} value={prevValue} />
      <MemoryComponentRow
        offset={currOffsetStr}
        value={currValue}
        style={{ backgroundColor: "#dedede" }}
      />
      <MemoryComponentRow offset={nextOffsetStr} value={nextValue} />
    </>
  );
}

const MemoryNode = ({ data }: { data: any }) => {
  const [searchValue, setSearchValue] = React.useState(0);
  const [base, setBase] = React.useState<Base>("HEX");

  function onUIUpdate() {
    setSearchValue(execute("get_memory_dir_bus"));
  }

  React.useEffect(() => {
    subscribeToUIUpdates(onUIUpdate);
    return () => {
      unsubscribeToUIUpdates(onUIUpdate);
    };
  }, []);
  const onSearch = (num: number, base: Base) => {
    setSearchValue(num);
    setBase(base);
  };

  const onBaseChange = (base: Base) => {
    setBase(base);
  };

  return (
    <div
      style={{
        height: data.height,
        width: data.width,
        border: "1px solid black",
        backgroundColor: "white",
      }}
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
          <NumberBaseInput
            initialBase={base}
            number={searchValue}
            onChange={onSearch}
            onBaseChange={onBaseChange}
            width={200}
          />
        </Col>
      </Row>
      <Row>
        <Col size="100%">
          <MemoryComponent offset={searchValue} base={base} />
        </Col>
      </Row>
    </div>
  );
};

export default MemoryNode;
