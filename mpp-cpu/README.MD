# CPU
## DEPENDENCIES

- [Emscripten](https://emscripten.org/docs/getting_started/index.html)
## BUILDING

```console
make
```

This will generate the following files in output/
- libmpp.js
- libmpp.wasm


## TESTING

```console
RUNTEST=1 make compile-tests && make run-tests
```