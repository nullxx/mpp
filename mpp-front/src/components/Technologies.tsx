import React from "react";
import { List, Avatar } from "antd";

const data = [
  {
    title: "React",
    description: "React is a A JavaScript library for building user interfaces",
    img: "https://miro.medium.com/max/500/1*cPh7ujRIfcHAy4kW2ADGOw.png",
    url: "https://reactjs.org/",
  },
  {
    title: "WebAssembly",
    description:
      "WebAssembly (abbreviated Wasm) is a binary instruction format for a stack-based virtual machine.",
    img: "https://upload.wikimedia.org/wikipedia/commons/thumb/1/1f/WebAssembly_Logo.svg/2048px-WebAssembly_Logo.svg.png",
    url: "https://webassembly.org/",
  },
  {
    title: "C",
    description:
      "C is a general-purpose imperative procedural computer programming language.  It was designed to be compiled to provide low-level access to memory and language constructs that map efficiently to machine instructions.",
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
            title={<a href={item.url} target="_blank" rel="noreferrer">{item.title}</a>}
            description={item.description}
          />
        </List.Item>
      )}
    />
  );
}
