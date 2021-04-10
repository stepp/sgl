/*
 * File: privateregexpr.cpp
 * ------------------------
 * Implementation of the functions in privateregexpr.h.
 * See regexpr.h for documentation of each function.
 * This functionality is considered "private" and not to be used by students.
 *
 * @author Marty Stepp
 * @version 2021/04/09
 * - moved to private SGL namespace
 * - renamed functions to remove 'regex' prefix
 * @version 2021/04/03
 * - removed dependency on custom collections
 * @version 2018/12/16
 * - added CodeStepByStep disabling of regexes
 * @version 2018/11/22
 * - added headless (non-Qt) mode support
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/14
 * - removed regexMatchCountWithLines for simplicity
 * 2014/10/08
 * - removed 'using namespace' statement
 * @since 2014/03/01
 */

#include "privateregexpr.h"
#ifndef SGL_HEADLESS_MODE
#include <exception>
#include <stdexcept>
#include <QtGlobal>
#endif // SGL_HEADLESS_MODE

#if defined(SPL_CODESTEPBYSTEP) || QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
namespace sgl {
namespace priv {
namespace regexpr {

bool match(const std::string& /*s*/, const std::string& /*regexp*/) {
    return false;   // not supported
}

int matchCount(const std::string& /*s*/, const std::string& /*regexp*/) {
    return 0;   // not supported
}

void matchCountWithLines(const std::string& /*s*/, const std::string& /*regexp*/,
                         std::vector<int>& /*linesOut*/) {
    // empty; not supported
}

int matchCountWithLines(const std::string& /*s*/, const std::string& /*regexp*/,
                        std::string& /*linesOut*/) {
    return 0;   // not supported
}

std::string replace(const std::string& s, const std::string& /*regexp*/, const std::string& /*replacement*/, int /*limit*/) {
    return s;   // not supported
}

} // namespace regexpr
} // namespace priv
} // namespace sgl

#else // QT_VERSION

// C++ regex support
#include <iterator>
#include <regex>

namespace sgl {
namespace priv {
namespace regexpr {

bool match(const std::string& s, const std::string& regexp) {
    std::regex reg(regexp);
    std::smatch match;
    return std::regex_search(s, match, reg);
}

int matchCount(const std::string& s, const std::string& regexp) {
    std::regex reg(regexp);
    auto it1 = std::sregex_iterator(s.begin(), s.end(), reg);
    auto it2 = std::sregex_iterator();
    return std::distance(it1, it2);
}

void matchCountWithLines(const std::string& s, const std::string& regexp,
                         std::vector<int>& linesOut) {
    linesOut.clear();

    // keep a running index and line#, and each time we find a regex match,
    // "walk" forward to its index.  when we see a \n, increment line number
    int currentIndex = 0;
    int currentLine = 1;

    // get all regex matches by character position/index
    std::regex reg(regexp);
    for (std::sregex_iterator itr = std::sregex_iterator(s.begin(), s.end(), reg),
            end = std::sregex_iterator();
            itr != end;
            ++itr) {
        std::smatch match = *itr;
        int matchIndex = (int) match.position();
        while (currentIndex < (int) s.length() && currentIndex < matchIndex) {
            if (s[currentIndex] == '\n') {
                currentLine++;
            }
            currentIndex++;
        }
        linesOut.push_back(currentLine);
    }
}

int matchCountWithLines(const std::string& s, const std::string& regexp,
                        std::string& linesOut) {
    std::vector<int> linesOutVec;
    matchCountWithLines(s, regexp, linesOutVec);

    // concatenate the vector into a string like "1, 4, 7, 7, 19"
    linesOut = "";
    if (!linesOutVec.empty()) {
        linesOut += std::to_string(linesOutVec[0]);
        for (int i = 1; i < linesOutVec.size(); i++) {
            linesOut += ", " + std::to_string(linesOutVec[i]);
        }
    }
    return linesOutVec.size();
}

std::string replace(const std::string& s, const std::string& regexp, const std::string& replacement, int limit) {
    std::regex reg(regexp);
    std::string result;
    if (limit == 1) {
        // replace single occurrence
        result = std::regex_replace(s, reg, replacement,
                                    std::regex_constants::format_first_only);
    } else if (limit <= 0) {
        // replace all
        result = std::regex_replace(s, reg, replacement);
    } else {
        throw std::runtime_error("regexReplace: given limit not supported.");
    }
    return result;
}

} // namespace regexpr
} // namespace priv
} // namespace sgl

#endif // QT_VERSION
