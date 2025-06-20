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

Options available in all versions:

- `--help`                      Show the help message

#### Bonus options 🎁

Available only in the bonus version (`ft_traceroute_bonus`):

- `-f, --first-hop=NUM`         Set the initial hop (TTL) value (default: 1)
- `-m, --max-hop=NUM`           Set the maximum number of hops to probe (default: 64)
- `-p, --port=PORT`             Set the destination port for probes (default: 33434)
- `-q, --tries=NUM`             Set the number of probe packets per hop (default: 3)
- `--resolve-hostnames`         Resolve and display hostnames in addition to IP addresses
- `-w, --wait=NUM`              Set the timeout in seconds to wait for each response (default: 3)
- `-?, --help`                  Show the help message
- `--usage`                     Show a short usage message
- `-V, --version`               Show program version information

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
