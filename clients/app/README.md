# App Android/iOS

Este proyecto fue generado con [Ionic Framework 5](https://ionicframework.com/) versión 5.0.7.

## Requerimientos
Debe estar instalado [NodeJS](https://nodejs.org/en/).

Si se desea una aplicación Android, el [SDK](https://developer.android.com/studio) debe ser instalado
con las correspondientes versiones de Android deseadas.

Si se desea una aplicación para iOS, el [SDK](https://developer.apple.com/ios/) debe ser instalado.


## Instalación
Para instalar Angular CLI:
```
npm i -g @angular/cli
```

Para instalar Ionic Framework:
```
npm i -g @ionic/cli
```

Para instalar Cordova:
```
npm install -g cordova
```

Para instalar los módulos necesarios:
```
npm i
```

## Configurar plataformas
Para emular y construir en android ejecute:
```
ionic cordova platform add android
```

Para emular y construir en iOS ejecute:
```
ionic cordova platform add ios
```

## Emular en la Web
Para emular en la web, ejecute:
```
ionic serve --port 3000
```
Luego navegue a `http://localhost:3000/` y la aplicación será recargada automáticamente cada vez
que se realice un cambio a los archivos fuentes.

Para emular un dispositivo móvil iOS, ejecute
```
ionic cordova build ios
ionic cordova emulate ios
```

Para emular un dispositivo móvil Android, ejecute
```
ionic cordova build android
ionic cordova emulate android
```

## Build

Ejecute
```
ionic cordova build android
```
o
```
ionic cordova build ios
```
dependiendo de la plataforma a usar y luego instalé el archivo.
