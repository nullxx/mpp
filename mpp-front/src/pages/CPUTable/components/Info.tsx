import React, { useState } from "react";
import { Drawer, Button, Space, Divider } from "antd";
import { InfoCircleOutlined } from "@ant-design/icons";

import Changelog from "./Changelog";
import ThirdPartyAttribution from "../../../components/ThirdPartyAttribution";
import Technologies from "../../../components/Technologies";

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
          <Divider style={{margin: 0}}>About</Divider>
          <p>
            This is a simulator for the machine ++. It is developed by{" "}
            <a href="https://nullx.me" target="_blank" rel="noreferrer">
              nullx
            </a>{" "}
            but it was originally designed on the 90's by José Mª Angulo
            Usategui, Javier García Zubía &amp; Ignacio Angulo Martínez.
          </p>
          <Divider>Technologies</Divider>
          <Technologies />
          <Divider>Third party attribution</Divider>
          <ThirdPartyAttribution />
          <Divider>Changelog</Divider>
          <Changelog />
        </Space>
      </Drawer>
    </>
  );
};

export default Info;
