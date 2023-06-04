#include "server.hpp"

// main server

MainServer::MainServer() {
}

void	MainServer::init_server()
{
    std::cout << "constructing server.." << std::endl;
    for (std::vector<v_server>::iterator it = myvs.begin(); it != myvs.end(); it++) {
		it->listener = new Socket(AF_INET, SOCK_STREAM, INADDR_ANY, it->get_port(), 10);
    }
}

int     MainServer::is_listener(unsigned long int fd)
{
	for (std::vector<v_server>::iterator it = myvs.begin(); it != myvs.end(); it++)
		if (it->get_fd_server() == fd)
			return 1;
    return (0);
}

MainServer::~MainServer()
{
    std::cout << "server destructor called.." << std::endl;
	for (std::vector<v_server>::iterator it = myvs.begin(); it != myvs.end(); it++)
		if (it->listener != NULL)
			delete it->listener;
    std::cout << std::endl << "server shut down, take care :).." << std::endl;
}

void    MainServer::accept_new_connection(unsigned long int fd) 
{
    int newfd;
    struct sockaddr_in their_addr;
    socklen_t   addr_size;
    addr_size = sizeof their_addr;

    newfd = accept(fd, (sockaddr *)&their_addr, &addr_size);
    clients.push_back(client(newfd, inet_ntoa(their_addr.sin_addr), fd));
	for (size_t i = 0; i < max_clients; i++)  
	{  
		if( rfds[i] == 0 )
		{  
			rfds[i] = newfd;  
			std::cout << "Adding " << newfd << " to rfds array" << std::endl;
			break;
		}
	}
}

void    MainServer::destroy_connection(client *c, int event)
{
    std::cout << "client " << c->fd << " disconnected.." << std::endl << std::endl;
	for (std::vector<client>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (it->fd == c->fd) {
			close(c->fd);
			clients.erase(it);
			break ;
		}
	}
	// delete fd from rfds | wfds array.
	if ( event == -1 )
	{
		for (size_t i = 0; i < max_clients ; i++) {
			if (c->fd == wfds[i]) {
				wfds[i] = 0;
				return ;
			}
		}
	}
	for (size_t i = 0; i < max_clients ; i++) {
		if (c->fd == rfds[i]) {
			rfds[i] = 0;
			return ;
		}
	}
}

size_t MainServer::get_client_server(unsigned long int fd) {
	// std::vector<v_server>::iterator	it = myvs.begin();
	size_t tmp = 0;

	for (; tmp < myvs.size(); tmp++) {
		if (myvs[tmp].get_fd_server() == fd) {
			return tmp;
		}
	}
	return -1;
}

void    MainServer::recv_request(client *c, int i_fd)
{
    size_t pos;
    size_t rec;
    std::string tmp;

    bzero(c->recBuffer, MAX_IO_SIZE);
	rec = recv(c->fd, c->recBuffer, MAX_IO_SIZE - 1, 0);
    if (rec > 0) {
        c->client_request.append(c->recBuffer);
	}
    else {
        destroy_connection(c, -1);
        return ;
    }
    if ((pos = c->client_request.find("\r\n\r\n")) != std::string::npos) {
        if (c->header.empty()) {
            c->header = c->client_request.substr(0, pos+4);
            c->body_len = 0;
            c->chunked = 0;
			if (c->header.find("Transfer-Encoding: chunked") != std::string::npos)
				c->chunked = 1;
            else if ((pos = c->header.find("Content-Length: ")) != std::string::npos) {
				tmp = c->header;
				tmp.erase(0, pos + 16);
				c->body_len = atoi(tmp.substr(0, tmp.find("\r\n")).c_str());
			}
        }
    }
    if (!c->header.empty() && ((c->chunked == 0 && c->body_len == c->client_request.length() - c->header.length()) || (c->chunked == 1 && c->client_request.find("\r\n0\r\n\r\n") != std::string::npos))) {
		std::cout << c->client_request << std::endl;
		c->client_body = c->client_request.substr(c->header.length(), c->body_len);
		if (c->chunked == 1)
			c->ft_unchunck_body();
		int i = get_client_server(c->v_server_fd);
        handle_request(myvs[i], c);
        c->client_request.clear();
        c->header.clear();
        c->sent = 0;
		// remove fd from rfds.
		rfds[i_fd] = 0;
		// add to writefds array. 
		for (size_t i = 0; i < max_clients; i++)  
		{  
			//if position is empty
			if( wfds[i] == 0 )
			{  
				wfds[i] = c->fd;
				std::cout << "Adding " << c->fd << " to wfds array" << std::endl;
				// printf("Adding to list of sockets as %d\n" , i);
				break;
			}
		}
    }
}

