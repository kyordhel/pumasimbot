/* ** *****************************************************************
* clipsclient.h
*
* Author: Mauricio Matamoros
*
* ** *****************************************************************/
#ifndef __CLIPS_CLIENT_H__
#define __CLIPS_CLIENT_H__
#pragma once

/** @cond */
#include <map>
#include <mutex>
#include <string>
#include <iomanip>
#include <condition_variable>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>
/** @endcond */

#include "reply.h"
#include "request.h"
#include "clipsstatus.h"

class ClipsClient;
typedef std::shared_ptr<ClipsClient> ClipsClientPtr;

/**
 * Implements a tcp client that connects to clipsserver
 */
class ClipsClient: public std::enable_shared_from_this<ClipsClient>{
	struct Private{ explicit Private() = default; };
public:
	/**
	 * Initializes a new instance of ClipsClient.
	 */
	ClipsClient(const Private&): is(&buffer), clipsStatus(NULL){}
	~ClipsClient(){disconnect();}

private:
	// Disable copy constructor and assignment op.
	/**
	 * Copy constructor disabled
	 */
	ClipsClient(ClipsClient const& obj)        = delete;
	/**
	 * Copy assignment operator disabled
	 */
	ClipsClient& operator=(ClipsClient const&) = delete;

public:
	/**
	 * Connects to ClipsServer
	 * @param  address ClipsServer IPv4 address
	 * @param  port    ClipsServer port
	 * @return         true if a connection was established, false otherwise
	 */
	bool connect(const std::string& address, uint16_t port){
		if(socketPtr) return false;
		boost::asio::ip::tcp::endpoint remote_endpoint{boost::asio::ip::address::from_string(address), port};
		socketPtr = std::make_shared<boost::asio::ip::tcp::socket>(io_service);
		try{
			socketPtr->connect(remote_endpoint);
		}
		catch(...){
			return false;
		}

		buffer.prepare(0xffff);
		beginReceive();
		serviceThreadPtr = std::shared_ptr<boost::thread>( new boost::thread(
			[this](){
					this->io_service.run();
				}
		));
		onConnected();
		return true;
	}

	/**
	 * Disconnects from ClipsServer
	 */
	void disconnect(){
		abortAllRPC();
		if(serviceThreadPtr){
			io_service.stop();
			serviceThreadPtr->join();
			onDisconnected();
		}
		socketPtr = NULL;
}

	/**
	 * Requests ClipsServer to load a file
	 * @param file Path to the clp file to load
	 */
	void loadFile(const std::string& file){rpc("load", file);}

	/**
	 * Requests ClipsServer to execute the (clear) command
	 */
	void clear(){rpc("clear");}

	/**
	 * Requests ClipsServer to execute the (reset) command
	 */
	void reset(){rpc("reset");}

	/**
	 * Requests ClipsServer to run clips, executing the (run n) command
	 * @param n Maximum number fo rules to fire.
	 *          A negative value will fire all pending rules until the agenda becomes empty.
	 *          Default: -1
	 */
	void run(int32_t n = -1){
		if( n < -1 ) n = -1;
		rpc("run", std::to_string(n));
	}

	/**
	 * Requests ClipsServer to execute the (assert fact) command
	 * @param fact The fact to assert
	 */
	void assertFact(const std::string& fact){rpc("assert", fact);}

	/**
	 * Requests ClipsServer to execute the (retract fact) command
	 * @param fact The fact to retract
	 */
	void retractFact(const std::string& fact){rpc("raw", "(retract " + fact + ")" );}

