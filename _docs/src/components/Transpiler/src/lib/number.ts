function formatHex(num: number, padStart: number) {
  return num.toString(16).padStart(padStart, "0");
}

export function formatHexValue(num: number) {
  return formatHex(num, 2); // 8 bits
}

export function formatHexDir(num: number) {
  return formatHex(num, 4); // 16 bits
}

export function parseHexString(str: string): number | null {
  const sourceIsHex = /0x(.+)/gm;
  const hexMatch = sourceIsHex.exec(str);
  if (hexMatch && hexMatch.length > 1 && hexMatch[1]) {
    const value = parseInt(hexMatch[1], 16);
    return !isNaN(value) ? value : null;
  }

  return null;
}

export function parseDecimalString(str: string): number | null {
  const sourceIsDec = /0d(.+)/gm;
  const decMatch = sourceIsDec.exec(str);
  if (decMatch && decMatch.length > 1 && decMatch[1]) {
    return Number(decMatch[1]);
  }

  return null;
}

export function parseBinString(str: string): number | null {
  const sourceIsBin = /0b(.+)/gm;
  const binMatch = sourceIsBin.exec(str);
  if (binMatch && binMatch.length > 1 && binMatch[1]) {
    const value = parseInt(binMatch[1], 2);
    return !isNaN(value) ? value : null;
  }

  return null;
}

export function parseString(str: string): number | null {
  const num =
    [parseHexString(str), parseDecimalString(str), parseBinString(str)].find(
      (e) => e !== null
    ) ?? null;
  return num;
}