void    MainServer::send_request(client *c, int i_fd) {
    int sent = 0, to_send = c->client_response.length() - c->sent;

    if (to_send > MAX_IO_SIZE)
        to_send = MAX_IO_SIZE;
    if ((sent = send(c->fd, c->client_response.c_str() + c->sent, to_send, 0)) != -1)
        c->sent += sent;
    else {
        destroy_connection(c, -2);
        return ;
    }
    if (c->sent == c->client_response.length()) {
		c->client_body.clear();
		c->client_response.clear();
		wfds[i_fd] = 0;
		// if (c->get_response().find("Connection: close") != std::string::npos) {
			destroy_connection(c, -1);
		// }
		usleep(150);
    }
}

void MainServer::run() {
	max_fd = -1;
	max_clients = 1024;
	int sock_d;

    for (size_t i = 0; i < max_clients; i++)
    {  
        rfds[i] = 0;  
        wfds[i] = 0;  
    }

	std::vector<v_server>::iterator it = myvs.begin();
	std::map<std::string, v_server> extra;

	while (it != myvs.end())
	{
		extra_map.insert(std::make_pair((*it).get_host() + ":" + std::to_string((*it).get_port()) + "#" + (*it).get_name(), *it));
		extra.insert(std::make_pair((*it).get_host() + ":" + std::to_string((*it).get_port()), *it));
		it++;
	}

    std::cout << "server up and running.." << std::endl << std::endl;
    for(;;) {
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		// Set master sockets in read fds.
		for (size_t i = 0; i < myvs.size(); i++)
		{
			sock_d = myvs[i].get_fd_server();
			std::cout << "SETTING Master socket : " << sock_d << "in readfds SET." << std::endl;
			FD_SET(sock_d,&readfds);
			if (sock_d > max_fd)
				max_fd = sock_d;
		}
		// set read fds from array of fds.
		for (size_t i = 0; i < max_clients; i++)
		{
			sock_d = rfds[i];
			if (sock_d > 0){
				std::cout << "SETTING Client socket : " << sock_d << "in readfds SET." << std::endl;
				FD_SET(sock_d,&readfds);
			}
			if (sock_d > max_fd)
				max_fd = sock_d;
		}
		// set write fds from array of fds.
		for (size_t i = 0; i < max_clients; i++)
		{
			sock_d = wfds[i];
			if (sock_d > 0)
			{std::cout << "SETTING Client socket : " << sock_d << "in writefds SET." << std::endl;
				FD_SET(sock_d,&writefds);
			}if (sock_d > max_fd)
				max_fd = sock_d;
		}
		// std::cout << "read fds array : {" ;
		// for (size_t i = 0; i < max_clients; i++)
		// {
		// 	std::cout << rfds[i] << ",";
		// }
		// std::cout << "}" << std::endl;
		// std::cout << "write fds array : {";
		// for (size_t i = 0; i < max_clients; i++)
		// {
		// 	std::cout << wfds[i] << ",";
		// }
		// std::cout << "}" << std::endl;
		std::cout << "max fd :" << max_fd << std::endl;
		activity = select( max_fd + 1 , &readfds , &writefds , NULL , NULL);
		std::cout << "select returned" << std::endl;
		if (activity < 0){
			std::cerr << RED << "select failed." << std::endl;
			continue;
		}
		for (size_t i = 0; i < myvs.size(); i++)
		{
			if (FD_ISSET(myvs[i].get_fd_server(),&readfds)){
				accept_new_connection(myvs[i].get_fd_server());
			}
		}
		for (size_t i = 0; i < max_clients; i++)
		{
			if (FD_ISSET(rfds[i],&readfds))
			{
				for (size_t j = 0; j < this->clients.size() ; j++)
				{
					if (this->clients[j].fd == rfds[i])
					{
						std::cout << "GONNA RECEIVE A REQUEST" << std::endl;
		        	    recv_request(&(this->clients[j]),i);
						break;
					}
				}
			}
			else if (FD_ISSET(wfds[i],&writefds))
			{
				for (size_t j = 0; j < this->clients.size() ; j++)
				{
					if (this->clients[j].fd == wfds[i])
					{
						std::cout << "GONNA SEND A RESPONSE" << std::endl;
		        	    send_request(&(this->clients[j]),i);
						break;
					}
				}
			}
		}
    }
}