	/**
	 * Requests ClipsServer to execute a command.
	 * A command is any of
	 * 		assert   Asserts the fact given in args
	 * 		reset    Resets CLIPS
	 * 		clear    Clears CLIPS KB
	 * 		raw      Injects the string in CLIPS language contained in args
	 * 		path     Sets the working path of CLIPSServer
	 * 		print    Prints the elements specified in args (any of {facts, rules, agenda})
	 * 		watch    Toggles the watch set in args (any of {functions, globals, facts, rules})
	 * 		load     Loads the CLP or DAT file specidied in args
	 * 		run      Executes (run n) with the integer value given in args
	 * 		log      Sets the log level of CLIPSServer
	 *
	 * @param  cmd  The command to execute
	 * @param  args The command to execute
	 * @return      true if the command was successfully executed, false otherwise
	 */
	bool execute(const std::string& cmd, const std::string& args){
		static std::regex rxInt("-?\\d{1,9}");
		static std::regex rxPrint("facts|rules|agenda");
		static std::regex rxWatch("functions|globals|facts|rules");
		std::smatch match;

		if(cmd == "reset") return rpc(cmd);
		else if(cmd == "clear") return rpc(cmd);
		else if(cmd == "run") {
			if( args.empty() ) return rpc(cmd, "-1");
			if( std::regex_match(args, match, rxInt) ) return rpc(cmd, args);
			return false;
		}
		else if(cmd == "print")  return std::regex_match(args, match, rxPrint) ? rpc(cmd, args) : false;
		else if(cmd == "watch")  return std::regex_match(args, match, rxWatch) ? rpc(cmd, args) : false;
		else if(cmd == "assert") return !args.empty() ? rpc(cmd, args) : false;
		else if(cmd == "raw")    return !args.empty() ? rpc(cmd, args) : false;
		else if(cmd == "path")   return !args.empty() ? rpc(cmd, args) : false;
		else if(cmd == "load")   return !args.empty() ? rpc(cmd, args) : false;
		else if(cmd == "log")    return !args.empty() ? rpc(cmd, args) : false;
		return false;
	}

	/**
	 * Requests ClipsServer to perform a query on the KB
	 * @param  query  A string containing query to perform on CLIPS language
	 * @param  result The results yielded by CLIPS
	 * @return        true if the query was performed, false otherwise
	 */
	bool query(const std::string& query, std::string& result){return rpc("query", query, result);}

	/**
	 * Sends the given string to CLIPSServer
	 * @param s The string to send
	 */
	bool send(const std::string& s){
		if(!socketPtr || !socketPtr->is_open() ) return false;
		socketPtr->send( boost::asio::buffer(s) );
		return true;
	}

	/**
	 * Sets the working path of CLIPSServer
	 * @param  s the path where CLIPSServer should look for clp files
	 */
	bool setPath(const std::string& s){ return rpc("path " + s); }

	/**
	 * Retrieves the latest reported status sent by CLIPSServer
	 * @return       The latest reported status sent by CLIPSServer
	 */
	ClipsStatusPtr getStatus(){return clipsStatus;}

	/**
	 * Requests ClipsServer to report the active watches
	 * @return       An integer containing CLIPS watches
	 */
	uint32_t getWatches(){
		rpc("watch");
		return clipsStatus ? clipsStatus->getWatches() : -1;
	}

	/**
	 * Requests ClipsServer to toggle a watch
	 * @param  watch Any of {functions, globals, facts, rules}
	 * @return       An integer containing CLIPS watches to toggle
	 */
	uint32_t toggleWatch(const std::string& watch){
		if( (watch == "functions") || (watch == "globals") || (watch == "facts") || (watch == "rules") )
			rpc("watch", watch);
		return clipsStatus ? clipsStatus->getWatches() : -1;
	}

public:
	ClipsClientPtr getPtr(){return shared_from_this();}

	void addMessageReceivedHandler(std::function<void(const ClipsClientPtr&, const std::string&)> handler){
		if(!handler) return;
		messageReceivedHandlers.push_back(handler);
	}

	void addClipsStatusChangedHandler(std::function<void(const ClipsClientPtr&, const ClipsStatusPtr&)> handler){
		if(!handler) return;
		clipsStatusChangedHandlers.push_back(handler);
	}

