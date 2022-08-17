import { Checkbox, Tooltip } from "antd";
import { Row, Col, Text } from "atomize";
import { useEffect, useState } from "react";
import {
  execute,
  subscribeToUIUpdates,
  unsubscribeToUIUpdates,
} from "../../../lib/core";

export default function FlagsNode({ data }: { data: any }) {
  const [fz, setFz] = useState(false);
  const [fc, setFc] = useState(false);
  const [changed, setChanged] = useState(false);

  function onUIUpdate() {
    const fz = Boolean(execute<number>("get_register_fz"));
    const fc = Boolean(execute<number>("get_register_fc"));

    let hasChanged = false;
    setFz((prevFz) => {
      if (prevFz !== fz) {
        hasChanged = true;
        return fz;
      }
      return prevFz;
    });
    setFc((prevFc) => {
      if (prevFc !== fc) {
        hasChanged = true;
        return fc;
      }
      return prevFc;
    });

    setChanged(hasChanged);
  }

  useEffect(() => {
    subscribeToUIUpdates(onUIUpdate);
    return () => {
      unsubscribeToUIUpdates(onUIUpdate);
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  return (
    <div
      style={{
        paddingLeft: 20,
        paddingRight: 20,
        paddingTop: 10,
        paddingBottom: 10,
      }}
      className={`${changed ? "blob" : ""} pretty-shadow`}
    >
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
        <Tooltip title={data.fzHelpInfo}>
          <Checkbox checked={fz}>FZ</Checkbox>
        </Tooltip>
      </Row>
      <Row>
        <Tooltip title={data.fcHelpInfo}>
          <Checkbox checked={fc}>FC</Checkbox>
        </Tooltip>
      </Row>
    </div>
  );
}
