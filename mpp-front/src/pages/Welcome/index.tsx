import { Spin, Space } from "antd";
import Image from "../../assets/icon.png";
import { animations } from "react-animation";

export default function Welcome() {
  return (
    <Space
      direction="vertical"
      align="center"
      style={{
        width: '100%',
        height: '100%',
        backgroundColor: 'white',
        position: 'absolute',
        left: 0,
        right: 0,
        top: 0,
        bottom: 0,
        margin: 'auto',
        zIndex: 6,
        alignItems: 'center',
        justifyContent: 'center',
        animation: animations.popIn,
        animationDuration: '100ms',
      }}
    >
      <h1>M++ simulator</h1>

      <img src={Image} alt="M++" height={200} />
      <p>Loading...</p>
      <Spin size="large" />
    </Space>
  );
}
