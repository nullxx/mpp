---
title: Arquitectura
description: Arquitectura del simulador
layout: ../../layouts/MainLayout.astro
---

Como muchos proyectos de software, el simulador M++ tiene una arquitectura modular:
* Backend: módulo que se encarga de la lógica del simulador.
* Frontend: módulo que se encarga de la [interfaz gráfica del simulador](https://es.wikipedia.org/wiki/Interfaz_gr%C3%A1fica_de_usuario).

## Tecnologías

Para el desarrollo del simulador se utilizaron las siguientes tecnologías:
* [C](https://es.wikipedia.org/wiki/C_(lenguaje_de_programaci%C3%B3n)) para el backend.
* Web con [React](https://es.wikipedia.org/wiki/React) para el frontend.

**[C](https://es.wikipedia.org/wiki/C_(lenguaje_de_programaci%C3%B3n))** es un lenguaje de propósito general de medio/bajo nivel que se puede utilizar en todos los sistemas operativos por su independecia de hardware. Es por esto por lo que el backend se desarrolló en este lenguaje.

El frontend se desarrolló en **[React](https://es.wikipedia.org/wiki/React)**, una librería de JavaScript para la web.

## Multiplataforma

Uno de los aspectos más importantes es la compatibilidad con todos los sistemas operativos.

La interfaz visual fue lo más problematico para programarlo en [C](https://es.wikipedia.org/wiki/C_(lenguaje_de_programaci%C3%B3n)). Aunque hay varias librerías que hacen esto posible, fue una tarea difícil y tediosa que no producía el resultado deseado.

El objetivo era conseguir un diseño fácil de utilizar y sobretodo amigable y bonito.

Cuando un usuario utiliza tu programa cómo una herramienta para realizar algo, lo más que puede hacer el desarrollador es facilitarselo a través de una interfaz accesible.

También traté de hacer la interfaz gráfica en Java, pero tampoco me convencía.

Entonces es cuando compienzo el desarrollo de la interfaz gráfica en la web, a través de [React](https://es.wikipedia.org/wiki/React).

Pero, si el núcleo está desarrollado en [C](https://es.wikipedia.org/wiki/C_(lenguaje_de_programaci%C3%B3n)), ¿cómo puedo utilizarlo en la web?. Tradicionalmente no se podía "importar" ficheros binarios (compilados de [C](https://es.wikipedia.org/wiki/C_(lenguaje_de_programaci%C3%B3n))) en la web. Desde 2015, esto es posible gracias a [WebAssembly](https://en.wikipedia.org/wiki/WebAssembly), un formato de [código binario](https://es.wikipedia.org/wiki/C%C3%B3digo_binario) portable que se ejecuta en el lado del cliente (portable).

#### ¿Para qué desarrollar un programa en cada plataforma, si se puede crear una web accesible desde todos los sistemas operativos?
