#include "stdafx.h"
#include "IOCPSession.h"

IoData::IoData()
	:ioType_(IO_ERROR)
{
	ZeroMemory(&overlapped_, sizeof(overlapped_));

	this->clear();
}

void IoData::clear()
{
	buffer_.fill(0);
	totalBytes_ = 0;
	currentBytes_ = 0;
}

bool IoData::needMoreIO(size_t transferSize)
{
	currentBytes_ += transferSize;
	if (currentBytes_ < totalBytes_)
	{
		return true;
	}
	return false;
}

int32_t IoData::setupTotalBytes()
{
	packet_size_t offset = 0;
	packet_size_t packetLen[1] = { 0, };
	if (totalBytes_ == 0)
	{
		memcpy_s((void *)packetLen, sizeof(packetLen), (void *)buffer_.data(), sizeof(packetLen));
		//PacketObfuscation::getInstance().decodingHeader((Byte*)&packetLen, sizeof(packetLen));

		totalBytes_ = (size_t)packetLen[0];
	}
	offset += sizeof(packetLen);

	return offset;
}

size_t IoData::totalByte()
{
	return totalBytes_;
}

IO_OPERATION &IoData::type()
{
	return ioType_;
}

void IoData::setType(IO_OPERATION type)
{
	ioType_ = type;
}

char* IoData::data()
{
	return buffer_.data();
}

bool IoData::setData(Stream &stream)
{
	this->clear();

	if (buffer_.max_size() <= stream.size())
	{
		SLog(L"! packet size too big [%d]byte", stream.size());
		return false;
	}

	const size_t packetHeaderSize = sizeof(packet_size_t);
	packet_size_t offset = 0;

	char *buf = buffer_.data();
	//									 head size  + real data size
	packet_size_t packetLen[1] = { (packet_size_t)packetHeaderSize + (packet_size_t)stream.size(), };
	// insert packet len
	memcpy_s(buf + offset, buffer_.max_size(), (void *)packetLen, packetHeaderSize);
	offset += packetHeaderSize;

	// packet obfuscation
	//패킷 암호화 부분. 현재 암호화 사용 안함
	//PacketObfuscation::getInstance().encodingHeader((Byte*)buf, packetHeaderSize);
	//PacketObfuscation::getInstance().encodingData((Byte*)stream.data(), stream.size());

	// insert packet data
	memcpy_s(buf + offset, buffer_.max_size(), stream.data(), (int32_t)stream.size());
	offset += (packet_size_t)stream.size();

	totalBytes_ = offset;
	return true;
}

LPWSAOVERLAPPED IoData::overlapped()
{
	return &overlapped_;
}

WSABUF IoData::wsabuf()
{
	WSABUF wsaBuf;
	wsaBuf.buf = buffer_.data() + currentBytes_;
	wsaBuf.len = (ULONG)(totalBytes_ - currentBytes_);

	return wsaBuf;
}

//-----------------------------------------------------------------//
IOCPSession::IOCPSession()
	: Session()
{

	InitializeSRWLock(&lock);

	this->initialize();
}

void IOCPSession::initialize()
{
	ZeroMemory(&socketData_, sizeof(SOCKET_DATA));
	ioData_[IO_READ].setType(IO_READ);
	ioData_[IO_WRITE].setType(IO_WRITE);
	totalOffset = 0;
}

void IOCPSession::checkErrorIO(DWORD ret)
{
	if (ret == SOCKET_ERROR
		&& (WSAGetLastError() != ERROR_IO_PENDING))
	{
		SLog(L"! IOCPSession socket error: %d", WSAGetLastError());
	}
}

void IOCPSession::recv(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD recvBytes;
	DWORD errorCode = WSARecv(socketData_.socket_, &wsaBuf, 1, &recvBytes, &flags, ioData_[IO_READ].overlapped(), NULL);
	this->checkErrorIO(errorCode);
}

bool IOCPSession::isRecving(size_t transferSize)
{
	if (ioData_[IO_READ].needMoreIO(transferSize))
	{
		SLog(L"isRecving");
		this->recv(ioData_[IO_READ].wsabuf());
		return true;
	}
	return false;
}

