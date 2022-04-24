import { Spin, Space } from "antd";
import Image from "../../assets/icon.png";

export default function Welcome() {
  return (
    <Space
      direction="vertical"
      align="center"
      style={{
        width: "100%",
        position: "absolute",
        transform: "translate(-50%, -50%)",
        top: "50%",
        left: "50%",
      }}
    >
      <h1>M++ simulator</h1>

      <img src={Image} alt="M++" height={200} />
      <p>Loading...</p>
      <Spin size="large" />
    </Space>
  );
}
