//
//
#include <algorithm>

#include "bot_manager_item.h"
#include "../defines/defines.h"
#include "../defines/b2b_proto.h"
#ifndef WIN32

#include <ctype.h>

void strupr(char *s)
{
    while (*s) {
        *s = toupper((unsigned char) *s);
        s++;
    }
}
		
#endif

bot_manager_item::bot_manager_item(bot_manager* mngr, const char* name, const char* work_dir)
{
	
	_mngr = mngr;
	_name = name;
	_work_dir = work_dir;
	std::string questions_filename = _work_dir + "questions.txt";
	std::string players_filename = _work_dir + "players.xml";
	int time = 30;
	this->game = Game(questions_filename, players_filename, "qna", time);

}

bot_manager_item::~bot_manager_item()
{
	
}

bool bot_manager_item::startup()
{
    disable = false;
	std::string config_filename = _work_dir + CONFIG_FILENAME;

#ifdef WIN32
	strcpy_s(_config_full_path, 2600, config_filename.c_str());
#else
	strcpy(_config_full_path, config_filename.c_str());
#endif

	FILE *f;
#ifdef WIN32
	if (fopen_s(&f, _config_full_path, "r"))
		f = NULL;
#else
	f = fopen(_config_full_path, "r");
#endif

	if (f) 
	{
		char buffer[MAX_MSG_LENGTH], *res;
		memset(buffer,0,sizeof(buffer));
		res = fgets(buffer, MAX_MSG_LENGTH, f);
		if (res)
			_message = std::string(res);
		fclose(f);
	}


	return true;
}

void bot_manager_item::cleanup()
{

}


void bot_manager_item::on_push_event(bot_exchange_format f)
{
	switch (f.pid)
	{
case BOT_EVENT_ROOM_TEXT: {
		std::string user = std::string(f[0x01]);
		std::string message = std::string(f[0x02]);
		Responses responses = game.processMessage(user, message);
		for (Responses::iterator i = responses.begin(); i != responses.end(); i++) {
			bot_exchange_format p(PLUGIN_EVENT_ROOM_TEXT);
			p << bot_value(0x01, (*i).c_str());
			std::string d = p.data();
			_mngr->deliver_event(_name.c_str(), d.c_str(), (int)d.size());
		}
	} break;
	case BOT_EVENT_ROOM_TIMER: {						
		Responses responses = game.tick();
		for (Responses::iterator i = responses.begin(); i != responses.end(); i++) {
			bot_exchange_format p(PLUGIN_EVENT_ROOM_TEXT);
			p << bot_value(0x01, (*i).c_str());
			std::string d = p.data();
			_mngr->deliver_event(_name.c_str(), d.c_str(), (int)d.size());
		}
	} break;
	case BOT_EVENT_BOT_2_BOT:
		{
			bot_exchange_format p;
			if (!p.receive(std::string(f[0x02])))
				return;

			switch (p.pid)
			{
			case BB_MODULE_INFO_RESPONSE:
				{
					// do something
				} break;
			case BB_MODULE_INFO_REQUEST:
				{
					std::string sender = std::string(p[0x01]);

					bot_exchange_format pck(BB_MODULE_INFO_RESPONSE);
					pck << bot_value (0x01, PLUGIN_UNIQID);
					pck << bot_value (0x02, INFO_DESCRIPTION);
					pck << bot_value (0x03, CMD_VERSION);

					send_to_module(sender.c_str(), pck);
				} break;
			case BB_PROCESS_COMMAND:
				{
					// do something
				} break;
			case BB_SHOW_HELP:
				{
					std::string nick = std::string(p[0x01]);
					std::string text = std::string(p[0x02]);

					if (text.empty())
						return;

					std::string msg = std::string(MSG_HEAD)
						+ std::string("\n")
						+ std::string(MSG_VERSION_HELP);

					if (!msg.empty())
					{
						bot_exchange_format pck(PLUGIN_EVENT_IM);
						pck << bot_value(0x01, nick);
						pck << bot_value(0x02, msg);

						std::string d = pck.data();
						_mngr->deliver_event(_name.c_str(), d.c_str(), (int)d.size());
					}
				} break;
			default: return;
			}
		} break;

#ifdef WIN32
	case BOT_EVENT_SETTINGS_GET:
		{
			std::string data;
			_message_copy = _message;
			_mngr->nodes->process_settings_wnd(data, _message_copy);

			bot_exchange_format p(PLUGIN_SETTINGS_GET_REPLY);
			p << bot_value (0x01, data);

			std::string tmp = p.data();
			_mngr->deliver_event(_name.c_str(), tmp.c_str(), (u_long)tmp.size());
		} break;
	case BOT_EVENT_SETTINGS_PROCESS:
		{
			std::string data;
			std::string state = std::string(f[0x01]);

			_mngr->nodes->process_state(data,state, _message_copy);

			bot_exchange_format p(PLUGIN_SETTINGS_STATE_REPLY);
			p << bot_value (0x01, data);

			std::string tmp = p.data();
			_mngr->deliver_event(_name.c_str(), tmp.c_str(), (u_long)tmp.size());
			
			apply_changes();

			
		} break;
	case BOT_EVENT_SETTINGS_APPLY:
		{
			std::string data;
			std::string state = std::string(f[0x01]);

			_mngr->nodes->process_state(data, state, _message_copy);

			bot_exchange_format p(PLUGIN_SETTINGS_STATE_REPLY);
			p << bot_value (0x01, data);

			std::string tmp = p.data();
			_mngr->deliver_event(_name.c_str(), tmp.c_str(), (u_long)tmp.size());
			
			apply_changes();

		} break;
	case BOT_EVENT_SETTINGS_CANCEL:
		{
			std::string data;
			_message_copy = _message;
			_mngr->nodes->process_settings_wnd(data, _message_copy);

			bot_exchange_format p(PLUGIN_SETTINGS_GET_REPLY);
			p << bot_value (0x01, data);

			std::string tmp = p.data();
			_mngr->deliver_event(_name.c_str(), tmp.c_str(), (u_long)tmp.size());
		} break;
#endif
	
	}
}

void bot_manager_item::send_to_module(const char *module_id, bot_exchange_format &f)
{
	bot_exchange_format p(PLUGIN_BOT_2_BOT);
	p << bot_value(0x01, module_id);
	p << bot_value(0x02, f.data());


	const std::string &d = p.data();
	_mngr->deliver_event(_name.c_str(), d.c_str(), (int)d.size());
}

void bot_manager_item::apply_changes()
{
	char tmp_buffer[MAX_MSG_LENGTH+1]; 
	int msg_len = (int)strlen(_message_copy.c_str());
	memset(tmp_buffer,0,MAX_MSG_LENGTH);
	if (msg_len < MAX_MSG_LENGTH)
	{
		memcpy(tmp_buffer,_message_copy.c_str(),msg_len);
		tmp_buffer[msg_len] = '\0';
	}
	else
	{
		memcpy(tmp_buffer,_message_copy.c_str(),MAX_MSG_LENGTH);
		tmp_buffer[MAX_MSG_LENGTH] = '\0';
	}

	_message = std::string(tmp_buffer);
	FILE * f;
#ifdef WIN32
	if (fopen_s(&f, _config_full_path, "w"))
		f = NULL;
#else
	f = fopen(_config_full_path, "w");
#endif
	if (f)
	{
		fputs(_message.c_str(),f);
		fclose(f);
	}
}
