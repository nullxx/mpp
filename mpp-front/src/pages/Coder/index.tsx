import React, { useState } from "react";
import { Drawer, Space, Popconfirm, Divider } from "antd";
import CodeEditor from "./components/CodeEditor";
import { getCore } from "../../lib/core";
import IconButton from "../../components/IconButton";
import { CodeOutlined, SaveOutlined, CloseOutlined } from "@ant-design/icons";
import ResizeDrawer from "./components/ResizeDrawer";
import { useEffect } from "react";
import I18n, { useI18n } from "../../components/i18n";

const Coder: React.FC = () => {
  const searchParams = new URLSearchParams(window.location.search);
  const initialCode = searchParams.get("code") || "";

  const [visible, setVisible] = useState(Boolean(initialCode));
  const [canSaveToMem, setCanSaveToMem] = useState(false);
  const [initOffset, setInitOffset] = useState(0);
  const [slots, setSlots] = useState<string[]>([]);
  const [maximize, setMaximize] = useState(false);

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

  useEffect(() => {
    const maxOffset = getCore().get_memory_size() - 1;
    const s = initOffset + slots.length - 1 <= maxOffset;
    setCanSaveToMem(s);
  }, [slots, initOffset]);

  const handleResize = setMaximize;

  return (
    <>
      <Space>
        <IconButton
          title={useI18n("openEditor")}
          icon={<CodeOutlined />}
          onClick={showDefaultDrawer}
        />
      </Space>

      <Drawer
        width={maximize ? "100%" : undefined}
        closable={false}
        keyboard={false}
        maskClosable={false}
        closeIcon={null}
        title={
          <Space align="center">
            <span>Code</span>
            <Divider type="vertical" />
            <ResizeDrawer onResize={handleResize} />
          </Space>
        }
        placement="right"
        onClose={onClose}
        visible={visible}
        extra={
          <Space>
            <Popconfirm
              title={<I18n k="areYouSure" />}
              okText={<I18n k="words.yes" />}
              cancelText={<I18n k="words.no" />}
              onConfirm={onClose}
            >
              <IconButton icon={<CloseOutlined />} danger title={useI18n('words.cancel')} />
            </Popconfirm>
            <IconButton
              icon={<SaveOutlined />}
              type="primary"
              onClick={handleSaveToMemory}
              disabled={!canSaveToMem}
              title={useI18n('words.save')}
            />
          </Space>
        }
      >
        <CodeEditor
          onNewTranslation={onNewTranslation}
          onNewOffset={onNewOffset}
          initialCode={initialCode}
          maximized={maximize}
        />
      </Drawer>
    </>
  );
};

export default Coder;
