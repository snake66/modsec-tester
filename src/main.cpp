/*
 * modsec-tester -- A simple wrapper to test ModSecurity rules.
 *
 * Usage:
 *
 *     cat <raw-request> | modsec-tester <rule-files...>
 *
 * Building:
 *
 * Requires the ModSecurity libs and headers installed on the system,
 * and a C++17 capable C++ copiler.
 *
 * Run `make` in the root of this directory tree to build the binary.
 *
 * License:
 *
 * SPDX-FileCopyrightText: 2022 Automattic, Inc
 * SPDX-FileCopyrightText: 2022 Harald Eilertsen <harald.eilertsen@automattic.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <modsecurity/modsecurity.h>
#include <modsecurity/rules_set.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>

#include "process.hpp"

using modsecurity::LogProperty;
using modsecurity::ModSecurity;
using modsecurity::RulesSet;

void logger(void *, const void * data)
{
    std::cerr << "[*] " << reinterpret_cast<const char *>(data) << std::endl;
}

int main(int argc, char * argv[])
{
    if (argc < 2) {
        std::cerr << "[-] No rules to load: Specify at least one rule file to load." << std::endl;
        return EXIT_FAILURE;
    }

    auto modsec = ModSecurity{};
    modsec.setServerLogCb(logger, LogProperty::TextLogProperty);

    auto rules = RulesSet{};

    std::for_each(argv++, argv + argc,
            [&rules](const char * arg) {
                rules.loadFromUri(arg);
            });

    auto result = process(modsec, rules, std::cin);
    if (result.has_value()) {
        std::cout << "Blocked!" << std::endl;
    }

    std::cout << "Done." << std::endl;
}
