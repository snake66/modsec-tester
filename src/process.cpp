// modsec-tester
//
// Functions for processing a request.
//
// SPDX-FileCopyrightText: 2022 Automattic, Inc
// SPDX-FileCopyrightText: 2022 Harald Eilertsen <harald.eilertsen@automattic.com>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#include "process.hpp"
#include <iostream>
#include <optional>

using modsecurity::LogProperty;
using modsecurity::ModSecurity;
using modsecurity::RulesSet;
using modsecurity::Transaction;
using modsecurity::ModSecurityIntervention;


std::optional<ModSecurityIntervention> process(
        ModSecurity & modsec,
        RulesSet & rules,
        std::istream & input)
{
    auto modsecTransaction = Transaction(&modsec, &rules, nullptr);
    auto intervention = ModSecurityIntervention{};

    // Just hardcode the connection for now.
    modsecTransaction.processConnection("127.0.0.1", 443, "192.168.1.1", 1234);
    if (modsecTransaction.intervention(&intervention)) {
        return std::optional<ModSecurityIntervention>{intervention};
    }

    std::string method;
    std::string url;
    std::string version;

    input >> method >> url >> version;
    std::cout << "[*] " << method << " " << url << " " << version << std::endl;

    modsecTransaction.processURI(url.c_str(), method.c_str(), version.c_str());
    if (modsecTransaction.intervention(&intervention)) {
        return std::optional<ModSecurityIntervention>{intervention};
    }

    modsecTransaction.processRequestHeaders();
    if (modsecTransaction.intervention(&intervention)) {
        return std::optional<ModSecurityIntervention>{intervention};
    }

    modsecTransaction.processRequestBody();
    if (modsecTransaction.intervention(&intervention)) {
        return std::optional<ModSecurityIntervention>{intervention};
    }

    return std::nullopt;
}
