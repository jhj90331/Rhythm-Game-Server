#pragma once

// �������� ���⼭ �Ļ��ؼ� �����մϴ�.

#define MAX_PACKET_THREAD_		SIZE_64

class ContentsProcess
{
private:
	std::array<Thread *, MAX_PACKET_THREAD_> threadPool_;
	ThreadJobQueue<Package *> *packageQueue_;

protected:
	typedef void(*RunFunc)(Session *session, Packet *rowPacket);
	std::unordered_map<PacketType, RunFunc> runFuncTable_;

private:
	/// �ʱ�ȭ
	void initialize(xml_t *config);
	/// ��Ŷ �Լ� ��� (�ּ� ����)
	void registDefaultPacketFunc();
	/// ��Ű���� ��Ŷ �Լ��� ó��
	void run(Package *package);
	/// ��Ű�� ó��
	void execute();

	/// ������ ������ �Լ�
	void process();
	
public:
	ContentsProcess();
	virtual ~ContentsProcess();

	/// ��Ű�� ť�� ��� �Լ�
	void putPackage(Package *package);

	/// ���� ��Ŷ �Լ� ���
	virtual void registSubPacketFunc() = 0;

	//--------------------------------------------------------------//
	// �⺻ ��Ŷ ��� ����
	static void Packet_HeartBeat(Session *session, Packet *rowPacket);
	static void Packet_Notify_Terminal(Session *session, Packet *rowPacket);
	static void C_REQ_EXIT(Session *session, Packet *rowPacket);
};