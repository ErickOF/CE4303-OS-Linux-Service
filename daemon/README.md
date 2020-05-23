# Azure (Opcional)
Copiar los archivos al servidor de Azure:
```
scp /home/user/.../daemon user@publicip:.~/
```

Conéctese a Azure:
```
ssh -i pubkey user@publicip
```
Y entrar al folder `daemon`.

# Configurar Demonio
Ejecute el comando:
```
./setup_demon.sh
```
asegúrese de que el archivo tenga permisos para ejecutarse. Sino ejecute:
```
sudo chmode +x ./setup_demon.sh
```

# Detener
```
sudo systemctl stop ImageServer
```

# Reiniciar
```
sudo systemctl restart ImageServer
```

# Iniciar
```
sudo systemctl start ImageServer
```

# Eliminar el demonio
```
./delete_demon.sh
```
asegúrese de que el archivo tenga permisos para ejecutarse. Sino ejecute:
```
sudo chmode +x ./delete_demon.sh
```
