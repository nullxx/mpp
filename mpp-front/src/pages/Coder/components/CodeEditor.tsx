import AceEditor, { IMarker } from "react-ace";

import React, { useEffect } from "react";
import { parseInput } from "../../../lib/traslator";
import { TraslationError } from "../../../lib/traslator/index";
import NumberBaseInput from "../../../components/NumberBaseInput";
import { execute } from "../../../lib/core/index";
import { Button, Popover, Space } from "antd";
import "ace-builds/src-noconflict/mode-text";
import Examples from "./Examples";
import { setStoredValue, getStoredValue } from "../../../lib/storage";

export default function CodeEditor({
  onNewTranslation,
  onNewOffset,
}: {
  onNewTranslation: (lines: string[] | null) => void;
  onNewOffset: (offset: number) => void;
}) {
  const [code, setCode] = React.useState(getStoredValue("code", ""));
  const [traslated, setTraslated] = React.useState("");
  const [initOffset, setInitOffset] = React.useState(0);
  const [error, setError] = React.useState<TraslationError[]>([]);
  const [offsetValid, setOffsetValid] = React.useState<boolean>(true);

  useEffect(() => {
    setStoredValue("code", code);

    setTraslated("");
    setError([]);

    const res = parseInput(code, 0);

    const str = res.results.map((r) => r.join("\n")).join("\n");
    setTraslated(str);

    onNewTranslation(
      res.errors.length > 0 || str.length === 0 ? null : str.split("\n")
    );

    if (res.errors.length > 0) {
      setError(res.errors);
    }

    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [code]);

  function onChange(newValue: string) {
    setCode(newValue);
  }

  function onChangeInitOffset(newValue: number) {
    const maxOffset = execute("get_memory_size") - 1;
    let offsetValid = true;

    if (newValue < 0 || newValue > maxOffset) {
      offsetValid = false;
    }

    setOffsetValid(offsetValid);
    setInitOffset(newValue);

    if (offsetValid) onNewOffset(newValue);
  }

  function handlePCChange() {
    execute("set_register_pc", initOffset);
  }

  function handleSelectExample(code: string) {
    setCode(code);
  }
  // build markers

  const markers: IMarker[] = error.map((e) => {
    return {
      startRow: e.lineFrom,
      endRow: e.lineTo,
      className: "error-marker",
      type: "text",
      startCol: 0,
      endCol: 10,
    };
  });

  // Render editor
  return (
    <>
      <Space direction="vertical">
        <Space align="center" direction="horizontal">
          <NumberBaseInput
            initialBase="HEX"
            number={initOffset}
            onChange={onChangeInitOffset}
            width={200}
            isError={!offsetValid}
          />

          <Popover
            placement="topLeft"
            title={`PC setted successfully to ${initOffset}`}
            content="The execution will start from the new offset"
            trigger="click"
          >
            <Button
              type="primary"
              onClick={handlePCChange}
              disabled={!offsetValid}
            >
              Set PC to {initOffset}
            </Button>
          </Popover>
        </Space>

        <Space direction="vertical" style={{ width: "100%" }}>
          <AceEditor
            onChange={onChange}
            value={code}
            name="code"
            editorProps={{ $blockScrolling: false }}
            setOptions={{
              showLineNumbers: true,
              firstLineNumber: 0,
            }}
            height="200px"
            width="unset"
            mode="text"
            markers={markers}
          />

          <AceEditor
            value={traslated}
            name="traslated"
            editorProps={{ $blockScrolling: true }}
            setOptions={{
              showLineNumbers: true,
              firstLineNumber: initOffset,
              readOnly: true,
            }}
            height="200px"
            width="unset"
            mode="text"
          />
        </Space>

        <Examples onSelect={handleSelectExample} />
      </Space>
    </>
  );
}
