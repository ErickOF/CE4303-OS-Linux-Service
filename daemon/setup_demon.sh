#!/bin/bash

# Movemos el script del servicio y le damos permisos
sudo cp server_service.sh /usr/bin/server_service.sh
echo "server_service.sh copy to /usr/bin/server_service.sh"
sudo chmod +x /usr/bin/server_service.sh
echo "Permissions given to /usr/bin/server_service.sh"

# Copiamos el servicio al sistema y le damos permisos
sudo cp ImageServer.service /etc/systemd/system/ImageServer.service
echo "ImageServer.service copy to /etc/systemd/system/ImageServer.service"
sudo chmod 644 /etc/systemd/system/ImageServer.service
echo "Permissions given to /etc/systemd/system/ImageServer.service"

# Compilamos
echo "Compiling code..."
cd server
make clean_all
make
cd ..

# Copiamos el servidor a los archivos del sistema
sudo cp -r ./server /etc/
echo "server copy to /etc/"
sudo chmod +x /etc/server/server
echo "Permissions given to /etc/server/server"

# Activamos el server
sudo systemctl start ImageServer
echo "Image server started"
# Verificamos su funcionamiento
sudo systemctl status ImageServer
# Lo activamos para que corra al iniciar el sistema
sudo systemctl enable ImageServer
echo "Image server enabled"