	void addConnectedHandler(std::function<void(const ClipsClientPtr&)> handler){
		if(!handler) return;
		connectedHandlers.push_back(handler);
	}

	void addDisconnectedHandler(std::function<void(const ClipsClientPtr&)> handler){
		if(!handler) return;
		disconnectedHandlers.push_back(handler);
	}

#if __GNUC__ > 10
	void removeMessageReceivedHandler(std::function<void(const ClipsClientPtr&, const std::string&)> handler){
		if(!handler) return;

		typedef void(HT)(const ClipsClientPtr&, const std::string&);
		auto htarget = handler.target<HT>();
		for(auto it = messageReceivedHandlers.begin(); it != messageReceivedHandlers.end(); ++it){
			if (it->target<HT>() != htarget) continue;
			messageReceivedHandlers.erase(it);
		}
	}

	void removeClipsStatusChangedHandler(std::function<void(const ClipsClientPtr&, const ClipsStatusPtr&)> handler){
		if(!handler) return;

		typedef void(HT)(const ClipsClientPtr&, const ClipsStatusPtr&);
		auto htarget = handler.target<HT>();
		for(auto it = clipsStatusChangedHandlers.begin(); it != clipsStatusChangedHandlers.end(); ++it){
			if (it->target<HT>() != htarget) continue;
			clipsStatusChangedHandlers.erase(it);
		}
	}

	void removeConnectedHandler(std::function<void(const ClipsClientPtr&)> handler){
		if(!handler) return;

		typedef void(HT)(const ClipsClientPtr&);
		auto htarget = handler.target<HT>();
		for(auto it = connectedHandlers.begin(); it != connectedHandlers.end(); ++it){
			if (it->target<HT>() != htarget) continue;
			connectedHandlers.erase(it);
		}
	}

	void removeDisconnectedHandler(std::function<void(const ClipsClientPtr&)> handler){
		if(!handler) return;

		typedef void(HT)(const ClipsClientPtr&);
		auto htarget = handler.target<HT>();
		for(auto it = disconnectedHandlers.begin(); it != disconnectedHandlers.end(); ++it){
			if (it->target<HT>() != htarget) continue;
			disconnectedHandlers.erase(it);
		}
	}
#endif

protected:
	/**
	 * Sends the given string to CLIPSServer as a raw command to be executed by CLIPS
	 * @param s The string to send
	 */
	bool sendRaw(const std::string& s){
		uint32_t cmdId;
		return sendCommand("raw", s, cmdId);
	}


