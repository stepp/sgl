/*
 * File: privateregexpr.h
 * ----------------------
 * This file exports functions for performing regular expression operations
 * on C++ strings.  It will be unnecessary once the C++11 regex library
 * is widely available, but as of this writing the regex library is not
 * supported on gcc and other major C++ compilers.
 * This functionality is considered "private" and not to be used by students.
 *
 * @author Marty Stepp
 * @version 2021/04/09
 * - moved to private SGL namespace
 * - renamed functions to remove 'regex' prefix
 * @version 2021/04/03
 * - removed dependency on custom collections
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2018/09/20
 * - added Qt version checking around some regex functions for compatibility
 * @version 2014/10/14
 * - removed regexMatchCountWithLines for simplicity
 * @since 2014/03/01
 */


#ifndef _private_regexpr_h
#define _private_regexpr_h

#include <string>
#include <vector>

namespace sgl {
namespace priv {
namespace regexpr {

/**
 * Returns true if the given string s matches the given regular expression
 * as a substring.
 * For an exact match instead, put "^" and "$" in your regex as start/end anchors.
 */
bool match(const std::string& s, const std::string& regexp);

/**
 * Returns the number of times the given regular expression is found inside
 * the given string s.
 * Returns 0 if there are no matches for the regexp.
 */
int matchCount(const std::string& s, const std::string& regexp);

/**
 * Finds all matches of the given regular expression in the given string s
 * and fills 'linesOut' with a comma-separated string representing the line
 * numbers within the string at which the matches occur, such as "2,14,27,36".
 * This is mainly useful for grading programs.
 * Returns the number of times the given regular expression is found inside
 * the given string s.  Returns 0 if there are no matches for the regexp.
 */
int matchCountWithLines(const std::string& s, const std::string& regexp,
                        std::string& linesOut);

/**
 * Finds all matches of the given regular expression in the given string s
 * and fills 'linesOut' with the line numbers within the string at which the
 * matches occur, such as [2, 14, 27, 36].
 * This is mainly useful for grading programs.
 * Returns the number of times the given regular expression is found inside
 * the given string s.  Returns 0 if there are no matches for the regexp.
 */
void matchCountWithLines(const std::string& s, const std::string& regexp,
                         std::vector<int>& linesOut);

/**
 * Replaces occurrences of the given regular expression in s with the given
 * replacement text, and returns the resulting string.
 * If limit >  0 is passed, replaces that many occurrences of the regex.
 * If limit <= 0 is passed (or omitted), all occurrences are replaced.
 */
std::string replace(const std::string& s, const std::string& regexp,
                   const std::string& replacement, int limit = -1);

} // namespace regexpr
} // namespace priv
} // namespace sgl

#endif // _private_regexpr_h
