import React from "react";
import { execute, unsubscribeToUIUpdates } from "../../../../lib/core";
import { Row, Col, Text } from "atomize";
import { Alert, Tooltip } from "antd";

import NumberBaseInput, { Base } from "../../../../components/NumberBaseInput";
import { subscribeToUIUpdates } from "../../../../lib/core/index";
import { deductOperationOf } from "../../../../lib/debugger";
import { useForceUpdate } from "../../../../hook/forceUpdate";

function DebuggerComponentRow({
  range,
  operation,
  focus,
}: {
  range: [number, number];
  operation: string;
  focus: boolean;
}) {
  return (
    <Row className={focus ? "debuggerPointed" : undefined}>
      <Col>{range[0].toString(16) + "-" + range[1].toString(16)}</Col>
      <Col className={focus ? "debuggerPointedOp" : undefined}>{operation}</Col>
    </Row>
  );
}

function DebuggerComponent({
  memOffset,
  offsetPadding,
}: {
  memOffset: number;
  offsetPadding: number;
  base: Base;
}) {
  const pointerRef = React.useRef<HTMLDivElement>(null);
  const listScrollRef = React.useRef<HTMLDivElement>(null);

  const result = deductOperationOf(
    memOffset - offsetPadding,
    memOffset + offsetPadding
  );

  const pointIndex = result.findIndex(
    ({ range: [start, end] }) => start <= memOffset && end >= memOffset
  );

  React.useEffect(() => {
    if (!pointerRef.current || !listScrollRef.current) return;

    const pointer = pointerRef.current;
    const listScroll = listScrollRef.current;

    const listScrollHeight = listScroll.clientHeight;

    const pointerTop = pointer.offsetTop;
    const pointerHeight = pointer.clientHeight;

    const pointerBottom = pointerTop + pointerHeight;

    // listScroll.scrollTop = pointerBottom - listScrollHeight; // maybe is better without smooth
    listScroll.scrollTo({
      top: pointerBottom - listScrollHeight,
      behavior: "smooth",
    });
  }, [memOffset]);

  return (
    <>
      <Row>
        <Col>Memory range</Col>
        <Col>Operation</Col>
      </Row>
      {pointIndex === -1 && (
        <Row>
          <Col size="100%">
            <Alert
              message={`No operation in ${memOffset.toString(16)}`}
              type="warning"
            />
          </Col>
        </Row>
      )}
      <div style={{ overflow: "hidden", maxHeight: 200 }} ref={listScrollRef}>
        {result.map((item, i) => {
          return (
            <div
              ref={pointIndex === i ? pointerRef : undefined}
              key={item.range[0] + " " + item.range[1]}
            >
              <DebuggerComponentRow
                range={item.range}
                operation={item.operation}
                focus={pointIndex === i}
              />
            </div>
          );
        })}
      </div>
    </>
  );
}

const DebuggerNode = ({ data }: { data: any }) => {
  const [searchValue, setSearchValue] = React.useState(0);
  const [offsetPadding, setOffsetPadding] = React.useState(10);
  const [base, setBase] = React.useState<Base>("DEC");

  const forceUpdate = useForceUpdate();

  function onUIUpdate() {
    setSearchValue(execute("get_register_pc"));
    forceUpdate(); // need to update state to force re-render because the searchValue is not changed, but the DebuggerComponent could be changed
  }

  React.useEffect(() => {
    subscribeToUIUpdates(onUIUpdate);
    return () => {
      unsubscribeToUIUpdates(onUIUpdate);
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  const onSearch = (num: number, base: Base) => {
    setOffsetPadding(num);
    setBase(base);
  };

  const onBaseChange = (base: Base) => {
    setBase(base);
  };

  return (
    <div
      style={{
        paddingLeft: 20,
        paddingRight: 20,
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
          <Tooltip title="Bounds padding">
            <div>
              <NumberBaseInput
                initialBase={base}
                number={offsetPadding}
                onChange={onSearch}
                onBaseChange={onBaseChange}
                width={200}
              />
            </div>
          </Tooltip>
        </Col>
      </Row>
      <Row>
        <Col size="100%">
          <DebuggerComponent
            memOffset={searchValue}
            offsetPadding={offsetPadding}
            base={base}
          />
        </Col>
      </Row>
    </div>
  );
};

export default DebuggerNode;
