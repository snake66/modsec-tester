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

namespace {
    void process_start_line(Transaction & t, std::istream & input)
    {
        std::string method;
        std::string url;
        std::string version;

        input >> method >> url >> version >> std::ws;
        std::cout << "[*] " << method << " " << url << " " << version << std::endl;

        auto version_pos = version.find_first_of('/');

        t.processURI(url.c_str(), method.c_str(), version.c_str() + version_pos + 1);
    }

    void process_headers(Transaction & t, std::istream & input)
    {
        auto line = std::string{};

        do {
            std::getline(input, line);
            if (!line.empty()) {
                auto key_len = line.find_first_of(':');
                auto value_start = line.find_first_not_of(": \t", key_len);
                auto value_len = line.size() - value_start;
                t.addRequestHeader(
                        reinterpret_cast<const unsigned char *>(line.c_str()), key_len,
                        reinterpret_cast<const unsigned char *>(line.c_str()) + value_start, value_len);
            }
        } while(!line.empty());

        t.processRequestHeaders();
    }

    void process_body(Transaction & t, std::istream & input)
    {
        t.processRequestBody();
    }
}

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

    process_start_line(modsecTransaction, input);
    if (modsecTransaction.intervention(&intervention)) {
        return std::optional<ModSecurityIntervention>{intervention};
    }

    process_headers(modsecTransaction, input);
    if (modsecTransaction.intervention(&intervention)) {
        return std::optional<ModSecurityIntervention>{intervention};
    }

    process_body(modsecTransaction, input);
    if (modsecTransaction.intervention(&intervention)) {
        return std::optional<ModSecurityIntervention>{intervention};
    }

    return std::nullopt;
}
