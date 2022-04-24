import IconButton from "../../../components/IconButton";
import {
  PlaySquareOutlined,
  PlayCircleOutlined,
  PlayCircleTwoTone,
  StopFilled,
} from "@ant-design/icons";
import { execute } from "../../../lib/core";
import { sleep } from "../../../lib/utils";
import { SettingDefaultValue, SettingType } from "./Settings";
import { useState, useEffect, useRef } from "react";
import { getStoredValue } from "../../../lib/storage";
import { Space } from "antd";

export default function RunButtons() {
  const [running, setRunning] = useState(false);
  const _running = useRef<boolean>();
  const isBreak = useRef<AbortController>(new AbortController());

  function handleRunState() {
    setRunning(true);
    execute("run_clock_cycle");
    setRunning(false);
  }

  async function handleRunInstruction() {
    const cycleTime = getStoredValue(SettingType.CYCLE_TIME, SettingDefaultValue.CYCLE_TIME);
    _running.current = true;
    setRunning(true);
    while (_running.current) {
      execute("run_clock_cycle");
      const nextState = execute("get_next_state");
      if (nextState === 0) break;
      await sleep(
        cycleTime,
        isBreak.current.signal
      );
    }
    setRunning(false);
    _running.current = false;
  }

  async function handleRunProgram() {
    const cycleTime = getStoredValue(SettingType.CYCLE_TIME, SettingDefaultValue.CYCLE_TIME);
    _running.current = true;
    setRunning(true);
    const maxRepresentableValue =
      Math.pow(2, execute("get_memory_value_size_bits")) - 1;
    while (_running.current) {
      execute("run_clock_cycle");
      const riRegister = execute("get_register_ri");
      if (riRegister === maxRepresentableValue) break;
      await sleep(
        cycleTime,
        isBreak.current.signal
      );
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

  if (running) {
    return (
      <IconButton
        title="Stop"
        icon={<StopFilled />}
        onClick={handleStopRunning}
      />
    );
  }
  return (
    <Space direction="vertical" size="middle">
      <IconButton
        icon={<PlaySquareOutlined />}
        title="Run state"
        onClick={handleRunState}
        disabled={running}
      />
      <IconButton
        icon={<PlayCircleOutlined />}
        title="Run instruction"
        onClick={handleRunInstruction}
        disabled={running}
      />

      <IconButton
        icon={<PlayCircleTwoTone />}
        title="Run program"
        onClick={handleRunProgram}
        disabled={running}
      />
    </Space>
  );
}
