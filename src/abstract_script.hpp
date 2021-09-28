#ifndef LAMPOBOT_ABSTRACT_SCRIPT_HPP_
#define LAMPOBOT_ABSTRACT_SCRIPT_HPP_

#include <string>

namespace lampobot {

class AbstractScript {
public:
    virtual std::string run(const std::string &input) = 0;
};

} // namespace lampobot

#endif // !LAMPOBOT_ABSTRACT_SCRIPT_HPP_