/*
//
//
    virtual servers
//
//
*/

v_server::v_server(/* args */):default_page(""),par_error(0),page_error_val(0),listener(NULL){};
v_server::~v_server(){};

headers v_server::get_header_var(void)
{
	return (header_var);
}

std::string v_server::get_method(){
	return (req_method);
};

void v_server::set_pg_val(int val)
{
	page_error_val = val;
}

void v_server::set_client_ip(std::string str)
{
	client_ip = str;
}

void v_server::set_header(char *str){
	std::string cpy(str);
	std::vector<std::string> res;
	std::vector<std::string>::iterator it;
	std::vector<std::string> content;
	std::vector<std::string> met;
	std::string HTTP;
	char buff[100];
	time_t rawtime;
	struct tm * timeinfo;

	qu_len = 0;
	header_var.reset_header();
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (buff,80,"%a, %e %b %Y %H:%M:%S %Z",timeinfo);
	header_var.push("Date", buff);
	res = ft_split(cpy, "\r\n");

	it = res.begin();
	file_open = f_log.file;
	if (f_log.valid == 1 && (f_log.req != "PUT" && f_log.req != "POST"))
	{
		header_var.push("Content-Length", std::to_string(f_log.size));
		timeinfo = localtime (&f_log.st.st_mtime);
		strftime (buff,80,"%a, %e %b %Y %H:%M:%S %Z",timeinfo);
	}
	header_var.push("Content-Location", f_log.first_req);
	if (f_log.valid == 1)
		header_var.push("Last-Modified", buff);
	header_var.push("Content-Type", get_file_type());
	header_var.push("Transfer-Encoding", "identity");
	header_var.push("Connection", "keep-alive");
	while (it != res.end())
	{
		content = ft_split(*it, ": ");
		if (content.front() == "Accept-Encoding")
			header_var.push("Accept-Encoding", content.back());
		if (content.front() == "Connection")
			header_var.push("Connection", content.back());
		if (content.front() == "Accept-Charsets")
			header_var.push("Accept-Charsets", content.back());
		if (content.front() == "Content-Type" || content.front() == "Content-type")
			header_var.push("Content-Type", &(*it)[(*it).find(":") + 2]);
		else if (content.front() == "Authorization")
			header_var.push("Authorization", content.back());
		else if (content.front() == "Content-Language")
			header_var.push("Content-Language", content.back());
		else if (content.front() == "Host")
			header_var.push("Host", &(*it)[(*it).find(":") + 2]);
		else if (content.front() == "Location")
			header_var.push("Location", content.back());
		else if (content.front() == "Referer")
			header_var.push("Referer", content.back());
		else if (content.front() == "Retry-After")
			header_var.push("Retry-After", content.back());
		else if (content.front() == "Server")
			header_var.push("Server", content.back());
		else if (content.front() == "User-Agent")
			header_var.push("User-Agent", content.back());
		else if (content.front() == "Authorization")
			header_var.push("WWW-Authenticate", &(*it)[(*it).find(":") + 2]);
		else if (content.front() == "Content-Length" && (f_log.valid != 1 || f_log.req == "POST" || f_log.req == "PUT"))
			header_var.push("Content-Length", content.back());
		else if (content.front() == "Content-length")
			header_var.push("Content-Length", content.back());
		else if (content.front() == "Transfer-Encoding")
			header_var.push("Transfer-Encoding", content.back());
		else if (content.size() == 2 && content.front() != "Status")
			extra_headers.insert(std::make_pair(content.front(), content.back()));
		else if (content.size() > 1 && content.front() != "Status")
			extra_headers.insert(std::make_pair(content.front(),  &(*it)[(*it).find(":") + 2]));
		it++;
	}
	if (req_method == "PUT" || req_method == "POST")
	{
		if (header_var.get_key("Content-Length").size())
			qu_len = ft_atoi(header_var.get_key("Content-Length").c_str());
		else
			qu_len = 0;
	}
	else if (req_method == "OPTIONS")
		header_var.push("Allow", get_string_methods());
};

