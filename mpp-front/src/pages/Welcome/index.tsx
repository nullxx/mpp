import { Spin, Space } from "antd";

export default function Welcome() {
  return (
    <Space direction="vertical" align="center" style={{ width: "100%" }}>
      <h1>M++ simulator</h1>

      <p>Loading...</p>
      <Spin size="large" />
    </Space>
  );
}
