/*
 * File: diff.cpp
 * --------------
 * This file contains implementations of functions that perform a text 'diff'
 * operation to compare two strings and output the differences.
 * See diff.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2021/04/03
 * - removed dependency on custom collections
 * @version 2016/10/30
 * - fixed diff flags; added punctuation flag
 * @version 2016/10/22
 * - added diffPass (for autograder assertDiff)
 * @version 2014/10/14
 * - initial version
 * @since 2014/10/14
 */

#include "privatediff.h"
#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include "privateregexpr.h"
#include "privatestrlib.h"

namespace sgl {
namespace priv {
namespace diff {

static std::string stripWhitespace(const std::string& s) {
    std::ostringstream result;
    for (size_t i = 0; i < s.length(); i++) {
        if (!isspace(s[i])) {
            result << tolower(s[i]);
        }
    }
    return result.str();
}

std::string diff(std::string s1, std::string s2, int flags) {
    std::vector<std::string> lines1 = sgl::priv::strlib::split(s1, "\n");
    std::vector<std::string> lines2 = sgl::priv::strlib::split(s2, "\n");
    std::vector<std::string> lines1Original = lines1;
    std::vector<std::string> lines2Original = lines2;

    if (flags & IGNORE_NUMBERS) {
        s1 = sgl::priv::regexpr::replace(s1, "[0-9]+", "###");
        s2 = sgl::priv::regexpr::replace(s2, "[0-9]+", "###");
        lines1 = sgl::priv::strlib::split(s1, "\n");
        lines2 = sgl::priv::strlib::split(s2, "\n");
    }
    if (flags & IGNORE_NONNUMBERS) {
        s1 = sgl::priv::regexpr::replace(s1, "[^0-9\n]+", " ");
        s2 = sgl::priv::regexpr::replace(s2, "[^0-9\n]+", " ");
        lines1 = sgl::priv::strlib::split(s1, "\n");
        lines2 = sgl::priv::strlib::split(s2, "\n");
    }
    if (flags & IGNORE_PUNCTUATION) {
        std::string punct = "[.,?!'\"()\\/#$%@^&*_\\[\\]{}|<>:;-]+";
        s1 = sgl::priv::regexpr::replace(s1, punct, "");
        s2 = sgl::priv::regexpr::replace(s2, punct, "");
        lines1 = sgl::priv::strlib::split(s1, "\n");
        lines2 = sgl::priv::strlib::split(s2, "\n");
    }
    if (flags & IGNORE_AFTERDECIMAL) {
        s1 = sgl::priv::regexpr::replace(s1, "\\.[0-9]+", ".#");
        s2 = sgl::priv::regexpr::replace(s2, "\\.[0-9]+", ".#");
        lines1 = sgl::priv::strlib::split(s1, "\n");
        lines2 = sgl::priv::strlib::split(s2, "\n");
    }
    if (flags & IGNORE_CASE) {
        s1 = sgl::priv::strlib::toLowerCase(s1);
        s2 = sgl::priv::strlib::toLowerCase(s2);
        lines1 = sgl::priv::strlib::split(s1, "\n");
        lines2 = sgl::priv::strlib::split(s2, "\n");
    }
    if (flags & IGNORE_CHARORDER) {
        std::vector<std::string> lines1Sorted;
        for (std::string line : lines1) {
            std::sort(line.begin(), line.end());
            lines1Sorted.push_back(line);
        }
        lines1 = lines1Sorted;
        s1 = sgl::priv::strlib::join(lines1, "\n");

        std::vector<std::string> lines2Sorted;
        for (std::string line : lines2) {
            std::sort(line.begin(), line.end());
            lines2Sorted.push_back(line);
        }
        lines2 = lines2Sorted;
        s2 = sgl::priv::strlib::join(lines2, "\n");
    }
    if (flags & IGNORE_LINEORDER) {
        std::sort(lines1.begin(), lines1.end());
        std::sort(lines2.begin(), lines2.end());
        s1 = sgl::priv::strlib::join(lines1, "\n");
        s2 = sgl::priv::strlib::join(lines2, "\n");
    }
    if (flags & IGNORE_WHITESPACE) {
        for (int i = 0; i < lines1.size(); i++) {
            lines1[i] = stripWhitespace(lines1[i]);
        }
        for (int i = 0; i < lines2.size(); i++) {
            lines2[i] = stripWhitespace(lines2[i]);
        }
    }

    if (sgl::priv::strlib::trimEnd(s1) == sgl::priv::strlib::trimEnd(s2)) {
        return NO_DIFFS_MESSAGE;
    }

    // build a reverse-index array using the line as key and line number as value
    // don't store blank lines, so they won't be targets of the shortest distance
    // search
    std::map<std::string, std::set<int>> reverse1;
    std::map<std::string, std::set<int>> reverse2;
    for (int i = 0; i < lines1.size(); i++) {
        std::string line = lines1[i];
        if ((int) line.length() > 0) {
            reverse1[line].insert(i);
        }
    }
    for (int i = 0; i < lines2.size(); i++) {
        std::string line = lines2[i];
        if ((int) line.length() > 0) {
            reverse2[line].insert(i);
        }
    }

    // indexes of current lines within each file
    // (start at beginning of each list)
    int index1 = 0;
    int index2 = 0;
    std::vector<int> actions;

    // walk this loop until we reach the end of one of the lists of lines
    while (index1 < lines1.size() && index2 < lines2.size()) {
        // if we have a common line, save it and go to the next
        if (lines1[index1] == lines2[index2]) {
            actions.push_back(4);
            index1++;
            index2++;
            continue;
        }
        // otherwise, find the shortest move (Manhattan-distance) from the
        // current location
        int best1 = lines1.size();
        int best2 = lines2.size();
        int s1 = index1;
        int s2 = index2;
        while ((s1 + s2 - index1 - index2) < (best1 + best2 - index1 - index2)) {
            int d = -1;
            if (lines2.size() > s2 && reverse1.find(lines2[s2]) != reverse1.end()) {
                for (int lineNumber : reverse1[lines2[s2]]) {
                    if (lineNumber >= s1) {
                        d = lineNumber;
                        break;
                    }
                }
            }
            if (d >= s1 && (d + s2 - index1 - index2) < (best1 + best2 - index1 - index2)) {
                best1 = d;
                best2 = s2;
            }

            d = -1;
            if (lines1.size() > s1 && reverse2.find(lines1[s1]) != reverse2.end()) {
                for (int lineNumber : reverse2[lines1[s1]]) {
                    if (lineNumber >= s2) {
                        d = lineNumber;
                        break;
                    }
                }
            }
            if (d >= s2 && (d + s1 - index1 - index2) < (best1 + best2 - index1 - index2)) {
                best1 = s1;
                best2 = d;
            }

            s1++;
            s2++;
        }

        while (index1 < best1) {
            actions.push_back(1);
            index1++;
        }  // deleted elements

        while (index2 < best2) {
            actions.push_back(2);
            index2++;
        }  // added elements
    }

    // we've reached the end of one list, now walk to the end of the other
    while (index1 < lines1.size()) {
        actions.push_back(1);
        index1++;
    }  // deleted elements

    if (flags & IGNORE_TRAILING) {
        while (index2 < lines2.size()) {
            actions.push_back(2);
            index2++;
        }  // added elements
    }

    // and this marks our ending point
    actions.push_back(8);

    int op = 0;
    int x0 = 0;
    int x1 = 0;
    int y0 = 0;
    int y1 = 0;
    std::vector<std::string> out;

    for (int action : actions) {
        if (action == 1) {
            op |= action;
            x1++;
            continue;
        } else if (action == 2) {
            op |= action;
            y1++;
            continue;
        }

        if (op > 0) {
            bool multipleLines = (x1 != x0 + 1);
            std::string xstr = std::string("") + (multipleLines ? (std::to_string(x0 + 1) + "-" + std::to_string(x1)) : std::to_string(x1));
            std::string ystr = std::string("") + ((y1 != y0 + 1) ? (std::to_string(y0 + 1) + "-" + std::to_string(y1)) : std::to_string(y1));
            std::string linesStr = std::string("\nLine") + (multipleLines ? "s " : " ");
            std::string doStr = std::string("do") + (multipleLines ? "" : "es");
            if (op == 1) {
                out.push_back(linesStr + xstr + " deleted near student line " + std::to_string(y1));
            } else if (op == 3) {
                if (xstr == ystr) {
                    out.push_back(linesStr + xstr + " " + doStr + " not match");
                } else {
                    out.push_back(linesStr + xstr + " changed to student line " + ystr);
                }
            }

            while (x0 < x1) {
                out.push_back("EXPECTED < " + lines1Original[x0]);
                x0++;
            }   // deleted elems

            if (op == 2) {
                if (!(flags & IGNORE_LEADING) || x1 > 0) {
                    out.push_back(linesStr + std::to_string(x1) + " added at student line " + ystr);
                }
            } else if (op == 3) {
                // out.push_back("---");
            }

            while (y0 < y1) {
                if (!(flags & IGNORE_LEADING) || op != 2 || x1 > 0) {
                    out.push_back("STUDENT  > " + lines2Original[y0]);
                }
                y0++;
            }   // added elems
        }
        x1++;
        x0 = x1;
        y1++;
        y0 = y1;
        op = 0;
    }

    if (out.size() > 0) {
        out.push_back("");
        return sgl::priv::strlib::trim(sgl::priv::strlib::join(out, "\n"));
    } else {
        return NO_DIFFS_MESSAGE;
    }
}

bool diffPass(const std::string& s1, const std::string& s2, int flags) {
    std::string diffs = diff(s1, s2, flags);
    bool result = sgl::priv::strlib::trim(diffs) == NO_DIFFS_MESSAGE;
    return result;
}

bool isDiffMatch(const std::string& diffs) {
    return sgl::priv::strlib::trim(diffs) == NO_DIFFS_MESSAGE;
}

} // namespace sgl
} // namespace priv
} // namespace diff
