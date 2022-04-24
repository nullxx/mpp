import Coder from "../../Coder";
import RunButtons from "./RunButtons";
import { Space } from "antd";

export default function ButtonStack() {
  return (
    <Space direction="vertical" size="middle" style={{ padding: 20 }}>
      <RunButtons />
      <Coder />
    </Space>
  );
}
