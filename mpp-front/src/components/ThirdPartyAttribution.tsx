import React from "react";
import { List, Avatar } from "antd";

const data = [
  {
    title: "Ant Design",
    description:
      "Ant Design is a React UI library that are useful for building elegant user interfaces.",
    img: "https://gw.alipayobjects.com/zos/rmsportal/KDpgvguMpGfqaHPjicRK.svg",
    url: "https://ant.design",
  },
  {
    title: "React Flow",
    description:
      "React Flow is highly customizable library for building interactive node-based UIs, editors, flow charts and diagrams.",
    img: "https://reactflow.dev/img/logo.svg",
    url: "https://reactflow.dev/",
  },
  {
    title: "Atomize React",
    description:
      "Atomize React is a UI framework that helps developers collaborate with designers and build consistent user interfaces effortlessly.",
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
            title={<a href={item.url}>{item.title}</a>}
            description={item.description}
          />
        </List.Item>
      )}
    />
  );
}
