#ifndef _CHAT_HPP
#define _CHAT_HPP

#include <iostream>
#include <string>
#include <vector>

#include "GUI/GUIText.hpp"
#include "GUI/GUIInput.hpp"

using namespace std;

class Chat {
private:
	// private stuff
	vector<GUIElement*> chatlog;
	bool m_isActive = true;
	unsigned int msg_index = 0;
	GUI* gui = nullptr;
public:
	// public parts
	Chat(GUI* g) : gui(g) {
		if(gui != nullptr) gui->addInput("chat", new GUIInput());
		gui->getInput("chat")->setY(600-32);
		deActivate();
	}
	virtual ~Chat() {

	}

	inline const void addLog(const string& msg) {
		gui->addText("chatlog"+to_string(++msg_index), new GUIText());
		chatlog.push_back(
			gui->getText("chatlog"+to_string(msg_index))->setText(msg)->setAlign(TEXT_ALIGN::OVER)
			);
		if(m_isActive) Activate();
		else deActivate();
		// if chatlog has more than entries remove first
		if(chatlog.size() > 15) {
			gui->removeText("chatlog"+to_string(msg_index-15));
		} 
		updatePos();
	}
	inline const void updatePos() {
		int offset = chatlog.size()*24;
		for(auto& msg : chatlog) {
			msg->setY(600-offset);
			offset -= 24;
		}
	}
	inline const bool& isActive() const { return m_isActive; }
	inline const void Activate() {
		for(auto& it : chatlog) it->show();
		gui->getInput("chat")->show();
		gui->getInput("chat")->setActive(true); 
		m_isActive = true; 
	}
	inline const void deActivate() {
		for(auto& it : chatlog) it->hide();
		gui->getInput("chat")->hide();
		gui->getInput("chat")->setActive(false);    
		m_isActive = false; 
	}
};

#endif