sudo systemctl stop ImageServer.service
sudo systemctl disable ImageServer.service
sudo rm /etc/systemd/system/ImageServer.service
sudo rm /usr/bin/server_service.sh
sudo rm -r /etc/server/
sudo systemctl daemon-reload
sudo systemctl reset-failed