unsigned long int v_server::get_fd_server()
{
	return (this->listener->get_socket());
};

int v_server::get_pg_error(){
	return (this->page_error_val);
};
struct sockaddr_in v_server::get_server_address(){
	return (this->address);
};

int v_server::ft_access()
{
	std::string ret;
	std::map<std::string, location>::iterator it = locations.find(f_log.location);
	std::string file;
	std::string keys;
	struct stat st;
	int fd = 0;

	if (it == locations.end() || it->second.get_access().size() == 0)
		return (1);
	file = it->second.get_access();
	ret = header_var.get_key("Authorization");
	if (ret.size() != 0)
	{
		if (stat(file.c_str(), &st) == 0 && st.st_size > 0)
		{
			fd = open(file.c_str(), O_RDONLY);
			if (fd > 0)
			{
				char buff[st.st_size + 1];
				bzero(buff, st.st_size);
				read(fd, buff, st.st_size);
				close(fd);
				file.clear();
				file = buff;
				keys = base64_decode(ret);
				keys += "\n";
				if (file.find(keys) != std::string::npos)
				{
					header_var.push("Authorization", "");
					header_var.push("WWW-Authenticate", "");
					header_var.push("Set-Cookie", keys.substr(0, keys.find(':')) + "=" + keys.substr(keys.find(':') + 1, keys.length() - keys.find(':') - 2));
					return (1);
				}
			}
		}
	}
	set_pg_val(401);
	return (0);
}

bool v_server::ft_allowed()
{
	if (f_log.location.size() != 0)
	{
		location loc = locations.find(f_log.location)->second;
		std::vector<std::string> m = loc.get_methods();
		std::vector<std::string>::iterator it = m.begin();
		std::vector<std::string>::iterator e_it = m.end();
		while (it != e_it)
		{
			if (req_method == *it)
				return true;
			it++;
		}
	}
	else
	{
		std::list<std::string>::iterator it;
		std::list<std::string>::iterator e_it;
		it = methods.begin();
		e_it = methods.end();
		while (it != e_it)
		{
			if (req_method == *it)
				return true;
			it++;
		}
	}
	return false;
}

std::string v_server::options_method(){
	std::string response;
	
	header_var.push("Content-Length", "");
	f_log.set_size();
	if (f_log.valid == -1)
		header_var.set_http(204, "No Content");
	response = header_var.header_msg() + "\r\n";
	header_var.reset_header();
	return (response);
};

std::string v_server::delete_method()
{
	int i = 0;
	std::string response;
	
	header_var.push("Content-Length", "");
	header_var.push("Last-Modified", "");
	f_log.set_size();
	if (f_log.valid == -1)
		header_var.set_http(204, "No Content");
	else
	{
  		if(remove(f_log.file.c_str()) != 0)
    		header_var.set_http(202, "Accepted");
    	else
		{
			header_var.set_http(200, "OK");
			header_var.push("Content-Length", "7");
			i = 1;
		}
	}
	response = header_var.header_msg() + "\r\n";
	if (i)
		response += "Success\r\n";
	header_var.reset_header();
	return (response);
};

std::string v_server::put_method()
{
	int fd;
	std::string response;
	
	f_log.set_size();
	if ((f_log.size == 0 && f_log.valid == -1))
		header_var.set_http(201, "Created");
	fd = open(f_log.file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, S_IRWXU);
	if (fd > 0)
		pending_write(fd, qu_string);
	close(fd);
	header_var.push("Content-Length", "7");
	header_var.push("Transfer-Encoding", "");
	response = header_var.header_msg() + "\r\n" + "Success";
	header_var.reset_header();
	return (response);
}

void v_server::set_qu_string(std::string str){
	qu_string = str;	
}

void v_server::set_pwd(std::string p){
	pwd_dir = p;
};

void v_server::set_qu_len(std::string val){
	qu_len = ft_atoi(val.c_str());
};


