#include <SFML/Graphics.hpp>
using namespace sf;

///////////////////////////////////����� ������//////////////////////////////////
int getCurrentMission(int x , int y)//�-��� ������ ������, ������� ������ ����� ������, � ����������� �� ���������� ������ � (���� ����� ���������� ��� ����������)
{
	int mission = 0;
	if ((x>0) && (x<400)) { mission = 0; } //�������� ������ � �����
	if (x>400) { mission = 1; } //����� �� ������ ������
	if (x>700) { mission = 2; }//2��
	if ((x>2430) && (y>750)) { mission = 3; }//� ��

	return mission;//�-��� ���������� ����� ������
}
/////////////////////////////////////����� ������/////////////////////////////////
std::string getTextMission(int currentMission) {

	std::string missionText = "";//����� ������ � ��� �������������

	switch (currentMission)//����������� ����� ������ � � ����������� �� ���� ���������� missionText ������������� ��������� �����
	{
	case 0: missionText = "\n ������ 5 ���������"; break;
	case 1: missionText = "\n ������ ��������"; break;
	case 2: missionText = "\n ���� ����� �����"; break;
	case 3: missionText = "\n ����"; break;
	}
	return missionText;//�-��� ���������� �����
};