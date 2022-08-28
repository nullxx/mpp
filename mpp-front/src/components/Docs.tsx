import { Button, Tooltip } from "antd";
import { BookTwoTone } from "@ant-design/icons";

import packageJSON from "../../package.json";
import I18n from "./i18n";

export default function Docs() {
  function handleClick() {
    window.open(packageJSON.repository.documentation, "_blank");
  }

  return (
    <Tooltip title={<I18n k="words.documentation" />}>
      <Button
        shape="circle"
        icon={<BookTwoTone />}
        size="large"
        onClick={handleClick}
      />
    </Tooltip>
  );
}
