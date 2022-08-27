import React, { useState } from "react";
import { Drawer, Button, Space, Divider } from "antd";
import { InfoCircleOutlined } from "@ant-design/icons";

import Changelog from "./Changelog";
import ThirdPartyAttribution from "../../../components/ThirdPartyAttribution";
import Technologies from "../../../components/Technologies";
import I18n, { useI18n } from "../../../components/i18n";

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
        title={<I18n k="info.title" />}
        placement="right"
        onClose={onClose}
        visible={visible}
      >
        <Space direction="vertical">
          <Divider style={{margin: 0}}><I18n k="info.about.title" /></Divider>
          <p dangerouslySetInnerHTML={{ __html: useI18n('info.about.description')}} />
          <Divider><I18n k="info.technologies.title" /></Divider>
          <Technologies />
          <Divider><I18n k="info.thirdParty.title" /></Divider>
          <ThirdPartyAttribution />
          <Divider><I18n k="info.changelog.title" /></Divider>
          <Changelog />
        </Space>
      </Drawer>
    </>
  );
};

export default Info;
