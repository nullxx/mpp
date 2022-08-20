import AceEditor, { IMarker } from "react-ace";
import "brace/ext/language_tools"; // after import react-ace
import MppMode from "../MppMode";
import completer from "../Completer";

import React, { useEffect } from "react";
import { EtiquetaPos, parseInput } from "../../../lib/traslator";
import type { TraslationError } from "../../../lib/traslator/index";
import NumberBaseInput from "../../../components/NumberBaseInput";
import { getCore } from "../../../lib/core/index";
import { Button, Popover, Space, Collapse } from "antd";
import Examples from "./Examples";
import { setStoredValue, getStoredValue } from "../../../lib/storage";
import { Text } from "atomize";
import { useTextFile } from "../../../lib/utils";
import toast from "react-hot-toast";
import TranslatedEditor from "./TranslatedEditor";
import { Annotation } from "brace";

const { Panel } = Collapse;

export default function CodeEditor({
  onNewTranslation,
  onNewOffset,
  initialCode,
  maximized = false,
}: {
  onNewTranslation: (lines: string[] | null) => void;
  onNewOffset: (offset: number) => void;
  initialCode?: string;
  maximized?: boolean;
}) {
  const [code, setCode] = React.useState<string>(
    initialCode || getStoredValue("code", "")
  );
  const [traslated, setTraslated] = React.useState("");
  const [initOffset, setInitOffset] = React.useState(0);
  const [error, setError] = React.useState<TraslationError[]>([]);
  const [offsetValid, setOffsetValid] = React.useState<boolean>(true);
  const [etiPositions, setEtiPositions] = React.useState<EtiquetaPos[]>([]);
  const [annotations, setAnnotations] = React.useState<Annotation[]>([]);

  const aceEditorRef = React.createRef<AceEditor>();

  const { downloadFile, openFile } = useTextFile();

  useEffect(() => {
    setStoredValue("code", code);

    setTraslated("");
    setError([]);

    const res = parseInput(code, 0);

    const str = res.results.map((r) => r.result.join("\n")).join("\n");
    setTraslated(str);
    setEtiPositions(res.etiPositions);
    setAnnotations(res.results.map((r) => ({
      row: r.originalOffset,
      column: 0,
      type: "info",
      text: r.result.join("\n"),
    })));

    onNewTranslation(
      res.errors.length > 0 || str.length === 0 ? null : str.split("\n")
    );

    if (res.errors.length > 0) {
      setError(res.errors);
    }

    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [code]);

  useEffect(() => {
    if (!aceEditorRef.current) return;
    aceEditorRef.current.editor.getSession().setMode(new MppMode() as any);
    aceEditorRef.current.editor.completers = [completer];
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  useEffect(() => {
    setTimeout(() => {
      if (!aceEditorRef.current) return;

      // every render, we resize the editor to fit the content
      aceEditorRef.current.editor.resize();
    }, 300); // wait for the drawer to expand
  });

  function onChange(newValue: string) {
    setCode(newValue);
  }

  function onChangeInitOffset(newValue: number) {
    const maxOffset = getCore().get_memory_size() - 1;
    let offsetValid = true;

    if (newValue < 0 || newValue > maxOffset) {
      offsetValid = false;
    }

    setOffsetValid(offsetValid);
    setInitOffset(newValue);

    if (offsetValid) onNewOffset(newValue);
  }

  function handlePCChange() {
    getCore().set_register_pc(initOffset);
  }

  function handleSelectExample(code: string) {
    setCode(code);
  }

  function handleDownloadCode() {
    toast.success("Downloading code...");
    // to avoid clousure
    setCode((code) => {
      downloadFile("code.mpp.txt", code);
      return code;
    });
  }

  async function handleOpenCode() {
    const { content, file } = await openFile();
    setCode(content);
    toast.success("Code loaded from " + file.name);
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

  const markers = [...hightlightEtiMarkers, ...errorMarkers];
  // const annotations = error.map((e) => ({
  //   row: e.lineFrom,
  //   column: e.startCol,
  //   text: e.message,
  //   type: "error",
  // }));

  // Render editor
  return (
    <>
      <Space direction="vertical" style={{ width: "100%" }}>
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

        <Space direction="vertical" style={{ width: "100%" }}>
          <AceEditor
            onChange={onChange}
            value={code}
            name="code"
            editorProps={{ $blockScrolling: false }}
            ref={aceEditorRef}
            setOptions={{
              showLineNumbers: true,
              firstLineNumber: 0,
              fontSize: `${maximized ? "24" : "14"}px`,
              enableBasicAutocompletion: true,
              enableLiveAutocompletion: true,
            }}
            height={maximized ? undefined : "200px"}
            width="unset"
            mode="text"
            markers={markers}
            annotations={annotations}
            commands={[
              {
                name: "save",
                bindKey: { win: "Ctrl-S", mac: "Cmd-S" },
                exec: handleDownloadCode,
              },
              {
                name: "open",
                bindKey: { win: "Ctrl-O", mac: "Cmd-O" },
                exec: handleOpenCode,
              },
            ]}
          />
          {!maximized && (
            <TranslatedEditor initOffset={initOffset} traslated={traslated} />
          )}
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
