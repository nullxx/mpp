/**
 * DO NOT LOOK HERE
 * THIS IS THE MAGIC
 * IT IS NOT A GOOD WAY TO DO IT
 * IT IS HORRIBLE
 * BUT IT WORKS
 * --
 * I KNOW IT IS HORRIBLE
 * BUT IT WORKS
 * --
 * I WILL REFACTOR IT
 * SOON
 * --
 * MAYBE I WILL HAVE TO REDO IT ALL OVER AGAIN
 */

import instructions from "./operations.json";

interface Instruction {
  HEX: string;
  NEMO: string;
  REGEX: string;
  GRUPO: number;
  ALLOC: number;
}

interface Marker {
  lineFrom: number;
  lineTo: number;
  startCol: number;
  endCol: number;
}

const ETI_START_NAME = "T";
const COMMENT_LINE_START = "#";

export interface TraslationError extends Marker {
  message: string;
}
export interface Comment extends Marker {}

let setEtiquetas: string[] = [];

const checkCommentLine = (line: string) => {
  const startCommentIndex = line.indexOf(COMMENT_LINE_START);
  if (startCommentIndex > -1) {
    return line.slice(0, startCommentIndex).trimEnd();
  }
  return line;
};

const parseInput = (text: string, initDir: number) => {
  setEtiquetas = [];
  let results: Result[] = [];
  const errors: TraslationError[] = [];
  const comments: Comment[] = [];
  const lines = text
    .split("\n")
    .map((line) => line.trim())
    .map((line) => checkCommentLine(line));

  pre(lines);

  for (let i = 0; i < lines.length; i++) {
    let line = lines[i];
    if (!line) continue;

    const startCommentIndex = line.indexOf(COMMENT_LINE_START);
    if (startCommentIndex !== -1) {
      comments.push({
        lineFrom: i,
        lineTo: i,
        startCol: startCommentIndex,
        endCol: line.length,
      });
      if (startCommentIndex === 0) {
        continue;
      } else {
        line = line.slice(0, startCommentIndex).trimEnd(); // trim because of the space(s) between the code and the #
      }
    }

    const result = executeRegex(line);
    if (!result || result.result.length === 0) {
      errors.push({
        lineFrom: i,
        lineTo: i,
        startCol: 0,
        endCol: line.length,
        message: "Syntax error",
      });
      continue;
    }

    results.push({ ...result, originalOffset: i });
  }

  const etiPositions = calculateEtiquetasPos(results, initDir);
  results = post(results, initDir);

  return {
    errors,
    results: results,
    etiPositions,
    comments,
  };
};

const pre = (lines: string[]) => {
  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    for (let j = 0; j < instructions.length; j++) {
      const instruction = instructions[j];

      const words = line.split(" ");
      const posibleSetEti = words[0];
      const line2 = words.slice(1, words.length);
      const result = new RegExp(instruction.REGEX, "gim").exec(line2.join(" "));

      if (result && result.length > 1) {
        setEtiquetas.push(posibleSetEti);
      }
    }
  }
};

const post = (results: Result[], initDir: number) => {
  const etiPositions = calculateEtiquetasPos(results, initDir);

  for (let i = 0; i < results.length; i++) {
    const result = results[i];
    if (!result.result) continue;

    for (let j = 1; j < result.result.length; j++) {
      let r = result.result[j];
      const eti = etiPositions.find((s) => s.eti.eti === r);
      if (eti) {
        result.result.splice(j, 1);
        const etiDirHex = Number(eti.dir)
          .toString(16)
          .toUpperCase()
          .padStart(4, "0");

        for (let i = 0; i < etiDirHex.length; i += 2) {
          result.result.push(etiDirHex.slice(i, i + 2));
        }
      } else if (!checkIsHex(r)) {
        // is not !hex && !eti => error
        results.splice(i, 1);
      }
    }
  }
  return results;
};
interface RegexResponse {
  line: string;
  result: string[];
  eti: string;
  instruction: Instruction;
}

type Result = RegexResponse & { originalOffset: number };

