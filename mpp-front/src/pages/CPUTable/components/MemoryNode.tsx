import React from "react";
import { execute, unsubscribeToUIUpdates } from "../../../lib/core";
import { Row, Col, Text } from "atomize";

import NumberBaseInput, { getRadix } from "../../../components/NumberBaseInput";
import { Base } from "../../../constants/bases";

import { subscribeToUIUpdates } from "../../../lib/core/index";
import { useForceUpdate } from "../../../hook/forceUpdate";
import { Tooltip } from "antd";
import { getStoredValue } from "../../../lib/storage";
import { SettingType, SettingDefaultValue } from "./Settings";
import usePrev from "../../../hook/usePrev";
import { Handle, Position } from "react-flow-renderer";
import useUpdateEdges from "../../../hook/useUpdateEdges";

function MemoryComponentRow({
  offset,
  value,
  style,
  valueBaseRadix,
  hasChanged,
}: {
  offset: string;
  value: string | number;
  style?: React.CSSProperties;
  valueBaseRadix: number;
  hasChanged: boolean;
}) {
  if (Number(value) === -1) return null;

  return (
    <Row style={style}>
      <Col>{offset}</Col>
      <Col style={{ animation: hasChanged ? "wiggle 2s linear 1" : undefined }}>
        <span>
          {Number(value).toString(valueBaseRadix).toUpperCase()}
          <sub>({valueBaseRadix})</sub>
        </span>
      </Col>
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

  const pPrevOffset = usePrev(prevOffset);
  const pCurrOffset = usePrev(currOffset);
  const pNextOffset = usePrev(nextOffset);
  const pPrevValue = usePrev(prevValue);
  const pCurrValue = usePrev(currValue);
  const pNextValue = usePrev(nextValue);

  const valueBaseRadix = getRadix(
    getStoredValue(
      SettingType.MEM_VALUE_BASE,
      SettingDefaultValue.MEM_VALUE_BASE
    )
  );

  return (
    <div style={{ overflow: "hidden" }}>
      <Row>
        <Col>Offset</Col>
        <Col>Value</Col>
      </Row>
      <MemoryComponentRow
        offset={prevOffsetStr}
        value={prevValue}
        valueBaseRadix={valueBaseRadix}
        hasChanged={pPrevOffset === prevOffset && pPrevValue !== prevValue}
      />
      <MemoryComponentRow
        offset={currOffsetStr}
        value={currValue}
        style={{ backgroundColor: "#f0c40094" }}
        valueBaseRadix={valueBaseRadix}
        hasChanged={pCurrOffset === currOffset && pCurrValue !== currValue}
      />
      <MemoryComponentRow
        offset={nextOffsetStr}
        value={nextValue}
        valueBaseRadix={valueBaseRadix}
        hasChanged={pNextOffset === nextOffset && pNextValue !== nextValue}
      />
    </div>
  );
}

const MemoryNode = ({ data, id }: { data: any; id: string }) => {
  const [searchValue, setSearchValue] = React.useState(0);
  const [base, setBase] = React.useState<Base>("HEX");

  const forceUpdate = useForceUpdate();
  useUpdateEdges({ data, id });

  function onUIUpdate() {
    setSearchValue(execute("get_memory_dir_bus"));
    forceUpdate(); // need to update state to force re-render because the searchValue is not changed, but the MemoryComponent could be changed
  }

  React.useEffect(() => {
    subscribeToUIUpdates(onUIUpdate);
    return () => {
      unsubscribeToUIUpdates(onUIUpdate);
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
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
        padding: 10,
        backgroundColor: "#f5f5f5",
      }}
      className="pretty-shadow"
    >
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
      <Row>
        <Col size="100%">
          <Tooltip title={data.helpInfo}>
            <Text tag="h4" textSize="display4">
              {data.label}
            </Text>
          </Tooltip>
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
