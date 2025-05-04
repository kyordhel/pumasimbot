/* ** *****************************************************************
* request.h
*
* Author: Mauricio Matamoros
*
* ** *****************************************************************/
#ifndef __REQUEST_H__
#define __REQUEST_H__
#pragma once

/** @cond */
#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>
/** @endcond */

class Request;
typedef std::shared_ptr<Request> RequestPtr;

class Request{
private:
	uint32_t cmdId;
	std::string cmd;
	std::string args;
	static uint32_t lastCommandId;

public:
	Request(const std::string& command, const std::string& args="") :
		cmdId(++Request::lastCommandId), cmd(command), args(args){}

private:
	Request(){}
	Request(Request const& obj)        = delete;
	Request& operator=(Request const&) = delete;

public:
	uint32_t    getCommandId() const{ return cmdId; }
	std::string getCommand() const{ return cmd; }
	std::string getArgs() const{ return args; }

	std::vector<char> getPayload() const{
		std::string content = cmd;
		if( !args.empty() ) content += " " + args;

		std::vector<char> payload;
		uint16_t packetsize = 7 + content.length();
		payload.resize(packetsize, 0);
		char* buffer = payload.data();
		std::memcpy(buffer, &packetsize, 2);
		buffer[2] = 0;
		std::memcpy(buffer+3, &cmdId, 4);
		content.copy(buffer+7, content.length());
		return payload;
	}



public:
	static RequestPtr fromMessage(const std::string& message){
		if( message[0] || ( message.length() < 6) ) return NULL;

		size_t argsPos = message.find( ' ' , 5);
		if(argsPos == 5) return NULL;

		RequestPtr rqPtr = RequestPtr( new Request() );
		rqPtr->cmd = message.substr(5, argsPos);
		rqPtr->args = (argsPos != std::string::npos) ? message.substr(argsPos+1) : "";
		message.copy( (char*)&(rqPtr->cmdId), 4, 1 );
		return rqPtr;
	}

	static RequestPtr make_shared(const std::string& command, const std::string& args=""){ return RequestPtr(new Request(command, args)); }

};

uint32_t Request::lastCommandId = 0;

#endif //__REQUEST_H__
