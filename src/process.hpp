// modsec-tester
//
// Header for request processing functions.
//
// SPDX-FileCopyrightText: 2022 Automattic, Inc
// SPDX-FileCopyrightText: 2022 Harald Eilertsen <harald.eilertsen@automattic.com>
//
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MODSEC_TESTER_PROCESS_HPP
#define MODSEC_TESTER_PROCESS_HPP

#include <modsecurity/modsecurity.h>
#include <modsecurity/rules_set.h>
#include <optional>

std::optional<modsecurity::ModSecurityIntervention> process(
        modsecurity::ModSecurity & modsec,
        modsecurity::RulesSet & rules,
        std::istream & input);

#endif
