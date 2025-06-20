# ft_traceroute 🛰️

[![WTFPL License](https://www.wtfpl.net/wp-content/uploads/2012/12/wtfpl-badge-2.png)](http://www.wtfpl.net/)

`ft_traceroute` is a simplified implementation of the classic `traceroute` utility. It traces the route that packets take to a network host, displaying each hop along the way.

## Features ✨

- Sends UDP probes with increasing TTL to discover each hop to the destination.
- Displays the IP address and round-trip time for each hop.
- Bonus: Additional options and features (see below).

## Usage 🚀

```sh
./ft_traceroute <destination>
```

For the bonus version (with extra options):

```sh
./ft_traceroute_bonus <destination> [options]
```

### Options

Options valid for all versions:

- `--help`                      Give this help list

#### Bonus options 🎁

Available only in the bonus version (`ft_traceroute_bonus`):

- `-f, --first-hop=NUM`         set initial hop distance, i.e., time-to-live
- `-m, --max-hop=NUM`           set maximal hop count (default: 64)
- `-p, --port=PORT`             use destination PORT port (default: 33434)
- `-q, --tries=NUM`             send NUM probe packets per hop (default: 3)
- `--resolve-hostnames`         resolve hostnames
- `-w, --wait=NUM`              wait NUM seconds for response (default: 3)
- `-?, --help`                  Give this help list
- `--usage`                     Give a short usage message
- `-V, --version`               Print program version

### Example

#### Mandatory

```sh
sudo ./ft_traceroute google.com
```

#### Bonus

```sh
# Set first hop to 5, max hop to 30, and use 5 tries per hop
sudo ./ft_traceroute_bonus -f 5 -m 30 -q 5 8.8.8.8
```

## Build 🛠

To build the project with bonus features, run:

```sh
make bonus
```

Or, to build the mandatory part only:

```sh
make
```

## Requirements 📦

- GCC or Clang
- Make

## Notes 📝

- Root privileges may be required to receive raw icmp packets.
- This project is for educational purposes.

## License 📄

WTFPL – Do What the Fuck You Want to Public License
