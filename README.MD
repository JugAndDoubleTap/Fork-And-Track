
# Fork And Track

## Overview

**Fork And Track** is a simple utility that allows you to easily fork programs or commands
with group pid output for the forked process

A group pid is output as it ensures the pid stays consistent through runtime and
any spawned processes are also linked to it


## Features

- Launch programs or commands in a forked state
- Group pid output for tracking or termination


## Installation (build)

```bash
# Clone the repo
git clone https://github.com/JugAndDoubleTap/Fork-And-Track.git
# Enter directory
cd Fork-And-Track
# Run compile script (uses g++ and c++std23)
chmod +x compile.sh && ./compile.sh
# Place into local program/utility path
sudo cp -a fat /usr/local/bin/
```


## Arguments list

- **--program / -p** Path to the program or command to be forked.
- **--arguments / -a** String of arguments to provide to the forked program / command.


## Example usages

```bash
fat -p start-server -a "--nogui --noconsole"

PID=$(fat -p ~/applications/long-process.sh)

fat -p python3 -a "background-process.py"
```


## Using the output pid

```bash
# Using pkill
pkill -g $PID

# Using kill (use negative pid)
kill -9 -$PID
```


This utility uses the following libraries:

### Third-Party Libraries
- **argparse**: Used to provide easy argument processing to the utility, located in includes/argparse.hpp.
The argparse repo can be found [here.](https://github.com/p-ranav/argparse)


### Original Code License
This project, **Fork And Track**, is licensed under the **GNU Lesser General Public License v3.0 (LGPLv3)**.