const executeRegex = (line: string): RegexResponse | null => {
  for (let i = 0; i < instructions.length; i++) {
    const instruction = instructions[i] as Instruction;
    let eti = "";
    let result = new RegExp(instruction.REGEX, "gim").exec(line);
    if (!result) {
      const words = line.split(" ");
      const posibleSetEti = words[0];
      const line2 = words.slice(1, words.length);
      result = new RegExp(instruction.REGEX, "gim").exec(line2.join(" "));

      if (result && result.length > 1) {
        if (!posibleSetEti.startsWith(ETI_START_NAME)) return null;
        eti = posibleSetEti;
      }
    }

    if (result && result.length > 1) {
      const traslateOut = translate(
        instruction,
        result.slice(1, result.length)
      );

      return { line, result: traslateOut, eti, instruction };
    }
  }
  return null;
};

const checkIsHex = (word: string) => {
  for (let i = 0; i < word.length; i++) {
    const l = word[i];
    const r = parseInt(l, 16);
    if (isNaN(r)) return false;
  }
  return true;
};

const translate = (instruction: Instruction, ops: string[]) => {
  const length = ops.length;
  const OPHexCode = instruction.HEX;

  if (length === 1) {
    return [OPHexCode];
  } else if (length === 2) {
    if (instruction.GRUPO < 4) {
      return [OPHexCode];
    } else {
      const inmORDir = ops[1];
      const isDir = instruction.NEMO.includes("dir");
      const isInm = instruction.NEMO.includes("inm");
      if (!inmORDir) return [];

      if (inmORDir.startsWith(ETI_START_NAME)) {
        if (!setEtiquetas.includes(inmORDir)) return [];
        return [OPHexCode, inmORDir];
      }

      const isHex = checkIsHex(inmORDir);
      if (!isHex) return [];

      const hex = parseInt(inmORDir, 16);
      if (isDir && hex > 0xffff) return [];
      if (isInm && hex > 0xff) return [];

      if (isDir) {
        const newHex = hex.toString(16).padStart(4, "0");
        return [OPHexCode, newHex.slice(0, 2), newHex.slice(2)];
      } else {
        const newHex = hex.toString(16).padStart(2, "0");
        return [OPHexCode, newHex.slice(0, 2)];
      }
    }
  } else if (length === 3) {
    // only one code
    if (instruction.GRUPO < 4) {
      return [OPHexCode];
    } else {
      const inmORDir = ops[1];
      const isDir = instruction.NEMO.includes("dir");
      const isInm = instruction.NEMO.includes("inm");
      if (!inmORDir) return [];

      if (inmORDir.startsWith(ETI_START_NAME)) {
        if (!setEtiquetas.includes(inmORDir)) return [];
        return [OPHexCode, inmORDir];
      }

      const isHex = checkIsHex(inmORDir);
      if (!isHex) return [];

      const hex = parseInt(inmORDir, 16);
      if (isDir && hex > 0xffff) return [];
      if (isInm && hex > 0xff) return [];

      if (isDir) {
        const newHex = hex.toString(16).padStart(4, "0");
        return [OPHexCode, newHex.slice(0, 2), newHex.slice(2)];
      } else {
        const newHex = hex.toString(16).padStart(2, "0");
        return [OPHexCode, newHex.slice(0, 2)];
      }
    }
  }
  return [];
};

export interface EtiquetaPos extends Marker {
  eti: RegexResponse;
  dir: number;
}

const calculateEtiquetasPos = (
  results: Result[],
  initDir: number
): EtiquetaPos[] => {
  const toReturn: EtiquetaPos[] = [];
  const defEtiquetas = results.filter((r) => r.eti);
  for (let i = 0; i < defEtiquetas.length; i++) {
    const defEtiqueta = defEtiquetas[i];
    let sum = 0;
    for (let j = 0; j < results.length; j++) {
      const result = results[j];
      if (!result.result) continue;
      if (result === defEtiqueta) {
        break;
      } else {
        sum += result.instruction.ALLOC;
      }
    }

    toReturn.push({
      eti: defEtiqueta,
      dir: initDir + sum,
      lineFrom: defEtiqueta.originalOffset ?? -1,
      lineTo: defEtiqueta.originalOffset ?? -1,
      startCol: defEtiqueta.line.indexOf(defEtiqueta.eti),
      endCol:
        defEtiqueta.line.indexOf(defEtiqueta.eti) + defEtiqueta.eti.length,
    });
  }
  return toReturn;
};

export { parseInput };