std::string v_server::ft_cgi_exist()
{
	std::map<std::string, std::string>::reverse_iterator it = cgi.rbegin();
	std::vector<std::string> res;
	std::string line;
	std::string q_req;
	size_t num = 0;
	
	while (it != cgi.rend())
	{
		res = ft_split(it->first, "*");
		std::map<std::string, location>::iterator m = locations.find(it->first);

		if (res.size() == 2 && m->second.ft_method_check(f_log.req))
		{
			q_req = f_log.first_req.substr(0, f_log.first_req.find("?"));
			if (!ft_strncmp(res.front().c_str(), q_req.c_str(), res.front().size()))
			{
				if ((num = q_req.find(res.back())) == std::string::npos) {
					it++;
					continue ;
				}
				line = q_req.substr(num, (q_req.size() - num + 1));
				if (strcmp(line.c_str(), res.back().c_str()))
					return "";
				f_log.file = m->second.get_root() + "/" + &q_req[res.front().find_last_of("/") + 1];
				f_log.location = it->first;
				if (f_log.first_req.find("?") != std::string::npos)
					f_log.q_string = &f_log.first_req[f_log.first_req.find("?") + 1];
				f_log.s_file = q_req;
				return it->first;
			}
		}
		it++;
	}
	return "";
}

void v_server::update_location()
{
	std::map<std::string ,location>::iterator it = locations.begin();
	while (it != locations.end())
	{
		if (it->second.get_root().size() == 0)
			it->second.set_root(root);
		if (it->second.get_methods().size() == 0)
		{
			std::list<std::string>::iterator sit = methods.begin();
			while (sit != methods.end())
			{
				it->second.set_method(*sit);
				sit++;
			}
		}
		if (it->second.get_body_s().size() == 0)
			it->second.set_limit_body_size(MAX_BODY);
		it++;
	}
}

std::string v_server::get_name_headers(std::map<std::string, std::string>::iterator it)
{
	std::string ret;

	ret = it->first;

	for (size_t i = 0; i < ret.size(); i++)
	{
		if (ret[i] >= 'a' && ret[i] <= 'z')
			ret[i] -= 32;
		if (ret[i] == '-')
			ret[i] = '_';
	}
	return (ret);
}

cgi_env v_server::ft_add_other_headers()
{
	std::string tmp;
	cgi_env var(extra_headers.size());
	std::map<std::string, std::string>::iterator it = extra_headers.begin();
	
	while (it != extra_headers.end())
	{
		tmp = get_name_headers(it);
		if (var.check_key(tmp))
			var.push("HTTP_" + tmp + "=", it->second);
		it++;
	}
	return (var);
}

char ** v_server::setup_env()
{
	cgi_env var(extra_headers.size());
	std::string ret;
	std::vector<std::string> tmp;

	var = ft_add_other_headers();
	ret = header_var.get_key("WWW-Authenticate");
	if (ret.size())
	{
		tmp = ft_split(ret, " ");
		if (tmp.size() == 2)
		{
			var.push("AUTH_TYPE=", tmp.front()); // first part of it : basic or digest
    		var.push("REMOTE_USER=", tmp.back()); // second part of authanti.. the User-ID
		}
	}
    var.push("CONTENT_TYPE=", header_var.get_key("Content-Type"));
	var.push("PATH_INFO=", f_log.s_file);
    var.push("PATH_TRANSLATED=", f_log.file);
    var.push("QUERY_STRING=", f_log.q_string); //POST req after The "?"
    var.push("CONTENT_LENGTH=",  std::to_string(qu_len)); //
    var.push("REMOTE_ADDR=", client_ip); //ip of user 
    var.push("REMOTE_IDENT=", header_var.get_key("User-Agent"));
	var.push("REQUEST_METHOD=", f_log.req);
	var.push("REQUEST_URI=", f_log.first_req);
	var.push("SERVER_NAME=", name);
	var.push("SERVER_PORT=", std::to_string(port));
	var.push("SCRIPT_FILENAME=", f_log.file);
	var.push("SCRIPT_NAME=", f_log.file);
	
	// var.print_cgi();
	return (var.get_env());
}

