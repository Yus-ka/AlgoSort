#include <iostream>

using namespace std;

//
class DigitOut
{
//private:     //����� ��� ���������� ������� ������ ������
public:
    DigitOut(){};//�����������

    bool Off(){
        cout << "���������� off"<<endl;
        return true;
    };

    bool On(){
        cout << "���������� on"<<endl;
        return true;
    };   

    ~DigitOut(){};//����������
};

struct DevTable
{
    DigitOut*d1;                        //��������� �� DigitOut 
    DigitOut*d2;
    DigitOut*d3;
};

class Use_Dev_Table
{
private:
    DevTable* m_devs;                                                   //protected we use only with ������������
public:
    Use_Dev_Table(DevTable* dev) : m_devs(dev) {}
    void run()
    {
        m_devs-> d1->On(); 
        m_devs-> d2->On(); 
        m_devs-> d3->Off(); 
    }
};

int main()
{ 
   // DevTable() : d1(NULL), d2(NULL), d3(NULL) {}   //�������� ����� 0 , ��������� �� �� ����

    DevTable *m_devs = new DevTable;    //���������� ��� �������� ��� ������� 
    m_devs->d1 = new DigitOut();
    m_devs->d2 = new DigitOut();
    m_devs->d3 = new DigitOut();

    Use_Dev_Table udt = Use_Dev_Table(m_devs);
    udt.run();

    delete m_devs->d3;
    delete m_devs->d2;
    delete m_devs->d1;
    delete m_devs;

    return 0;
    
}


// DevTable; 












// void Inicialization(DevTable &m_devs)
// {
//     m_devs.d1 = getRealDO("nameA");
//     m_devs.d2 = getRealDO("nameB");
//     m_devs.d3 = NULL; //getRealDO("nameC");
// }

// getRealPdo ���������  const char   ������ ���������� ��������� �� ������ digit out


//���� ����� ������������� �++ �������� 69-