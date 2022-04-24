import { Button } from "antd";

export default function IconButton({
  icon,
  title,
  onClick,
  disabled,
}: {
  icon: React.ReactNode;
  title?: string;
  onClick?: () => void;
  disabled?: boolean;
}) {
  return (
    <Button type="primary" icon={icon} size="large" onClick={onClick} block disabled={disabled}>
      {title}
    </Button>
  );
}
