import AceEditor, { IMarker } from "react-ace";

import React, { useEffect } from "react";
import { EtiquetaPos, Comment,  parseInput } from "../../../lib/traslator";
import { TraslationError } from "../../../lib/traslator/index";
import NumberBaseInput from "../../../components/NumberBaseInput";
import { execute } from "../../../lib/core/index";
import { Button, Popover, Space, Alert, Collapse } from "antd";
import "ace-builds/src-noconflict/mode-text";
import Examples from "./Examples";
import { setStoredValue, getStoredValue } from "../../../lib/storage";
import { animations } from "react-animation";
import { Text } from "atomize";

const { Panel } = Collapse;

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
  const [etiPositions, setEtiPositions] = React.useState<EtiquetaPos[]>([]);
  const [comments, setComments] = React.useState<Comment[]>([]);

  useEffect(() => {
    setStoredValue("code", code);

    setTraslated("");
    setError([]);

    const res = parseInput(code, 0);

    const str = res.results.map((r) => r.join("\n")).join("\n");
    setTraslated(str);
    setEtiPositions(res.etiPositions);
    setComments(res.comments);

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

  const errorMarkers: IMarker[] = error.map((e) => {
    return {
      startRow: e.lineFrom,
      endRow: e.lineTo,
      className: "error-marker",
      type: "text",
      startCol: e.startCol,
      endCol: e.endCol,
    };
  });

  const hightlightEtiMarkers: IMarker[] = etiPositions.map((e) => ({
    startRow: e.lineFrom,
    endRow: e.lineTo,
    className: "eti-marker",
    type: "text",
    startCol: e.startCol,
    endCol: e.endCol,
  }));

  const hightlightComments: IMarker[] = comments.map((e) => ({
    startRow: e.lineFrom,
    endRow: e.lineTo,
    className: "comment-marker",
    type: "text",
    startCol: e.startCol,
    endCol: e.endCol,
  }));

  const markers = [...hightlightEtiMarkers, ...hightlightComments, ...errorMarkers];

  const thereIsErrors = error.length > 0;

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
          <Collapse bordered={false}>
            <Panel header="How to use labels?" key="1">
              <Text>- You declare them starting with 'T':</Text>

              <pre className="code">
                <span className="eti-marker-color">T1</span> MOV FF, AC
              </pre>
              <Text>
                They will be colored in{" "}
                <span className="eti-marker-color">yellow</span>.
              </Text>
              <Text>- You can use them in the code:</Text>
              <pre className="code">JMP T1</pre>
            </Panel>
          </Collapse>
        </Space>

        {thereIsErrors && (
          <Alert
            type="error"
            style={{ animation: animations.fadeInUp }}
            banner
            message="There are errors in the code"
            description="Check them out"
          />
        )}

        <Space direction="vertical" style={{ width: "100%" }}>
          <AceEditor
            onChange={onChange}
            value={code}
            name="code"
            editorProps={{ $blockScrolling: false }}
            setOptions={{
              showLineNumbers: true,
              firstLineNumber: 0,
              fontSize: "14px",
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
        <Collapse bordered={false}>
          <Panel header="Examples" key="1">
            <Examples onSelect={handleSelectExample} />
          </Panel>
        </Collapse>
      </Space>
    </>
  );
}
