import React from "preact";
import { useState } from "preact/hooks";

import "./Preview.css";

function ClosePreview() {
  return (
    <svg
      xmlns="http://www.w3.org/2000/svg"
      x="0"
      y="0"
      enableBackground="new 0 0 26 26"
      version="1.1"
      viewBox="0 0 26 26"
      xmlSpace="preserve"
      width={26}
      height={26}
    >
      <path
        fill="white"
        d="M21.125 0H4.875A4.874 4.874 0 000 4.875v16.25A4.874 4.874 0 004.875 26h16.25A4.874 4.874 0 0026 21.125V4.875A4.874 4.874 0 0021.125 0zM18.78 17.394l-1.388 1.387a.654.654 0 01-.924 0L13 15.313 9.533 18.78a.653.653 0 01-.925-.002L7.22 17.394a.66.66 0 010-.926l3.468-3.467-3.467-3.467a.657.657 0 010-.925l1.388-1.388a.651.651 0 01.925 0L13 10.689l3.468-3.468a.65.65 0 01.924 0l1.388 1.386a.66.66 0 01.001.927l-3.468 3.467 3.468 3.467a.66.66 0 01-.001.926z"
      ></path>
    </svg>
  );
}

export default function Preview({
  previewText,
  videoSrc,
  title,
}: {
  previewText: string;
  videoSrc: string;
  title: string;
}) {
  const [open, setOpen] = useState(false);

  return (
    <>
      <button className="preview-button" onClick={() => setOpen(!open)}>
        {previewText}
      </button>

      {open && (
        <>
          <div className="modal">
            <div className="modal-header">
              <h2 className="modal-title">{title}</h2>
            </div>
            <video
              src={videoSrc}
              className="center"
              autoPlay
              loop
              muted
              playsInline
              controls
            />
            <div className="modal-close" onClick={() => setOpen(false)}>
              <ClosePreview />
            </div>
          </div>
        </>
      )}
    </>
  );
}
