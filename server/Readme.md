El servidor no es mutithread, por lo que solo puede atender 1 cliente de manera simultánea.

El servidor recibe un mensaje POST que contiene el archivo a transferir.

El nombre del nuevo archivo puede definirse por medio del encabezado, ejemplo:
```
Filename: imagen.jpeg
```

Si no lleva el encabezado anterior, el nombre del archivo será la hora del sistema en el formato Y-m-d-H-M-S.
Además intentará tomar la extensión del encabezado Content-Type si se encuentra disponible. Los formatos que busca de imágenes son:
* jpeg, jpg, png
