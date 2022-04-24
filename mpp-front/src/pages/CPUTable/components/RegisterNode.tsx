import React, { memo } from "react";
import { Text, Row, Col } from "atomize";
import NumberBaseInput from "../../../components/NumberBaseInput";
import {
  execute,
  subscribeToUIUpdates,
  unsubscribeToUIUpdates,
} from "../../../lib/core/index";

export const DEFAULT_REGISTER_VALUE = 0;

export default memo(({ data, isConnectable }: any) => {
  const [value, setValue] = React.useState(DEFAULT_REGISTER_VALUE);
  const [changed, setChanged] = React.useState(false);

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
      className={
        changed ? "rotating-border rotating-border--google" : "bordered"
      }
      style={{ borderRadius: 3, padding: 5 }}
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
            width={153}
            readOnly
          />
        </Col>
      </Row>
    </div>
  );
});
