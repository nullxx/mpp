import {
  VerticalAlignBottomOutlined,
  ArrowDownOutlined,
  StopFilled,
  SendOutlined,
} from "@ant-design/icons";
import { execute } from "../../../lib/core";
import { sleep } from "../../../lib/utils";
import { SettingDefaultValue, SettingType } from "./Settings";
import { useState, useEffect, useRef } from "react";
import { getStoredValue } from "../../../lib/storage";
import { Space, Button, Tooltip, Collapse, List } from "antd";

const { Panel } = Collapse;

export let clockCycleTime = -1;

const buttonsInfo = [
  {
    description: "Run program",
    icon: <SendOutlined />
  },
  {
    description: "Run instruction",
    icon: <ArrowDownOutlined />
  },
  {
    description: "Run state",
    icon: <VerticalAlignBottomOutlined />
  },
  {
    description: "Stop",
    icon: <StopFilled />
  },
];

function RunButtonsInfo() {
  return (
    <Collapse bordered={false}>
      <Panel header="What is this?" key="1">
        <List
          itemLayout="horizontal"
          dataSource={buttonsInfo}
          renderItem={(item) => (
            <List.Item>
              <List.Item.Meta
                avatar={item.icon}
                title={item.description}
              />
            </List.Item>
          )}
        />
      </Panel>
    </Collapse>
  );
}

export default function RunButtons() {
  const [running, setRunning] = useState(false);
  const _running = useRef<boolean>();
  const isBreak = useRef<AbortController>(new AbortController());

  function handleRunState() {
    setRunning(true);
    clockCycleTime = execute("run_clock_cycle");
    setRunning(false);
  }

  async function handleRunInstruction() {
    const cycleTime = getStoredValue(
      SettingType.CYCLE_TIME,
      SettingDefaultValue.CYCLE_TIME
    );
    _running.current = true;
    setRunning(true);
    while (_running.current) {
      clockCycleTime = execute("run_clock_cycle");
      const nextState = execute("get_next_state");
      if (nextState === 0) break;
      await sleep(cycleTime, isBreak.current.signal);
    }
    setRunning(false);
    _running.current = false;
  }

  async function handleRunProgram() {
    const cycleTime = getStoredValue(
      SettingType.CYCLE_TIME,
      SettingDefaultValue.CYCLE_TIME
    );
    _running.current = true;
    setRunning(true);
    const maxRepresentableValue =
      Math.pow(2, execute("get_memory_value_size_bits")) - 1;
    let stoping = false;
    while (_running.current) {
      clockCycleTime = execute("run_clock_cycle");
      if (stoping) break; // doing this to "execute" FF. S0 -> S1 -> (next) S0. And leave ready for next
      const riRegister = execute("get_register_ri");
      stoping = riRegister === maxRepresentableValue;
      await sleep(cycleTime, isBreak.current.signal);
    }
    setRunning(false);
    _running.current = false;
  }

  const handleStopRunning = () => {
    _running.current = false; // will stop running the next loop
    // it can be in the sleep function so we abort the sleep
    isBreak.current.abort();
    isBreak.current = new AbortController();
  };

  useEffect(() => {
    _running.current = running;
  }, [running]);

  return (
    <Space direction="vertical" size="middle">
      <Space className="runButtons">
        <Tooltip title="Run program">
          <Button
            type="primary"
            icon={<SendOutlined />}
            onClick={handleRunProgram}
            disabled={running}
            size={"middle"}
          />
        </Tooltip>

        <Tooltip title="Run instruction">
          <Button
            type="primary"
            icon={<ArrowDownOutlined />}
            onClick={handleRunInstruction}
            disabled={running}
            size={"middle"}
          />
        </Tooltip>
        <Tooltip title="Run state">
          <Button
            type="primary"
            icon={<VerticalAlignBottomOutlined />}
            onClick={handleRunState}
            disabled={running}
            size={"middle"}
          />
        </Tooltip>
        <Tooltip title="Stop">
          <Button
            type="primary"
            icon={<StopFilled />}
            onClick={handleStopRunning}
            disabled={!running}
            size={"middle"}
          />
        </Tooltip>
      </Space>
      <p style={{ fontSize: 10 }}>Status: {running ? "running" : "stopped"}</p>
      <RunButtonsInfo />
    </Space>
  );
}
