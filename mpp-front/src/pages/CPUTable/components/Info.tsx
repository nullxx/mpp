import React, { useState } from "react";
import { Drawer, Button, Space, Divider } from "antd";
import { InfoCircleOutlined } from "@ant-design/icons";

import Changelog from "./Changelog";

const Info: React.FC = () => {
  const [visible, setVisible] = useState(false);

  const showDrawer = () => {
    setVisible(true);
  };

  const onClose = () => {
    setVisible(false);
  };

  return (
    <>
      <Button
        type="default"
        onClick={showDrawer}
        icon={<InfoCircleOutlined />}
      />
      <Drawer
        title="Info"
        placement="right"
        onClose={onClose}
        visible={visible}
      >
        <Space direction="vertical">
          <Divider>About</Divider>
          <p>
            Mpp is a simulator for the machine ++. This was developed by{" "}
            <a href="https://nullx.me" target="_blank" rel="noreferrer">
              nullx
            </a>{" "}
            but it was designed originally on the 90's by José Mª Angulo
            Usategui, Javier García Zubía &amp; Ignacio Angulo Martínez.
          </p>
          <Divider>Changelog</Divider>
          <Changelog />
        </Space>
      </Drawer>
    </>
  );
};

export default Info;
