/* ** *****************************************************************
* reply.h
*
* Author: Mauricio Matamoros
*
* ** *****************************************************************/
#ifndef __REPLY_H__
#define __REPLY_H__
#pragma once

/** @cond */
#include <memory>
#include <string>

#include <boost/asio.hpp>
/** @endcond */

#include "request.h"

class Reply;
typedef std::shared_ptr<Reply> ReplyPtr;

class Reply{
private:
	uint32_t cmdId;
	bool success;
	std::string result;

private:
	Reply(uint32_t cmdId, bool success=0, const std::string& result=""):
	cmdId(cmdId), success(success), result(result){}
	Reply(Reply const& obj)        = delete;
	Reply& operator=(Reply const&) = delete;

public:
	uint32_t    getCommandId() const{return cmdId;}
	bool        getSuccess() const{return success;}
	std::string getResult() const{return result;}

	bool matches(const Request& r){return r.getCommandId() == cmdId;}
	bool matches(const RequestPtr& r){return r ? r->getCommandId() == cmdId : false;}


public:
	static bool matches(const Reply& rep, const Request& req){return rep.getCommandId() == req.getCommandId();}
	static bool matches(const ReplyPtr& rep, const RequestPtr& req){
		if(!rep || !req) return false;
		return rep->getCommandId() == req->getCommandId();
	}
	static ReplyPtr fromMessage(const std::string& message){
		// Reply is: 0x00 + 4byte CmdId + 1byte success flag + Response (if any).
		if( message.length() < 6) return NULL;
		if( message[0] ) return NULL;

		uint32_t cmdId;
		message.copy((char*)&cmdId,   4, 1);

		bool success = message[5];
		std::string result = (message.length() > 6) ? message.substr(6) : "";
		return ReplyPtr(new Reply(cmdId, success, result));
	}
};

#endif //__REPLY_H__
