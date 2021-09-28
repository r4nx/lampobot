#ifndef LAMPOBOT_LUA_SCRIPT_HPP_
#define LAMPOBOT_LUA_SCRIPT_HPP_

#include "abstract_script.hpp"

#include <string>

namespace lampobot {

class LuaScript : public AbstractScript {
public:
    LuaScript(std::string source);

    std::string run(const std::string &input) override;

private:
    std::string source_;
};

} // namespace lampobot

#endif // !LAMPOBOT_LUA_SCRIPT_HPP_