std::string v_server::run_cgi()
{
std::map<std::string, std::string>::iterator it;
    int id;
    char store[65536] = {0};
    int ret;
    std::string response;
    FILE *Fin = tmpfile();
    FILE *Fout = tmpfile();
    char *tab[3];
    int fdin = fileno(Fin);
    int fdout = fileno(Fout);
    int out = dup(STDOUT_FILENO);
    int in = dup(STDIN_FILENO);
	std::string sent_response;
	int status = 0;


    it = cgi.find(f_log.location);
    tab[0] = (char*)it->second.c_str();
    tab[1] = (char*)file_open.c_str();
    tab[2] = NULL;
    ret = 1;
    ft_write_string(fdin, qu_string);
    lseek(fdin, 0, SEEK_SET);
    id = fork();
    if (id == -1)
        std::cerr << RED << "Fork crashed." << RESET << std::endl;
    else if (!id)
    {
        dup2(fdin, STDIN_FILENO);
        dup2(fdout, STDOUT_FILENO);
		close(fdin);
		close(fdout);
        execve(it->second.c_str(), tab, setup_env());
        std::cerr << RED << "execve crashed: "  << strerror(errno) << RESET << std::endl;
		exit(-5);
    }
    else
    {
        waitpid(-1, &status, 0);
		if (status == 0)
		{
			lseek(fdout, 0, SEEK_SET);
			ret = 1;
			while (ret > 0)
			{
				bzero(store, 65536);
				ret = read(fdout, store, 65535);
				response += store;
			}
			if (ret == -1)
				ft_write_string(2, "READ ERROR\n");
		}
    }
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    fclose(Fin);
    fclose(Fout);
    close(in);
    close(out);
    close(fdin);
    close(fdout);
	if (status || id == -1)
		return (run_error(500));
	std::string tmp = &response[response.find("\r\n\r\n") + 4];
    header_var.reset_header();
    set_header((char*)response.c_str());
    header_var.push("Accept-Charsets", "");
    header_var.push("Accept-Encoding", "");
    header_var.push("Content-Language", "");
    header_var.push("Accept-Language", "");
    header_var.push("Last-Modified", "");
	header_var.push("Date", "");
    header_var.push("Content-Length", std::to_string(tmp.size())); // size body
	cgi_status_code(response);
	sent_response = header_var.header_msg() + "\r\n" + tmp;
	return (sent_response);
}

std::string v_server::post_method()
{
	return (put_method());
}

void v_server::cgi_status_code(std::string response)
{
	std::string ret;
	std::string num;
	std::string str;

	ret = response.substr(0, response.find("\r\n"));
	if (ret.find("Status") != std::string::npos)
	{
		ret = ret.substr(ret.find(": ") + 2, ret.size());
		num = ret.substr(0, ret.find(" "));
		str = ret.substr(ret.find(" ") + 1, ret.size());
		header_var.set_http(ft_atoi(num.c_str()), str);
	}
	else
		header_var.set_http(200, "OK");
}

std::string v_server::run_method()
{
	std::string ret = ft_cgi_exist();
	
	if (ret.size())
		ret = run_cgi();
	else if (req_method == "GET" || req_method == "HEAD")
		ret = run_file();
	else if (req_method == "PUT")
		ret = put_method();
	else if (req_method == "DELETE")
		ret = delete_method();
	else if (req_method == "OPTIONS")
		ret = options_method();
	else if (req_method == "POST")
		ret = post_method();
	return (ret);
}

std::string v_server::get_file_type()
{
	std::string ret;
	
	ret = &file_open[file_open.find_last_of(".")];
	file_type = ret;
	if (ret == ".gif")
		return("image/gif");
	if (ret == ".png")
		return ("image/png");
	if (ret == ".jpg")
		return ("image/jpeg");
	if (ret == ".html")
		return ("text/html");
	if (ret == ".ico")
		return ("image/x-icon");
	if (ret == ".js")
		return ("text/javascript");
	if (ret == ".css")
		return ("text/css");
	if (ret == ".json")
		return ("text/json");
	if (ret == ".xml")
		return ("text/xml");
	return ("text/plain");
}

std::string v_server::ft_read_file(std::string val)
{
	std::string ret;
	struct stat st;
	int fd = 0;

	ret.clear();
	if (val.size() == 0 || stat(val.c_str(), &st) < 0)
		return (ret);
	if ((fd = open(val.c_str(), O_RDONLY)) < 0)
		return (ret);
	char buf[st.st_size + 1];
	if (read(fd, buf, st.st_size) < 0)
	{
		close (fd);
		return (ret);
	}
	buf[st.st_size] = '\0';
	ret = buf;
	close(fd);
	return (ret);
}

