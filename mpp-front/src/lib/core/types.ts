export interface MppCoreExtension {
    getValue: (ptr: any, type?: string) => any;
}

export interface MppCore {
    print_hello(): void;
    linker_set_update_ui(fnptr: number): void;
    
    init(): void;
    shutdown(): void;

    get_memory_size(): number;
    get_memory_value(offset: number): number;

}

export type UIUpdateCallbackFn = () => void;

export function emptyMppCore(): MppCore & MppCoreExtension {
    return {
        getValue: (ptr: any, type?: string) => {
            throw new Error(`Method getValue not implemented`);
        },
        print_hello: () => { throw new Error("MppCore not loaded"); },
        linker_set_update_ui: (fnptr: number) => { throw new Error("MppCore not loaded"); },

        init: () => { throw new Error("MppCore not loaded"); },
        shutdown: () => { throw new Error("MppCore not loaded"); },

        get_memory_size: () => { throw new Error("MppCore not loaded"); },
        get_memory_value: (offset: number) => { throw new Error("MppCore not loaded"); },

    };
}