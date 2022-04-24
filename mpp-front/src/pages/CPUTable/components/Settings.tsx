import React, { useState } from "react";
import { Drawer, Button, Input, Row, Col } from "antd";
import { SettingFilled } from "@ant-design/icons";
import { setStoredValue, getStoredValue } from "../../../lib/storage";

export enum SettingType {
  CYCLE_TIME = "settings::cycleTime",
}

const Settings: React.FC = () => {
  const [visible, setVisible] = useState(false);
  const [cycleTime, setCycleTime] = useState(
    getStoredValue(SettingType.CYCLE_TIME) || 500,
  );

  const showDrawer = () => {
    setVisible(true);
  };

  const onClose = () => {
    setVisible(false);
  };

  const handleCycleTimeChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    if (e.target.valueAsNumber > Number(e.target.max)) return;
    if (e.target.valueAsNumber < Number(e.target.min)) return;

    setStoredValue(
      SettingType.CYCLE_TIME,
      e.target.valueAsNumber,
      setCycleTime
    );
  };

  return (
    <>
      <Button type="dashed" onClick={showDrawer} icon={<SettingFilled />} />
      <Drawer
        title="Settings"
        placement="right"
        onClose={onClose}
        visible={visible}
      >
        <Input.Group size="large">
          <Row gutter={8}>
            <Col span={24}>
              <Input
                addonBefore="Cycle time (ms)"
                value={cycleTime}
                type={"number"}
                max={5000}
                min={0}
                onChange={handleCycleTimeChange}
              />
            </Col>
          </Row>
        </Input.Group>
      </Drawer>
    </>
  );
};

export default Settings;