std::string v_server::run_error(int val)
{
	time_t rawtime;
	struct tm * timeinfo;
	char buff[100];
	std::string ret;
	std::string response;

	header_var.reset_header();
	header_var.set_http(val, _err._data[val]);
	ret = ft_read_file(get_error_page(val));
	if (ret.size() == 0)
		ret = _err.print_error_val(val);
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (buff,80,"%a, %e %b %Y %H:%M:%S %Z",timeinfo);
	header_var.push("Date", buff);
	header_var.push("Content-Length", std::to_string(ret.size()));
	header_var.push("Accept-Charsets", "");
	header_var.push("Accept-Encoding", "");
	header_var.push("Accept-Language", "");
	header_var.push("Content-Language", "");
	if (val == 401)
		header_var.push("WWW-Authenticate", "Basic realm=\"User Visible Realm\"");
	header_var.push("Content-Type", "text/html");
	header_var.push("Connection", "close");
	response = header_var.header_msg() + "\r\n";
	if (req_method != "HEAD")
		response += ret;
	header_var.set_http(200, "OK");
	return (response);
}

std::string v_server::run_file()
{
	std::string cc;
	std::string response;

	if (page_error_val)
		return (run_error(page_error_val));
	if (f_log.valid != 1 && f_log.auto_index)
	{
		response = send_auto(f_log.file, header_var, req_method, f_log);
		if (response.size() == 0)
			return (run_error(404));
		return response;
	}
	t_file_info s_file(f_log.file);
	if (s_file.works <= 0)
	{
		if (s_file.tab != NULL)
			free(s_file.tab);
		if (s_file.works == 0)
		{
			if (errno == EACCES)
				return (run_error(403));
			else
				return (run_error(404));
		}
		else if (s_file.works == -5)
			return (run_error(500));
		return (run_error(404));
	}
	if (req_method == "HEAD")
		header_var.push("Content-Type", "");
	header_var.push("Content-Length", std::to_string(s_file.size));
	response = header_var.header_msg() + "\r\n";
	if (req_method == "GET")
		response += s_file.tab;
	free(s_file.tab);
	return (response);
}

std::string v_server::get_string_methods()
{
	std::list<std::string>::iterator it = methods.begin();
	std::string ret;
	int i = 0;
	if (f_log.location.size() != 0 && locations.find(f_log.location) != locations.end())
		return (locations.find(f_log.location)->second.get_string_methods());
	while (it != methods.end())
	{
		if (i != 0)
			ret += ", ";
		ret += *it;
		it++;
		i = 1;
	}
	return (ret);
}

std::string v_server::get_file(std::map<std::string, location>::iterator loc, std::string s_root)
{
	std::string root = (s_root + "/");
	std::string str1;
	struct stat st;
	location a = loc->second;
	std::vector<std::string> var = a.get_default();
	std::vector<std::string>::iterator it = var.begin();

	f_log.location = loc->first;
	str1.clear();
	for ( ; it != var.end(); it++)
	{
		str1 = root;
		str1 += *it;
		if (stat(str1.c_str(), &st) != -1)
		{
			f_log.file = str1;
			f_log.set_size();
			f_log.first_req = loc->first;
			f_log.limit_body = a.get_limit();
			return (str1);
		}
	}
	f_log.file = root;
	f_log.auto_index = a.get_index();
	f_log.set_size();
	f_log.limit_body = a.get_limit();
	return (root);
}

file_log v_server::get_f_log(){
	return (this->f_log);
};

void v_server::set_method(std::string val){
	req_method = val;
};

std::string v_server::get_req_file(std::string val, std::string met)
{
	std::map<std::string, location>::iterator loc = locations.find(val);
	std::map<std::string, location>::reverse_iterator r_it;
	
	f_log.rest();
	f_log.req = met;
	if (loc != locations.end())
		return (get_file(loc, loc->second.get_root()));
	r_it = locations.rbegin();
	for ( ; r_it != locations.rend(); r_it++)
	{
		if (!ft_strncmp(val.c_str(), (r_it)->first.c_str(), (r_it)->first.size()))
		{
			f_log.file = r_it->second.get_root() + "/" + val.substr((r_it)->first.size(), 1024);
			f_log.first_req = val;
			f_log.auto_index = r_it->second.get_index();
			f_log.location = r_it->first;
			f_log.set_size();
			if (S_ISDIR(f_log.st.st_mode))
				return (get_file(locations.find(r_it->first), f_log.file));
			f_log.limit_body = r_it->second.get_limit();
			return (f_log.file);
		}
	}
	f_log.file = root + "/" + val;
	f_log.set_size();
	f_log.limit_body = limit_body;
	return (f_log.file);
}