#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <iostream>
#include <QObject>
#include "modbus.h"

//单个通信工具的基础属性
template <typename transmissionUnit>
class CommunicationBaseTool {
public:
	virtual bool init() = 0;
	virtual bool close() = 0;
	virtual void writeUnitToClient(unsigned int Client, unsigned int addr, transmissionUnit value)=0;
	virtual void writeContinueUnitToClient(unsigned int Client, unsigned int addr, int length, transmissionUnit* value) = 0;
	virtual transmissionUnit* readFromClient(unsigned int Client, unsigned int addr,int length) = 0;


	CommunicationBaseTool() {};
	virtual ~CommunicationBaseTool() {};
private:
	
};

//具体通信工具的实现
template <typename transmissionUnit>
class CommunicationTool : public CommunicationBaseTool<transmissionUnit> {
public:
	virtual bool init();
	virtual bool close();
	virtual void writeUnitToClient(unsigned int Client, unsigned int addr, transmissionUnit value);
	virtual void writeContinueUnitToClient(unsigned int Client, unsigned int addr, int length, transmissionUnit* value);
	virtual void readFromClient(unsigned int Client, unsigned int addr, int length, transmissionUnit* data);
	
	void readWithWrite(unsigned int Client, int writeAddr, int writeLength, transmissionUnit* wtiteData,
											int readAddr, int readLength, transmissionUnit* readData);

	CommunicationTool()
	{
	}
	virtual ~CommunicationTool() {
		close();
	};
private:
	//必要的通信协议配置参数
	modbus_t* md_rtu;
	int masterId = 1;
	std::string comName = "COM1";
	int baud = "9600";
	char parity = 'N';
	int dataBit = 8;
	int stopBit = 1;

	int testAddr = 66;//回环测试地址
};

template <typename transmissionUnit>
class CommunicationToolProxy
{
public:
	CommunicationToolProxy()
	{
		if (tool == nullptr)
		{
			tool = new CommunicationTool;
		}
		tool->init();
	}
	~CommunicationToolProxy()
	{
		if (tool != nullptr)
		{
			delete tool;
		}
	}
	void setCameraGrabFinishSignal(int mCameraIndex);
	void setCameraResultSignal(int mCameraIndex, int mResult);
	bool getCameraTriggerSignal();

private:
	CommunicationTool<transmissionUnit>* tool = nullptr;
};



//对外给出的接口
namespace Model {
	template <typename transmissionUnit>
	class CommunicationToolInterface {
	public:
		CommunicationToolInterface() {
			if (toolProxy == nullptr)
			{
				toolProxy = new CommunicationToolProxy;
			}
		}
		~CommunicationToolInterface() {
			if (toolProxy == nullptr)
			{
				delete toolProxy;
			}
		}

		void setCameraGrabFinishSignal(int mCameraIndex);
		void setCameraResultSignal(int mCameraIndex, int mResult);
		bool getCameraTriggerSignal();

	private:
		CommunicationToolProxy<transmissionUnit>* toolProxy=nullptr;

	};
}






#endif

