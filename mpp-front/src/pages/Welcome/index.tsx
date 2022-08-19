import { Spin, Space, Typography } from "antd";
import Image from "../../assets/icon.png";
import Attribution from "../../components/Attribution";

export default function Welcome() {
  return (
    <Space
      direction="vertical"
      align="center"
      style={{
        width: "100%",
        height: "100%",
        backgroundColor: "white",
        position: "absolute",
        left: 0,
        right: 0,
        top: 0,
        bottom: 0,
        margin: "auto",
        zIndex: 1001,
        alignItems: "center",
        justifyContent: "center",
      }}
    >
      <Typography.Title>M++ Simulator</Typography.Title>

      <img src={Image} alt="M++" height={200} />
      <p>Loading...</p>
      <Spin size="large" />

      <Attribution />
    </Space>
  );
}
