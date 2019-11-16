//
// Created by Aleksey Timin on 11/16/19.
//

#ifndef _SRC_SESSIONINFO_H_
#define _SRC_SESSIONINFO_H_

#include <string>
namespace eipScanner {
	class SessionInfo {
	public:
		SessionInfo(const std::string &host, int port);
		~SessionInfo();
		virtual void sendAndReceive() const;
	private:
		std::string _host;
		int _port;
		int _sockedFd;
	};
}

#endif  // _SRC_SESSIONINFO_H_
