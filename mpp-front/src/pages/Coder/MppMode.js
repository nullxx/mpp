/*
	Don't look at this :( It's a mess. I just like the result, but there is no way to make it work with typescript at this time.
*/

import 'brace/mode/java';
import operations from '../../lib/traslator/operations.json';

export class CustomHighlightRules extends window.ace.acequire("ace/mode/text_highlight_rules").TextHighlightRules {
	constructor() {
		super();
		const allInstructions = [...new Set(operations.map(s => s.NEMO.split(" ")[0]))].join('|');

		this.$rules = {
			"start": [
				{
					token: "entity.name.function.assembly",
					regex: "T\\S*",
					caseInsensitive: true
				},
				{
					token: "keyword.control.assembly",
					regex: `\\b(?:${allInstructions})\\b`,
					caseInsensitive: true
				}, {
					token: "variable.parameter.register.assembly",
					regex: '\\b(?:AC|RB|RC|RD|RE)',
					caseInsensitive: true
				}, {
					token: "constant.character.hexadecimal.assembly",
					regex: '[0-9A-F]+',
					caseInsensitive: true
				},
				{
					token: "comment.assembly",
					regex: "#.*$"
				}],
		};
	}
}

export default class MppMode extends window.ace.acequire('ace/mode/java').Mode {
	constructor() {
		super();
		this.HighlightRules = CustomHighlightRules;
	}
}