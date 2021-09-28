#include "lua_script.hpp"

#include <sol/sol.hpp>

#include <string>
#include <utility> // std::move

using namespace std::string_literals;

namespace lampobot {

LuaScript::LuaScript(std::string source) : source_{std::move(source)} {}

std::string LuaScript::run(const std::string &input)
{
    try
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base);

        lua["args"] = input;

        auto result = lua.safe_script(source_, sol::script_pass_on_error);

        if (!result.valid())
        {
            sol::error err = result;
            return err.what();
        }
        else
            return result.return_count() > 0
                       ? result
                       : "Script haven't returned any output"s;
    }
    catch (const sol::error &ex)
    {
        return "An error has occurred while executing the script. Make sure "
               "your script actually returns a string.";
    }
}

} // namespace lampobot
