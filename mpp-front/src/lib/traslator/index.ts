import instructions from "./operations.json";

interface Instruction {
  HEX: string;
  NEMO: string;
  REGEX: string;
  GRUPO: number;
  ALLOC: number;
}

export interface TraslationError {
  lineFrom: number;
  lineTo: number;
  startCol: number;
  endCol: number;
}

let setEtiquetas: string[] = [];
const parseInput = (text: string, initDir: number) => {
  setEtiquetas = [];
  let results: RegexResponse[] = [];
  const errors: TraslationError[] = [];
  const lines = text.split("\n");

  pre(lines);

  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    if (!line) continue;

    const result = executeRegex(line);
    console.log("result", result, line);
    if (!result) {
      errors.push({ lineFrom: i, lineTo: i, startCol: 0, endCol: line.length });
      continue;
    }
    results.push(result);
  }

  results = post(results, initDir);

  return {
    errors,
    results: results.map((s) => s.result),
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
const post = (results: RegexResponse[], initDir: number) => {
  const etiPositions = calculateEtiquetasPos(results, initDir);

  for (let i = 0; i < results.length; i++) {
    const result = results[i];
    if (!result.result) continue;

    for (let j = 1; j < result.result.length; j++) {
      let r = result.result[j];
      const eti = etiPositions.find((s) => s.eti.eti === r);
      if (eti) {
        // console.info("Found eti at", result.result);
        result.result.splice(j, 1);
        const etiDirHex = Number(eti.dir)
          .toString(16)
          .toUpperCase()
          .padStart(4, "0");
        for (let i = 0; i < etiDirHex.length; i += 2) {
          result.result.push(etiDirHex.slice(i, i + 2));
        }

        // result.result.push(etiDirHex.slice(0, 2), etiDirHex.slice(2));
      } else if (!/[0-9A-Fa-f]{2}/g.test(r)) {
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
        eti = posibleSetEti;
      }
    }

    if (result && result.length > 1) {
      const traslateOut = translate(
        instruction,
        result.slice(1, result.length)
      );
      if (traslateOut == null) return null;
      return { line, result: traslateOut, eti, instruction };
    }
  }
  return null;
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

      if (inmORDir.toUpperCase().startsWith("ET")) {
        if (!setEtiquetas.includes(inmORDir)) return null;
        return [OPHexCode, inmORDir];
      }

      if (isDir && inmORDir.length !== 4) {
        return [];
      }

      if (isInm && inmORDir.length !== 2) {
        return [];
      }

      if (isDir) {
        return [OPHexCode, inmORDir.slice(0, 2), inmORDir.slice(2)];
      } else {
        return [OPHexCode, inmORDir.slice(0, 2)];
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

      if (inmORDir.toUpperCase().startsWith("ET")) {
        return [OPHexCode, inmORDir];
      }

      if (isDir && inmORDir.length !== 4) {
        return [];
      }

      if (isInm && inmORDir.length !== 2) {
        return [];
      }

      if (isDir) {
        return [OPHexCode, inmORDir.slice(0, 2), inmORDir.slice(2)];
      } else {
        return [OPHexCode, inmORDir.slice(0, 2)];
      }
    }
  }
  return [];
};

interface EtiquetaPos {
  eti: RegexResponse;
  dir: number;
}

const calculateEtiquetasPos = (
  results: RegexResponse[],
  initDir: number
): EtiquetaPos[] => {
  const toReturn: EtiquetaPos[] = [];
  const defEtiquetas = results.filter((r) => r.eti);
  for (let i = 0; i < defEtiquetas.length; i++) {
    const defEtiqueta = defEtiquetas[i];
    let sum = 0;
    for (let i = 0; i < results.length; i++) {
      const result = results[i];
      if (!result.result) continue;
      if (result === defEtiqueta) {
        break;
      } else {
        console.info(
          result.line,
          result.result,
          "+" + result.instruction.ALLOC,
          "=>",
          sum
        );
        sum += result.instruction.ALLOC;
      }
    }
    toReturn.push({
      eti: defEtiqueta,
      dir: initDir + sum,
    });
    console.info(
      "Index hasta declaracion de et: " + defEtiqueta.eti + " => " + sum
    );
  }
  return toReturn;
};

export { parseInput };
