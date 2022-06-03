import Coder from "../../Coder";
import RunButtons from "./RunButtons";
import { Space, Divider } from "antd";

export default function ButtonStack() {
  return (
    <Space direction="vertical" size="middle" style={{ padding: 20 }}>
      <Divider style={{ margin: 0 }}>Actions</Divider>
      <RunButtons />
      <Divider style={{ margin: 0 }}>Coding</Divider>
      <Coder />
    </Space>
  );
}
