import FullscreenOutlined from "@ant-design/icons/FullscreenOutlined";
import FullscreenExitOutlined from "@ant-design/icons/FullscreenExitOutlined";
import { useState, useEffect } from "react";

interface ResizeDrawerProps {
  onResize: (maximize: boolean) => void;
}

export default function ResizeDrawer({ onResize }: ResizeDrawerProps) {
  const [maximized, setMaximized] = useState(false);

  useEffect(() => {
    if (onResize) {
      onResize(maximized);
    }
  }, [maximized, onResize]);

  if (!maximized) {
    return (
      <FullscreenOutlined
        onClick={() => setMaximized(true)}
        style={{ fontSize: "25px", color: "#08c" }}
      />
    );
  }

  return (
    <FullscreenExitOutlined
      onClick={() => setMaximized(false)}
      style={{ fontSize: "25px", color: "#08c" }}
    />
  );
}
