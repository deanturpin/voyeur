You can run anything under `watch` so __this doesn't belong on a public web server__.

## Installing Apache on Ubuntu 16
```bash
# Install
sudo apt install apache2
```

```bash
# Enable CGI and start the server
sudo a2enmod cgi
sudo service apache2 restart
```

See the [https://help.ubuntu.com/lts/serverguide/httpd.html](Ubuntu documentation).

## Installing watch
Copy everything into `/usr/lib/cgi-bin`

## Running watch
Simply add a command to the query string.
```
http://127.0.0.1/cgi-bin/watch?cat%20/proc/interrupts
```

## OUI
OUI must be installed for the vendor lookup. Curiously it is installed by
default on CentOS and Scientific Linux but not regular Ubuntu. It is trivial to
install, though.

```
sudo apt install ieee-data
update-oui
```

See `/usr/share/ieee-data/oui.txt`.