void IOCPSession::recvStandBy()
{
	ioData_[IO_READ].clear();

	WSABUF wsaBuf;
	wsaBuf.buf = ioData_[IO_READ].data();
	wsaBuf.len = SOCKET_BUF_SIZE; //- recvBuffer.size();  고쳐야됨

	this->recv(wsaBuf);
}

void IOCPSession::send(WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD sendBytes;
	DWORD errorCode = WSASend(socketData_.socket_,
		&wsaBuf, 1, &sendBytes, flags,
		ioData_[IO_WRITE].overlapped(), NULL);

	SLog(L"Session Send Size : %d \n", sendBytes);
	this->checkErrorIO(errorCode);
}

void IOCPSession::onSend(size_t transferSize)
{
	if (ioData_[IO_WRITE].needMoreIO(transferSize))
	{
		this->send(ioData_[IO_WRITE].wsabuf());
	}
}

void IOCPSession::sendPacket(Packet *packet)
{
	Stream stream;
	packet->encode(stream);
	if (!ioData_[IO_WRITE].setData(stream))
	{
		return;
	}

	WSABUF wsaBuf;
	wsaBuf.buf = ioData_[IO_WRITE].data();
	//wsaBuf.len = (ULONG)stream.size();
	wsaBuf.len = stream.size() + 4;
	this->send(wsaBuf);
	this->recvStandBy();
}

//Package *IOCPSession::onRecv(size_t transferSize)
//{
//	// empty
//	return nullptr;
//}

Package *IOCPSession::onRecv(size_t transferSize)
{
	packet_size_t offset = 0;
	offset += ioData_[IO_READ].setupTotalBytes();

	/*
	if (this->isRecving(transferSize))
	{
		return nullptr;
	}
	*/


	memcpy_s(recvBuffer.data() + totalOffset, SOCKET_BUF_SIZE, ioData_[IO_READ].buffer_.data(), transferSize);
	totalOffset += transferSize;
	const size_t packetHeaderSize = sizeof(packet_size_t);
	while (!recvBuffer.empty())
	{

		//SLog(L"total offset increment : %d \n", totalOffset);
		if (totalOffset == 60) {
			Sleep(1000);
		}
		//		size_t *packetDataSize = (size_t *)recvBuffer.data();
		if (totalOffset < packetHeaderSize) break;

		packet_size_t* packetDataSize = (packet_size_t*)recvBuffer.data();
		// headerSize + datasize;
		if (totalOffset < *packetDataSize) break;

		Byte *packetData = (Byte*)recvBuffer.data() + packetHeaderSize;


		//PacketObfuscation::getInstance().decodingData(packetData, packetDataSize);
		Packet *packet = PacketAnalyzer::getInstance().analyzer((const char *)packetData, *packetDataSize - packetHeaderSize);
		if (packet == nullptr)
		{
			SLog(L"! invalid packet");
			this->onClose(true);
			return nullptr;
		}

		Package *package = new Package(this, packet);


		pushPackage(package);

		totalOffset -= *packetDataSize;
		memcpy_s(recvBuffer.data(), SOCKET_BUF_SIZE, recvBuffer.data() + *packetDataSize, SOCKET_BUF_SIZE - *packetDataSize);

	}

	//totalOffset = 

	this->recvStandBy();

	return nullptr;
}

void	IOCPSession::pushPackage(Package* package) {
	AcquireSRWLockExclusive(&lock);
	packageQueue.push(package);
	ReleaseSRWLockExclusive(&lock);
}


Package* IOCPSession::popPackage() {
	AcquireSRWLockExclusive(&lock);
	Package* package = packageQueue.front();
	packageQueue.pop();
	ReleaseSRWLockExclusive(&lock);
	return package;
}

bool	IOCPSession::IsEmpty() {
	AcquireSRWLockShared(&lock);
	bool value = packageQueue.empty();
	ReleaseSRWLockShared(&lock);
	return value;
}