	/**
	 * Begins an asynchronous read operation
	 */
	void beginReceive(){
		boost::asio::async_read(*socketPtr, buffer,
			boost::asio::transfer_at_least(3),
			boost::bind(
				&ClipsClient::asyncReadHandler, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
			);
	}


	/**
	 * Handles asyncrhonous data reception
	 * @param error             Error code
	 * @param bytes_transferred Number of bytes transferred
	 */
	void asyncReadHandler(const boost::system::error_code& error, size_t bytes_transferred){
		if(error){
			disconnect();
			return;
		}
		int i = 1;
		size_t bytesConsumed = 0;

		do{
			// 1. Read message header to read only complete messages.
			// If header is incomplete, the bytes read are returned to the buffer
			uint16_t msgsize;
			is.read((char*)&msgsize, sizeof(msgsize));
			if(buffer.size() < (msgsize - 2) ){
				is.unget(); is.unget();
				break;
			}
			// If message size is 2 or less (empty/malformed), discard.
			if(!(msgsize-=2)) continue;
			// 2. Read the whole message. Bytes read are removed from the buffer by the istream
			std::string s(msgsize, 0);
			is.read(&s[0], msgsize);

			// 3. If the message is a command's response, process it. Else publish the read string.
			if(s[0] == 0) handleResponseMesage(s);
			else onMessageReceived(s);
			// Repeat while buffer has data
		}while(buffer.size() > 0);
		beginReceive();
	}

	/**
	 * Calls handles for received messages
	 * @param handler The received message string
	 */
	void onConnected(){
		int i = 1;
		for(auto it = connectedHandlers.begin(); it != connectedHandlers.end(); ++it){
			try{ (*it)( getPtr() ); }
			catch(int err){}
		}
	}

	/**
	 * Calls handles for received messages
	 * @param handler The received message string
	 */
	void onDisconnected(){
		for(auto it = disconnectedHandlers.begin(); it != disconnectedHandlers.end(); ++it){
			try{ (*it)( getPtr() ); }
			catch(int err){}
		}
	}

	/**
	 * Calls handles for received messages
	 * @param handler The received message string
	 */
	void onMessageReceived(const std::string& s){
		for(auto it = messageReceivedHandlers.begin(); it != messageReceivedHandlers.end(); ++it){
			try{ (*it)( getPtr(), s ); }
			catch(int err){}
		}
	}

	/**
	 * Calls handles for clips status change
	 * @param handler The new status
	 */
	void onClipsStatusChanged(){
		for(auto it = clipsStatusChangedHandlers.begin(); it != clipsStatusChangedHandlers.end(); ++it){
			try{ (*it)( getPtr(), clipsStatus ); }
			catch(int err){}
		}
	}

private:
	/**
	 * Sends the given command to ClipsServer
	 * @param command The command to send
	 * @param args    The arguments for the command
	 */
	bool sendCommand(const std::string& command, const std::string& args, uint32_t& cmdId){
		static uint32_t __cmdId = 1;
		if(!socketPtr || !socketPtr->is_open() ) return false;

		Request rq(command, args);
		cmdId = rq.getCommandId();
		socketPtr->send( boost::asio::buffer(rq.getPayload()) );

		return true;
	}

	/**
	 * Awaits until a response arrives from the server
	 * @param cmdId   The ID of the command that awaits for response
	 * @param success When this method returns contains a boolean indicating
	 *                whether the execution on the remote server was successful
	 * @param result  When this method returns contains the results produced
	 *                by the rpc call on the remote server
	 */
	bool awaitResponse(int cmdId, bool& success, std::string& result){
		std::unique_lock<std::mutex> lock(pcmutex);
		bool aborted = false;
		do{
			pccv.wait(lock, [&]{ return hasReponseArrived(cmdId, aborted); } );
		} while(!hasReponseArrived(cmdId, aborted));
		if( (pendingCommands[cmdId] == nullptr) || aborted)
			return false;
		success = pendingCommands[cmdId]->getSuccess();
		result  = pendingCommands[cmdId]->getResult();
		pendingCommands.erase(cmdId);
		return true;
	}

	/**
	 * Performs a RPC call on CLIPSServer to execute a command and synchronously
	 * awaits for the response to arrive
	 * @param cmd     The command to send and execute
	 * @param args    The arguments for the command
	 * @param result  When this method returns contains the results produced
	 *                by the rpc call on the remote server
	 * @return        true if the RPC was successfully completed, false otherwise.
	 */
	bool rpc(const std::string& cmd, const std::string& args, std::string& result){
		uint32_t cmdId = 0;
		bool success = false;
		if( !sendCommand(cmd, args, cmdId) ) {fprintf(stderr, "Failed to send command\n");return false;}
		{std::lock_guard<std::mutex> lock(pcmutex);
			pendingCommands[cmdId] = NULL;
		}
		if( !awaitResponse(cmdId, success, result) ) return false;
		return success;
	}

	bool rpc(const std::string& cmd){
		std::string result;
		return rpc(cmd, "", result);
	}

	bool rpc(const std::string& cmd, const std::string& args){
		std::string result;
		return rpc(cmd, args, result);
	}

	/**
	 * Aborts all RPC request releasing all waiting locks. To be used during disconnection.
	 */
	void abortAllRPC(){
		std::unique_lock<std::mutex> lock(pcmutex);
		pendingCommands.clear();
		lock.unlock();
		pccv.notify_all();
	}

	/**
	 * Analyzes a received response and triggers the mechanisms to complete RPCs for issued commands
	 * @param s The received message
	 */
	void handleResponseMesage(const std::string& s){
		ReplyPtr rplptr = Reply::fromMessage(s);
		if( rplptr ){
			if(rplptr->getCommandId() == -1){
				updateStatus(rplptr);
				return;
			}
			std::unique_lock<std::mutex> lock(pcmutex);
			if( !pendingCommands.count(rplptr->getCommandId()) )  return;
			pendingCommands[rplptr->getCommandId()] = rplptr;
			lock.unlock();
			pccv.notify_all();
		}
	}

	/**
	 * Checks whether the response for a given command has arrived.
	 * For use of a condition variable
	 * @param  cmdId   The id of the command
	 * @param  aborted A value indicating whether the call was aborted (e.g. disconnection)
	 * @return         true if the response has arrived, false otherwise
	 */
	bool hasReponseArrived(uint32_t cmdId, bool& aborted){
		if( pendingCommands.count(cmdId) < 1 ){
			aborted = true;
			return true;
		}
		return pendingCommands[cmdId] != nullptr;
	}

	/**
	 * Updates the status based on the info sent by CLIPSServer
	 * @param ReplyPtr A pointer to the reply object containing the status
	 */
	void updateStatus(ReplyPtr r){
		const std::string& result = r->getResult();
		if( (r->getCommandId() != -1) || !r->getSuccess() || (result.substr(0, 9) != "watching:" ))
			return;
		// clipsStatus = std::stoi( result.substr(9) );
		ClipsStatusPtr ncs = ClipsStatus::fromString(result);
		if(!ncs) return;
		clipsStatus = ncs;
		onClipsStatusChanged();
	}

private:
	/**
	 * Service required for async communications
	 */
	boost::asio::io_service io_service;

	/**
	 * Background thread to run the service
	 */
	std::shared_ptr<boost::thread> serviceThreadPtr;

	/**
	 * Pointer to the socket object used to connect to the clips server
	 */
	std::shared_ptr<boost::asio::ip::tcp::socket> socketPtr;

	/**
	 * Buffer to receive messages asynchronously
	 */
	boost::asio::streambuf buffer;

	/**
	 * Stream used to read the buffer
	 */
	std::istream is;

	/**
	 * Protection lock for the pendingCommands map
	 */
	std::mutex pcmutex;

	/**
	 * Condition variable to notify RPC waiters their response has arrived
	 */
	std::condition_variable pccv;

	/**
	 * Stores the ids of commands sent that are awaiting for a response;
	 * as well as the response when it arrives
	 */
	std::map<uint32_t, ReplyPtr> pendingCommands;

	/**
	 * Stores handler functions for message reception
	 */
	std::vector<std::function<void(const ClipsClientPtr&, const std::string&)>> messageReceivedHandlers;

	/**
	 * Stores handler functions for status changed event
	 */
	std::vector<std::function<void(const ClipsClientPtr&, const ClipsStatusPtr&)>> clipsStatusChangedHandlers;

	/**
	 * Stores handler functions for connect events
	 */
	std::vector<std::function<void(const ClipsClientPtr&)>> connectedHandlers;

	/**
	 * Stores handler functions for disconnection events
	 */
	std::vector<std::function<void(const ClipsClientPtr&)>> disconnectedHandlers;

	/**
	 * Stores CLIPS status and active watches
	 */
	ClipsStatusPtr clipsStatus;


// Facotry functions replace constructor
public:
	/**
	 * Initializes a new instance of ClipsClient and returns a shared pointer to it.
	 * @return A shared pointer to a newly created instance of ClipsClient
	 */
	static ClipsClientPtr create(){return std::make_shared<ClipsClient>(Private());}
};

#endif // __CLIPS_CLIENT_H__

