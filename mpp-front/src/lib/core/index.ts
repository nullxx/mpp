import type { MppCore, UIUpdateCallbackFn } from "./types";
import libmppModule from "./files/libmpp";
import { emptyMppCore } from "./types";
import toast from "react-hot-toast";

let moduleInstance: any;
export let mppCore: MppCore | null;

const uiUpdatesSubscriptions = new Set<UIUpdateCallbackFn>();

const exportedMethods: {
  name: keyof MppCore;
  returnType: "number" | "string" | "array" | null;
  typeArgs: ("number" | "string")[];
}[] = [
  {
    name: "init",
    returnType: null,
    typeArgs: [],
  },
  {
    name: "shutdown",
    returnType: null,
    typeArgs: [],
  },

  {
    name: "get_memory_size",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_memory_value_size_bits",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_memory_value",
    returnType: "number",
    typeArgs: ["number"],
  },
  {
    name: "get_memory_dir_bus",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "set_memory_value",
    returnType: null,
    typeArgs: ["number", "number"],
  },

  {
    name: "get_register_acum",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_acum",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_fc",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_fz",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_b",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_c",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_d",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_e",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_h",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_l",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_2op",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_pch",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_pcl",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_pc",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_sp",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_register_ri",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "set_register_pc",
    returnType: null,
    typeArgs: ["number"],
  },

  {
    name: "run_clock_cycle",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_state",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_next_state",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_data_bus",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_pccar",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_accar",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_acbus",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_spcar",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_2opcar",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_hcar",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_lcar",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_ricar",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_regcar",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_regbus",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_membus",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_le",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "get_control_bus_selalu",
    returnType: "number",
    typeArgs: [],
  },
  {
    name: "reset_control",
    returnType: null,
    typeArgs: [],
  },
  {
    name: "linker_set_update_ui",
    returnType: null,
    typeArgs: [],
  },
];

export function getCConsoleHandle() {
  return {
    print: (...args: unknown[]) => {
      console.log(...args);
    },
    printErr: (...args: unknown[]) => {
      if (process.env.NODE_ENV !== "production") {
        return console.error(...args);
      }

      toast.error(args.join(" "), {
        style: {
          border: "1px solid #713200",
          padding: "10px",
          color: "#713200",
        },
      });
    },
  };
}

export async function loadInstance(): Promise<void> {
  moduleInstance = await libmppModule(getCConsoleHandle());
  const tmpMppCore = emptyMppCore();

  for (const method of exportedMethods) {
    const { name, returnType, typeArgs } = method;
    tmpMppCore[name as keyof MppCore] = moduleInstance.cwrap(
      name,
      returnType,
      typeArgs
    );
  }

  mppCore = tmpMppCore;
}

export async function connectBackend() {
  if (mppCore) {
    console.warn("MppCore already loaded");
    return;
  }

  await loadInstance();
  if (!mppCore) throw new Error("MppCore not loaded");
  console.info("MppCore loaded");

  execute("init");

  console.info("MppCore initialized");

  createUpdateUICallback();
  console.info("MppCore UI callback connected");
}

export async function disconnectBackend() {
  if (!mppCore) {
    console.warn("MppCore not loaded");
    return;
  }

  execute("shutdown");
  mppCore = null;
}

export function getCore() {
  if (!mppCore) throw new Error("MppCore not loaded");
  return mppCore;
}

export function execute<T = any>(method: keyof MppCore, ...args: unknown[]) {
  if (!mppCore) throw new Error("MppCore not loaded");
  const response = (mppCore[method as keyof MppCore] as Function)(...args);
  return response as T;
}

export function subscribeToUIUpdates(callback: UIUpdateCallbackFn) {
  uiUpdatesSubscriptions.add(callback);
}

export function unsubscribeToUIUpdates(callback: UIUpdateCallbackFn) {
  uiUpdatesSubscriptions.delete(callback);
}

export function notifyUpdateToSubscribers() {
  uiUpdatesSubscriptions.forEach((callback) => callback());
}

export function createUpdateUICallback() {
  const fnPtr = moduleInstance.addFunction(() => {
    notifyUpdateToSubscribers();
  }, "v");

  mppCore?.linker_set_update_ui(fnPtr);
}
