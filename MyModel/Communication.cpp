#include "Communication.h"

template <typename transmissionUnit>
bool CommunicationBaseTool<transmissionUnit>::init()
{
	return true;
}

template <typename transmissionUnit>
bool CommunicationBaseTool<transmissionUnit>::close()
{
	return true;
}

template <typename transmissionUnit>
void CommunicationBaseTool<transmissionUnit>::writeUnitToClient(unsigned int Client, unsigned int addr, transmissionUnit value)
{
}

template <typename transmissionUnit>
void CommunicationBaseTool<transmissionUnit>::writeContinueUnitToClient(unsigned int Client, unsigned int addr, int length, transmissionUnit* value)
{
}

template <typename transmissionUnit>
transmissionUnit* CommunicationBaseTool<transmissionUnit>::readFromClient(unsigned int Client, unsigned int addr, int length)
{

}



template <typename transmissionUnit>
bool CommunicationTool<transmissionUnit>::init()
{
	md_rtu = modbus_new_rtu(comName.c_str(), baud, parity, dataBit, stopBit);   //��ͬ�Ķ˿�ֻ��ͬʱ��һ�� �����������ͷţ�����д�ڹ��캯���С�ʹ��ǰ��Ҫ��init

	if (modbus_connect(md_rtu) != 0)
	{
		printf("modbus can not connect! \n");
		return false;
	}
	modbus_set_slave(md_rtu, id);  //����modbus�ӻ���ַ 
	struct timeval t;
	t.tv_sec = 0;
	t.tv_usec = 1000000;   //����modbus��ʱʱ��Ϊ1000���� 
	modbus_set_response_timeout(md_rtu, (int)&t.tv_sec, (int)&t.tv_usec);

	//�ػ����������Ƿ�ɹ�
	modbus_write_register(md_rtu, testAddr, 1222);
	uint16_t* tab_reg = (uint16_t*)malloc(1 * sizeof(uint16_t));
	modbus_read_registers(md_rtu, testAddr, 1, tab_reg);
	if (tab_reg[0] == 1222)
	{
		free(tab_reg);
		return true;
	}
	else
	{
		free(tab_reg);
		printf("modbus slave can not connect! \n");
		return false;
	}
	return true;
}
template <typename transmissionUnit>
bool CommunicationTool<transmissionUnit>::close()
{
	modbus_free(md_rtu);
	return true;
}
template <typename transmissionUnit>
void CommunicationTool<transmissionUnit>::writeUnitToClient(unsigned int Client, unsigned int addr, transmissionUnit value)
{
	modbus_set_slave(md_rtu, Client);  //����modbus�ӻ���ַ 
	modbus_write_register(md_rtu, addr, value);
}
template <typename transmissionUnit>
void CommunicationTool<transmissionUnit>::writeContinueUnitToClient(unsigned int Client, unsigned int addr, int length, transmissionUnit* value)
{
	modbus_set_slave(md_rtu, Client);  //����modbus�ӻ���ַ 
	modbus_write_registers(md_rtu, addr, length, value);
}
template <typename transmissionUnit>
void CommunicationTool<transmissionUnit>::readFromClient(unsigned int Client, unsigned int addr, int length, transmissionUnit*data)
{
	modbus_set_slave(md_rtu, Client);  //����modbus�ӻ���ַ 
	int regs = modbus_read_registers(md_rtu, addr, length, data);
}
template <typename transmissionUnit>
void  CommunicationTool<transmissionUnit>::readWithWrite(unsigned int Client, int writeAddr, int writeLength, transmissionUnit* wtiteData,
																			  int readAddr, int readLength, transmissionUnit* readData)
{
	modbus_set_slave(md_rtu, Client);  //����modbus�ӻ���ַ 
	modbus_write_and_read_registers(md_rtu, writeAddr, writeLength, wtiteData, readAddr, readLength, readData);
}

template <typename transmissionUnit>
void CommunicationToolProxy<transmissionUnit>::setCameraGrabFinishSignal(int mCameraIndex)
{
	switch(mCameraIndex)
		case 0:
			tool->writeUnitToClient(2,1,1);//��λ��ID����ַ������ֵ��Լ������
			break;
		case 1��
			tool->writeUnitToClient(2, 2, 1); 
			break;
		default:
			break;

}
template <typename transmissionUnit>
void CommunicationToolProxy<transmissionUnit>::setCameraResultSignal(int mCameraIndex, int mResult)
{
	switch (mCameraIndex)
		case 0:
			tool->writeUnitToClient(2, 11, mResult);//��λ��ID����ַ������ֵ��Լ������
			break;
		case 1��
			tool->writeUnitToClient(2, 12, mResult); 
			break;
		default:
			break;

}
template <typename transmissionUnit>
bool CommunicationToolProxy<transmissionUnit>::getCameraTriggerSignal()
{
	static int preState = 0;
	int data=0;
	tool->readFromClient(2, 0, 1, &data);
	if (preState == 0 && data == 1)//��Ծ�任����
	{
		return true;
	}
	preState = data;
	return false;
}

template <typename transmissionUnit>
void Model::CommunicationToolInterface<transmissionUnit>::setCameraGrabFinishSignal(int mCameraIndex)
{
	toolProxy->setCameraGrabFinishSignal(mCameraIndex);
}
template <typename transmissionUnit>
void Model::CommunicationToolInterface<transmissionUnit>::setCameraResultSignal(int mCameraIndex, int mResult)
{
	toolProxy->setCameraResultSignal(mCameraIndex, mResult);
}
template <typename transmissionUnit>
bool Model::CommunicationToolInterface<transmissionUnit>::getCameraTriggerSignal()
{
	return toolProxy->getCameraTriggerSignal();
}