import { List, Button } from "antd";
import examples from "../constants/examples.json";

export default function Examples({
  onSelect,
}: {
  onSelect: (code: string) => void;
}) {
  return (
    <List
      header={<h3>Examples</h3>}
      dataSource={examples}
      size="small"
      split={true}
      renderItem={(item) => (
        <List.Item key={item.title}>
          <Button onClick={() => onSelect(item.code)}>{item.title}</Button>
        </List.Item>
      )}
    />
  );
}
