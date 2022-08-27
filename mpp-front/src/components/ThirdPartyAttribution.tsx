import React from "react";
import { List, Avatar } from "antd";
import I18n from "./i18n";

const data = [
  {
    titleKey: "info.thirdParty.antDesign",
    descriptionKey: "info.thirdParty.antDesign.description",
    img: "https://gw.alipayobjects.com/zos/rmsportal/KDpgvguMpGfqaHPjicRK.svg",
    url: "https://ant.design",
  },
  {
    titleKey: "info.thirdParty.reactFlow",
    descriptionKey: "info.thirdParty.reactFlow.description",
    img: "https://reactflow.dev/img/logo.svg",
    url: "https://reactflow.dev/",
  },
  {
    titleKey: "info.thirdParty.atomizeReact",
    descriptionKey: "info.thirdParty.atomizeReact.description",
    img: "https://www.saashub.com/images/app/service_logos/66/c18fdeed76cf/large.png?1639983720",
    url: "https://atomizecode.com/",
  },
];

export default function ThirdPartyAttribution() {
  return (
    <List
      itemLayout="horizontal"
      dataSource={data}
      renderItem={(item) => (
        <List.Item>
          <List.Item.Meta
            avatar={<Avatar shape="square" src={item.img} />}
            title={
              <a href={item.url} target="_blank" rel="noreferrer">
                <I18n k={item.titleKey} />
              </a>
            }
            description={<I18n k={item.descriptionKey} />}
          />
        </List.Item>
      )}
    />
  );
}
