export const SITE = {
	title: 'Documentación',
	description: 'M++ simulator docs.',
	defaultLanguage: 'es_ES',
};


export const KNOWN_LANGUAGES = {
	Spanish: 'es',
};

// Uncomment this to add an "Edit this page" button to every page of documentation.
export const GITHUB_EDIT_URL = `https://github.com/nullxx/mpp/tree/master/_docs/`;

// Uncomment this to enable site search.
// See "Algolia" section of the README for more information.
// export const ALGOLIA = {
//   indexName: 'XXXXXXXXXX',
//   appId: 'XXXXXXXXXX',
//   apiKey: 'XXXXXXXXXX',
// }

export const SIDEBAR = {
	es: [
		{ text: '', header: true },
		{ text: 'Sobre mpp', header: true },
		{ text: 'Introducción', link: 'es/introduction' },
		{ text: 'Arquitectura', link: 'es/architecture' },
		{ text: 'Detalles', link: 'es/about' },


		{ text: 'USO', header: true },
		{ text: 'Instrucciones soportadas', link: 'es/instructions/instructions' },
		{ text: 'MOV', link: 'es/instructions/MOV'},
		{ text: 'SUB', link: 'es/instructions/SUB'},
		{ text: 'ADD', link: 'es/instructions/ADD'},
		{ text: 'INC', link: 'es/instructions/INC'},
		{ text: 'CMP', link: 'es/instructions/CMP'},
		{ text: 'AND', link: 'es/instructions/AND'},
		{ text: 'OR', link: 'es/instructions/OR'},
		{ text: 'XOR', link: 'es/instructions/XOR'},
		{ text: 'CMA', link: 'es/instructions/CMA'},
		{ text: 'LDA', link: 'es/instructions/LDA'},
		{ text: 'STA', link: 'es/instructions/STA'},
		{ text: 'LDAX', link: 'es/instructions/LDAX'},
		{ text: 'STAX', link: 'es/instructions/STAX'},
		{ text: 'SFA', link: 'es/instructions/SFA'},
		{ text: 'LFA', link: 'es/instructions/LFA'},
		{ text: 'INISP', link: 'es/instructions/INISP'},
		{ text: 'PUSH', link: 'es/instructions/PUSH'},
		{ text: 'POP', link: 'es/instructions/POP'},
		{ text: 'BEQ', link: 'es/instructions/BEQ'},
		{ text: 'BC', link: 'es/instructions/BC'},
		{ text: 'JMP', link: 'es/instructions/JMP'},
		{ text: 'CALL', link: 'es/instructions/CALL'},
		{ text: 'RET', link: 'es/instructions/RET'},
		{ text: 'FIN', link: 'es/instructions/FIN'},

		{ text: 'Ejemplos', header: true },
		{ text: 'Multiplicación', link: 'es/examples/multiplication'},
	],
};
