 
DATE=`date '+%Y-%m-%d %H:%M:%S'`
echo "Example service started at ${DATE}" | systemd-cat -p info

# TODO: ACTUALIZAR ESTO CON LA DIRECCION FINAL
echo "Server starting..."
cd /etc/server && sudo ./server