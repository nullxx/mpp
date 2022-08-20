import { Button } from "antd";
import { ButtonType } from "antd/lib/button";
import { animations } from "react-animation";

export default function IconButton({
  icon,
  title,
  onClick,
  disabled,
  animate = true,
  type = 'primary',
  danger
}: {
  icon: React.ReactNode;
  title?: string;
  onClick?: () => void;
  disabled?: boolean;
  animate?: boolean;
  hidden?: boolean;
  type?: ButtonType;
  danger?: boolean;
}) {
  return (
    <Button
      style={animate ? { animation: animations.fadeIn } : {}}
      type={type}
      icon={icon}
      size="large"
      onClick={onClick}
      block
      disabled={disabled}
      danger={danger}
    >
      {title}
    </Button>
  );
}
