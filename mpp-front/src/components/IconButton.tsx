import { Button } from "antd";
import { animations } from "react-animation";

export default function IconButton({
  icon,
  title,
  onClick,
  disabled,
  animate = true,
}: {
  icon: React.ReactNode;
  title?: string;
  onClick?: () => void;
  disabled?: boolean;
  animate?: boolean;
  hidden?: boolean;
}) {
  return (
    <Button
      style={animate ? { animation: animations.fadeIn } : {}}
      type="primary"
      icon={icon}
      size="large"
      onClick={onClick}
      block
      disabled={disabled}
    >
      {title}
    </Button>
  );
}
