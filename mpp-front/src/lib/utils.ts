export function sleep(ms: number, signal: AbortSignal) {
  return new Promise<void>((resolve) => {
    const abort = () => {
      clearTimeout(timeout);
      resolve();
    };
    const timeout = setTimeout(() => {
      resolve();
      signal.removeEventListener("abort", abort);
    }, ms);
    if (signal.aborted) abort();
    else signal.addEventListener("abort", abort);
  });
}

export function concatenate(a: number, b: number, base = 10) {
  return a * Math.pow(base, Math.floor(Math.log(b) / Math.log(base)) + 1) + b;
}

export function useTextFile(initialContent?: string) {
  function open() {
    const input = document.createElement("input");
    input.type = "file";
    input.click();
    input.multiple = false;

    return new Promise<{ content: string; file: File }>((resolve, reject) => {
      input.onchange = async () => {
        const file = input.files && input.files[0];
        if (!file) return reject("No file selected");

        const reader = new FileReader();
        reader.onload = () => {
          resolve({ content: reader.result as string, file });
          input.remove();
        };

        reader.readAsText(file);
      };
    });
  }

  const download = (filename: string, content?: string) => {
    const element = document.createElement("a");
    const file = new Blob([initialContent ?? content ?? ""], {
      type: "text/plain",
    });
    element.href = URL.createObjectURL(file);
    element.download = filename;
    document.body.appendChild(element);
    element.click();
  };

  return { downloadFile: download, openFile: open };
}
