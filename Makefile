# SPDX-FileCopyrightText: 2022 Automattic, Inc
# SPDX-FileCopyrightText: 2022 Harald Eilertsen <harald.eilertsen@automattic.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later

VPATH = src

LDFLAGS := -lmodsecurity

all: modsec-tester

modsec-tester: main.o process.o
	g++ -o $@ $^ $(CFLAGS) $(LDFLAGS)

main.o: src/main.cpp src/process.hpp
process.o: src/process.cpp src/process.hpp
