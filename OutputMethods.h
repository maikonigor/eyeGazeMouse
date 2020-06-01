#pragma once

#include <netinet/in.h>

#include "GazeTracker.h"

class AbstractStore {
public:
	virtual ~AbstractStore();
	virtual void store() = 0;
};

class MmapStore: public AbstractStore {
public:
	MmapStore(const char *filename="/tmp/gaze-mouse");
	virtual ~MmapStore();
	virtual void store();

private:
	int _fileDescriptor;
	int *_positionTable;
};

class StreamStore: public AbstractStore {
public:
	StreamStore(std::ostream &stream);
	virtual ~StreamStore();
	virtual void store();

private:
	std::ostream &_stream;
};

class SocketStore: public AbstractStore {
public:
	SocketStore(int port=20320);
	virtual ~SocketStore();
	virtual void store();

private:
	int _mySocket;
	struct sockaddr_in _destAddr;
};


