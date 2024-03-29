import * as React from "react";
import "./Code.css";
import hljs from "highlight.js/lib/core";
import "highlight.js/styles/monokai-sublime.css";
import operations from "./operations.json";
import Preview from "./Preview";

const allInstructions = [
  ...new Set(operations.map((s) => s.NEMO.split(" ")[0])),
].join("|");

hljs.registerLanguage("mpp", (e) => {
  const COMMENT = {
    variants: [hljs.HASH_COMMENT_MODE],
  };

  return {
    name: "MPP",
    case_insensitive: true,
    aliases: ["mpp"],
    contains: [
      {
        className: "keyword",
        begin: `\\b(?:${allInstructions})\\b`,
      },
      {
        className: "built_in",
        begin: "AC|RB|RC|RD|RE",
      },
      COMMENT,
      {
        className: "number",
        variants: [
          {
            // hex
            begin: "[#$=]?[0-9a-f]+",
          },
        ],
        relevance: 0,
      },
      {
        className: "symbol",
        variants: [
          {
            begin: "T\\S*",
          },
        ],
        relevance: 0,
      },
    ],
  };
});

export default function Code({
  code,
  testCodeText,
  previewText,
  previewVideoSrc,
  bgColor,
}: {
  code: string;
  testCodeText?: string;
  previewText?: string;
  previewVideoSrc?: string;
  bgColor?: string;
}) {
  const colored = hljs.highlight(code, { language: "mpp" }).value;

  const openCode = (code: string) => {
    // open new url in new tab
    const url = new URL("https://mpp.nullx.me");
    url.searchParams.set("code", code);
    window.open(url.href, "_blank");
  };

  return (
    <pre
      className="shiki github-dark"
      style={{ backgroundColor: bgColor ?? "#0d1117", color: "#c9d1d9", overflow: 'scroll' }}
    >
      <div className="run-code">
        {testCodeText && (
          <button onClick={() => openCode(code)}>{testCodeText}</button>
        )}
        {previewText && previewVideoSrc &&  (
          <Preview previewText={previewText} videoSrc={previewVideoSrc}  title={code} />
        )}
      </div>

      <div className="code-container">
        <code dangerouslySetInnerHTML={{ __html: colored }} />
      </div>
    </pre>
  );
}
