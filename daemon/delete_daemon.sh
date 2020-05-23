sudo systemctl stop ImageServer.service
sudo systemctl disable ImageServer.service
sudo rm /etc/systemd/system/ImageServer.service
sudo rm /usr/lib/systemd/system/ImageServer.service
sudo systemctl daemon-reload
sudo systemctl reset-failed