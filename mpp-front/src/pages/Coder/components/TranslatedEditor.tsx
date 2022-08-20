import AceEditor from "react-ace";

interface TranslatedEditorProps {
  traslated: string;
  initOffset: number;
  width?: string;
  height?: string;
}

export default function TranslatedEditor({
  traslated,
  initOffset,
  width = "unset",
  height = "200px",
}: TranslatedEditorProps) {
  return (
    <AceEditor
      value={traslated}
      name="traslated"
      editorProps={{ $blockScrolling: true }}
      setOptions={{
        showLineNumbers: true,
        firstLineNumber: initOffset,
        readOnly: true,
        fontSize: "14px",
      }}
      height={height}
      width={width}
      mode="text"
    />
  );
}
