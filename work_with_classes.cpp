#include <iostream>

using namespace std;

//
class DigitOut
{
//private:     //нужно дле реализации функции внутри класса
public:
    DigitOut(){};//конструктор

    bool Off(){
        cout << "Вызывается off"<<endl;
        return true;
    };

    bool On(){
        cout << "Вызывается on"<<endl;
        return true;
    };   

    ~DigitOut(){};//деструктор
};

struct DevTable
{
    DigitOut*d1;                        //указатель на DigitOut 
    DigitOut*d2;
    DigitOut*d3;
};

class Use_Dev_Table
{
private:
    DevTable* m_devs;                                                   //protected we use only with наследование
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
   // DevTable() : d1(NULL), d2(NULL), d3(NULL) {}   //Значения равны 0 , обнуление их по сути

    DevTable *m_devs = new DevTable;    //применимам для структур без методов 
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

// getRealPdo принимает  const char   должна возвращать указатель на объект digit out


//скот маерс эффективность с++ страницы 69-