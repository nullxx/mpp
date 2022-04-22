import React, { memo } from "react";
import { execute } from "../../../lib/core";
import { Row, Col, Input, Text } from "atomize";
import {
  InfiniteTable,
  DataSource,
  InfiniteTableColumn,
} from "@infinite-table/infinite-react";

export type MemoryValue = {
  offset: number;
  value: number;
};

export const columns: Record<string, InfiniteTableColumn<MemoryValue>> = {
  offset: {
    field: "offset",
    header: "Offset",
  },
  value: {
    field: "value",
    header: "Value",
  },
};

const getMemoryData = (): MemoryValue[] => {
  const data: MemoryValue[] = [];
  const memSize = execute("get_memory_size");
  for (let i = 0; i < memSize; i++) {
    data.push({
      offset: i,
      value: execute("get_memory_value", i),
    });
  }
  return data;
};


export default memo(({ data, isConnectable }: any) => {
  const [searchValue, setSearchValue] = React.useState(5);
  const [dataSource, setDataSource] = React.useState<MemoryValue[]>([]);

  React.useEffect(() => {
    setDataSource(getMemoryData());
  }, []);

  const onSearch = (e: any) => {
    setSearchValue(Number(e.target.value));
  };

  return (
    <div
      style={{
        height: data.height,
        width: data.width,
      }}
    >
      <Row>
        <Col size="100%">
          <Text tag="h4" textSize="display4">
            {data.label}
          </Text>
        </Col>
      </Row>
      <Row>
        <Col size="100%">
          <Input placeholder="Enter a value" onChange={onSearch} />
        </Col>
      </Row>
      <Row>
        <Col size="100%">
          <DataSource<MemoryValue> data={dataSource} primaryKey="offset">
            <InfiniteTable<MemoryValue> columns={columns} activeIndex={searchValue} />
          </DataSource>
        </Col>
      </Row>
    </div>
  );
});
