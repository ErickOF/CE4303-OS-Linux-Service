#!/bin/bash

# Movemos el script del servicio y le damos permisos
sudo cp server_service.sh /usr/bin/server_service.sh
sudo chmod +x /usr/bin/server_service.sh

# Copiamos el servicio al sistema y le damos permisos
sudo cp sserver.service /etc/systemd/system/sserver.service
sudo chmod 644 /etc/systemd/system/sserver.service

# Activamos el server
sudo systemctl start sserver
# Verificamos su funcionamiento
sudo systemctl status sserver
# Lo activamos para que corra al iniciar el sistema
sudo systemctl enable sserver