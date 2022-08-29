import { GithubFilled } from "@ant-design/icons";
import { Button } from "antd";

import packageJSON from "../../package.json";

export default function Github() {
  function handleClick() {
    window.open(packageJSON.repository.url, "_blank");
  }

  return (
    <Button
      shape="circle"
      className="onboarding-opensource"
      icon={<GithubFilled />}
      size="large"
      onClick={handleClick}
    />
  );
}
