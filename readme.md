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

## Installing watch
Copy everything into into `/usr/lib/cgi-bin`

## Running
You can run anything under watch (of course this is a HUGE security hole).

`http://127.0.0.1/cgi-bin/watch?cat%20/proc/interrupts`

## OUI
OUI must be installed for the vendor lookup. Curiously it is by default on
CentOS and Scientific Linux but not Ubuntu. It is trivial to install, though.

```
sudo apt install ieee-data
update-oui
```

See `/usr/share/ieee-data/oui.txt`.
