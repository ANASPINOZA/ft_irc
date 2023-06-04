#include "parse_config.hpp"

void	print_conf(MainServer &server)
{
	std::vector<v_server>::iterator it = server.myvs.begin();
	while (it != server.myvs.end())
	{
		(*it).print_server();
		it++;
	}
}

void parse_check(t_parse &vars)
{
	if (vars.server_state.server || vars.server_state.location)
		throw std::runtime_error(missing_close_brace("}"));
}

void parse_config(char *config_path, MainServer& server)
{
	t_parse vars = {};
	config	configs = {};
	vars.conf_file.open(config_path);
	if (vars.conf_file.is_open())
	{
		while (std::getline(vars.conf_file, vars.line))
        {
			if (!vars.line.empty())
			{
				comment_check(vars.line);
				vars.tokens = split_line(vars.line, " \t");
				if (!vars.tokens.empty())
				{
					if (vars.server_state.location)
						location_block(vars, server);
					else
						server_block(vars, server);
					vars.tokens.clear();
				}
			}
        }
		parse_check(vars);
	}
	else
		throw std::runtime_error("Error : File not found");
	vars.conf_file.close();
	return ;
}
