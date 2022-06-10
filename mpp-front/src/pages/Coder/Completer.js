import operations from '../../lib/traslator/operations.json';

const completer = {
    getCompletions: (_editor, _session, _pos, prefix, callback) => {
        const allInstructions = [...new Set(operations.map(s => s.NEMO.split(" ")[0]))];
        const allRegisters = ['AC', 'RB', 'RC', 'RD', 'RE'];
        const allConstants = Array.from(Array(0xFFFF+1).keys()).map(i => i.toString(16).toUpperCase());
        const allCompletions = [...new Set([...allInstructions, ...allRegisters, ...allConstants])];
        const completions = allCompletions.filter(c => c.startsWith(prefix));

        callback(null, completions.map(c => {
            return {
                caption: c,
                value: c,
                meta: "assembly"
            };
        }));

        return true;
    }
};

export default completer;