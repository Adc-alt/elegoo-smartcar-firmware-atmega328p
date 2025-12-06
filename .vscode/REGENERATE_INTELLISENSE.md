# 🔄 Cómo Regenerar c_cpp_properties.json

El archivo `c_cpp_properties.json` es **auto-generado** por PlatformIO y **NO debe modificarse manualmente**.

## ⚠️ Problema

Si modificas `c_cpp_properties.json` manualmente, los cambios se perderán cuando PlatformIO regenere el archivo.

## ✅ Solución: Regenerar el archivo

Para regenerar `c_cpp_properties.json` con las rutas correctas:

### Método 1: Compilar el proyecto
```bash
pio run
```
Esto regenerará automáticamente `c_cpp_properties.json` con todas las rutas correctas.

### Método 2: Desde VS Code
1. Abre la paleta de comandos: `Ctrl+Shift+P`
2. Ejecuta: `PlatformIO: Rebuild IntelliSense Index`
3. O simplemente compila el proyecto desde VS Code

### Método 3: Forzar regeneración
```bash
# Eliminar el archivo y regenerarlo
rm .vscode/c_cpp_properties.json
pio run --target compiledb
```

## 📝 Notas Importantes

- Las rutas del toolchain (`toolchain-atmelavr/lib/gcc/avr/*/include`) se agregan automáticamente
- Las rutas de librerías (`.pio/libdeps/*`) se agregan automáticamente
- Las rutas personalizadas deben agregarse en `platformio.ini` usando `build_flags = -I ruta`

## 🔧 Configuración en platformio.ini

Si necesitas rutas adicionales, agrégalas en `platformio.ini`:

```ini
build_flags =
    -I include
    -I lib/custom_path
```

PlatformIO las incluirá automáticamente en `c_cpp_properties.json` cuando regenere el archivo.

