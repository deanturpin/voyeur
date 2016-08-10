## Installing Apache on Ubuntu 16
Install
```
sudo apt install apache2
```

Configure - use defaults
```bash
sudo a2enmod cgi
sudo service apache2 restart
```

See the [https://help.ubuntu.com/lts/serverguide/httpd.html](Ubuntu documentation).

## Installing voyeur
Copy everything into into `/usr/lib/cgi-bin`

## Running
http://127.0.0.1/cgi-bin/watch?cat%20/proc/interrupts
