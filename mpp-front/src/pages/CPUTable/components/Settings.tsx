import React, { useState } from "react";
import { Drawer, Button, Input, Row, Col, Select, Divider } from "antd";
import { SettingFilled } from "@ant-design/icons";
import { setStoredValue, getStoredValue } from "../../../lib/storage";
import { bases } from "../../../constants/bases";

const { Option } = Select;

export enum SettingType {
  CYCLE_TIME = "settings::cycleTime",
  MEM_VALUE_BASE = "settings::memValueBase",
}

export enum SettingDefaultValue {
  CYCLE_TIME = 500,
  MEM_VALUE_BASE = "HEX",
}

const Settings: React.FC = () => {
  const [visible, setVisible] = useState(false);
  const [cycleTime, setCycleTime] = useState(
    getStoredValue(SettingType.CYCLE_TIME, SettingDefaultValue.CYCLE_TIME)
  );
  const [memValueBase, setMemValueBase] = useState(
    getStoredValue(
      SettingType.MEM_VALUE_BASE,
      SettingDefaultValue.MEM_VALUE_BASE
    )
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

  const handleMemValueBaseChange = (selected: string) => {
    setStoredValue(SettingType.MEM_VALUE_BASE, selected, setMemValueBase);
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
                type="number"
                max={5000}
                min={0}
                onChange={handleCycleTimeChange}
              />
            </Col>
          </Row>
        </Input.Group>
        
        <Divider />

        <Input.Group size="large">
          <Row gutter={8}>
          <Col span={24}>
              <span className="ant-input-group-wrapper">
                <span className="ant-input-wrapper ant-input-group">
                  <span className="ant-input-group-addon">Memory value base</span>
                  <Select
                    value={memValueBase}
                    onChange={handleMemValueBaseChange}
                  >
                    {bases.map(({ base, radix }) => (
                      <Option key={base} value={base}>
                        {base}
                        <sub>({radix})</sub>
                      </Option>
                    ))}
                  </Select>
                </span>
              </span>
            </Col>
          </Row>
        </Input.Group>
      </Drawer>
    </>
  );
};

export default Settings;
