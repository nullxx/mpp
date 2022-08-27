import React from "react";
import { List, Avatar } from "antd";
import I18n from "./i18n";

const data = [
  {
    titleKey: "info.tecnologies.react",
    descriptionKey: "info.tecnologies.react.description",
    img: "https://miro.medium.com/max/500/1*cPh7ujRIfcHAy4kW2ADGOw.png",
    url: "https://reactjs.org/",
  },
  {
    titleKey: "info.tecnologies.webassembly",
    descriptionKey: "info.tecnologies.webassembly.description",
    img: "https://upload.wikimedia.org/wikipedia/commons/thumb/1/1f/WebAssembly_Logo.svg/2048px-WebAssembly_Logo.svg.png",
    url: "https://webassembly.org/",
  },
  {
    titleKey: "info.tecnologies.c",
    descriptionKey: "info.tecnologies.c.description",
    img: "https://upload.wikimedia.org/wikipedia/commons/thumb/1/18/C_Programming_Language.svg/695px-C_Programming_Language.svg.png",
    url: "https://en.wikipedia.org/wiki/C_(programming_language)",
  },
];

export default function Technologies() {
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
