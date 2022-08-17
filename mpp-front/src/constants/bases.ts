export type Base = "DEC" | "HEX" | "BIN";

export const bases: { base: Base; radix: number; regex: RegExp }[] = [
  {
    base: "DEC",
    radix: 10,
    regex: /^\d+\.?\d*$/,
  },
  {
    base: "HEX",
    radix: 16,
    regex: /^[a-fA-F0-9]+$/,
  },
  {
    base: "BIN",
    radix: 2,
    regex: /^[0-1]{1,}$/,
  },
];
