import React, { useState } from "react";
import { Drawer, Button, Space, Popconfirm } from "antd";
import CodeEditor from "./components/CodeEditor";
import { getCore } from '../../lib/core/index';
import IconButton from "../../components/IconButton";
import { CodeOutlined } from "@ant-design/icons";

const Coder: React.FC = () => {
  const searchParams = new URLSearchParams(window.location.search);
  const initialCode = searchParams.get("code") || "";

  const [visible, setVisible] = useState(Boolean(initialCode));
  const [canSaveToMem, setCanSaveToMem] = useState(false);
  const [initOffset, setInitOffset] = useState(0);
  const [slots, setSlots] = useState<string[]>([]);

  const showDefaultDrawer = () => {
    setVisible(true);
  };

  const onClose = () => {
    setVisible(false);
  };

  const handleSaveToMemory = () => {
    getCore().reset_control(); // reset the control to start from the beginning

    for (let i = 0; i < slots.length; i++) {
      const slot = slots[i];

      getCore().set_memory_value(initOffset + i, parseInt(slot, 16));
    }
    onClose();
  };

  const onNewTranslation = (lines: string[] | null) => {
    setCanSaveToMem(lines !== null);
    if (lines) {
      setSlots(lines);
    }
  };

  const onNewOffset = (offset: number) => {
    setInitOffset(offset);
  };

  return (
    <>
      <Space>
        <IconButton
          title="Open editor"
          icon={<CodeOutlined />}
          onClick={showDefaultDrawer}
        />
      </Space>

      <Drawer
        closable={false}
        keyboard={false}
        maskClosable={false}
        closeIcon={null}
        title="Code"
        placement="right"
        onClose={onClose}
        visible={visible}
        extra={
          <Space>
            <Popconfirm
              title="Are you sure?"
              okText="Yes"
              cancelText="No"
              onConfirm={onClose}
            >
              <Button>Cancel</Button>
            </Popconfirm>
            <Button
              type="primary"
              onClick={handleSaveToMemory}
              disabled={!canSaveToMem}
            >
              SAVE
            </Button>
          </Space>
        }
      >
        <CodeEditor
          onNewTranslation={onNewTranslation}
          onNewOffset={onNewOffset}
          initialCode={initialCode}
        />
      </Drawer>
    </>
  );
};

export default Coder;
