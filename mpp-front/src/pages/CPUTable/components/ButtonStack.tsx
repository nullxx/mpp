import Coder from "../../Coder";
import RunButtons from "./RunButtons";
import { Space, Divider } from "antd";
import I18n from "../../../components/i18n";

export default function ButtonStack() {
  return (
    <Space direction="vertical" size="middle" style={{ padding: 20 }}>
      <Divider style={{ margin: 0 }}><I18n k="words.actions" /></Divider>
      <RunButtons />
      <Divider style={{ margin: 0 }}><I18n k="words.coding" /></Divider>
      <Coder />
    </Space>
  );
}
