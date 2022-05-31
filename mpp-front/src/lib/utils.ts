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
