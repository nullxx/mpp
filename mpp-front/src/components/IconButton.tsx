import { Button } from "antd";

export default function IconButton({
  icon,
  title,
  onClick,
}: {
  icon: React.ReactNode;
  title: string;
  onClick: () => void;
}) {
  return (
    <Button type="primary" icon={icon} size="large" onClick={onClick}>
      {title}
    </Button>
  );
}
