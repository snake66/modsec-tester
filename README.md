# A simple test bench for ModSecurity rules

This is a very simple program just wrapping the ModSecurity API's to make
it feasible to test ModSecurity rules without having to set up a full server
environment.

It takes a list of rules files as arguments, and checks the raw request passed
in stdin with these rules. It's possible to pass in a single rule, or a full
set of rules.

## Usage

```
$ ./modsec-tester path/to/rules/*.conf < raw-request
```

## Building

The code requires the ModSecurity libs and headers installed on the system, and
a C++17 capable C++ compiler. Other than that it should have no dependencies.

Build by running `make` in the root of the repository.

## Note

This will _only_ test raw requests against the loaded ModSecurity rule sets. It will not
pretend to be a server, or produce any meaningful response. Neither will it at this time
check responses against the ModSecurity response rules.

The code is provided as is, in the hope that it will be useful.

## License

See the LICENSES directory for applicable licenses, and individual files for which
licenses apply to each.

This project uses [Reuse](https://reuse.software/) to manage licenses.

SPDX-FileCopyrightText: 2022 Automattic, Inc
SPDX-FileCopyrightText: 2022 Harald Eilertsen <harald.eilertsen@automattic.com>

SPDX-License-Identifier: CC0-1.0
