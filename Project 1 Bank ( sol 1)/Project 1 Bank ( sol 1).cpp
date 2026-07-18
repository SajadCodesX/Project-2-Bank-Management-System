#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>

using namespace std;

const string ClientsFileName = "Clients.txt";

void ShowMainMenue();
void ShowTransactionsMenue();
enum enMainMenuOptions
{
    eListClients = 1,
    eAddNewClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    enTransactions = 6,
    eExit = 7
};

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

vector <string> SplitString(string S1, string Delim)
{
    vector <string> vString;
    int pos;
    string Sword;

    while ((pos = S1.find(Delim)) != string::npos)
    {
        Sword = S1.substr(0, pos);
        if (Sword != "")
        {
            vString.push_back(Sword);
        }
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }
    return vString;
}

sClient ConvertLineDataToRecord(string Line, string Sperertor = "#//#")
{
    sClient Client;
    vector <string> vString = SplitString(Line, Sperertor);

    Client.AccountNumber = vString[0];
    Client.PinCode = vString[1];
    Client.Name = vString[2];
    Client.Phone = vString[3];
    Client.AccountBalance = stod(vString[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string Speretor = "#//#")
{
    string stDataLine = "";

    stDataLine += Client.AccountNumber + Speretor;
    stDataLine += Client.PinCode + Speretor;
    stDataLine += Client.Name + Speretor;
    stDataLine += Client.Phone + Speretor;
    stDataLine += to_string(Client.AccountBalance);

    return stDataLine;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClient;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        sClient Client;
        string Line;
        while (getline(MyFile, Line))
        {
            Client = ConvertLineDataToRecord(Line);
            vClient.push_back(Client);
        }
        MyFile.close();
    }
    return vClient;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient>& vClient)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        string Line;

        for (sClient& C : vClient)
        {
            if (C.MarkForDelete == false)
            {
                Line = ConvertRecordToLine(C);
                MyFile << Line << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber;
    cout << "\nEnter Account Number? ";
    getline(cin >> ws, AccountNumber);
    return AccountNumber;
}

bool ClientExistsByAccountNumber(string FileName, string AccountNumber)
{
    vector <sClient> vClient;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    sClient Client;
    string Line;

    if (MyFile.is_open())
    {
        while (getline(MyFile, Line))
        {
            Client = ConvertLineDataToRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}
sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(ClientsFileName, Client.AccountNumber))
    {
        cout << "Client with [" << Client.AccountNumber << "] already exists ,"
            << "Enter another Account Number? ";

        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "\nEnter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

sClient ReadChangeClient(string AccountNumber)
{
    sClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

void PrintClientCard(sClient CLient)
{
    cout << "\nThe following are the client details:\n";
    cout << "----------------------------------\n";
    cout << "AccountNumber: " << CLient.AccountNumber << endl;
    cout << "Pin Code     : " << CLient.PinCode << endl;
    cout << "Name         : " << CLient.Name << endl;
    cout << "Phone        : " << CLient.Phone << endl;
    cout << "AcoountBalance: " << CLient.AccountBalance << endl;
    cout << "----------------------------------\n";
}

void PrintClientRecord(sClient& Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> &vClient, sClient& CLient)
{
    for (sClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            CLient = C;
            return true;
        }
    }
    return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClient)
{
    for (sClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClient)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClientCard(Client);

        cout << "\nAre you sure you want delete this client? y/n ?  ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClient);
            SaveCleintsDataToFile(ClientsFileName, vClient);

            vClient = LoadCleintsDataFromFile(ClientsFileName);
            cout << "\nClient Deleted Successfully.\n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!";
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClient)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClientCard(Client);

        cout << "\nAre you sure you want Update this client? y/n ?  ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClient)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ReadChangeClient(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClient);

            cout << "\nClient Update Successfully.\n";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!";
        return false;
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    vector <sClient> VClient = LoadCleintsDataFromFile(ClientsFileName);
    char AddMore = 'y';
    cout << "Adding New Client:\n\n";
    do
    {
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}


void ShowDeleteClientScreen()
{
    system("cls");

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "-----------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();
    vector <sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
    sClient  Client;

    DeleteClientByAccountNumber(AccountNumber, vClient);
}

void ShowUpdateClientScreen()
{

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Screen\n";
    cout << "-----------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();
    vector <sClient>vClient = LoadCleintsDataFromFile(ClientsFileName);
    sClient  Client;

    UpdateClientByAccountNumber(AccountNumber, vClient);
}

void ShowAddNewClientsScreen()
{
    cout << "----------------------------------\n";
    cout << "\tAdd New Clients Screen\n";
    cout << "----------------------------------\n";

    AddNewClients();
}

void ShowFindClientScreen()
{
    vector <sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
    sClient CLient;
    cout << "\n--------------------------------\n";
    cout << "\tFind Client Screen\n";
    cout << "--------------------------------\n";

    string AccountNumber = ReadClientAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClient, CLient))
    {
        PrintClientCard(CLient);
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") Not Found!";
    }
}

void ShowAllClientsScreen()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________________________________\n" << endl;

    if (vClients.size() == 0)
    {
        cout << "\t\t\t\tNo Clients Available In the System!";

    }

    else
    {
        for (sClient& Client : vClients)
        {
            PrintClientRecord(Client);
            cout << endl;
        }
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________________________________\n" << endl;
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}

void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
  
    system("pause>0");
    ShowTransactionsMenue();

}

void ShowExitScreen()
{
    cout << "\n__________________________________\n";
    cout << "\n\tProgram Ends :-)\n";
    cout << "__________________________________\n";

    system("pause>0");
}

short ReadChoice()
{
    short Choice;

    cout << "\nChoose what do you want to do? [1 to 7] ? ";
    cin >> Choice;

    return Choice;
}
short ReadChoiceTransactions()
{
    short Choice;

    cout << "\nChoose what do you want to do? [1 to 4] ? ";
    cin >> Choice;

    return Choice;
}


double ReadDepositAmount()
{
    double DepositCost;
    cout << "\nPlease enter Deposit amount ? ";
    cin >> DepositCost;
    return DepositCost;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double DepositCost, vector <sClient>& VClient)
{

    char Answer = 'n';

    double NewBalance;


    cout << "\nAre you sure you want to perform this transaction? (Y/N): ";
    cin >> Answer;
if (Answer == 'y' || Answer == 'Y')
{

    for (sClient& C : VClient)
    {

        if (C.AccountNumber == AccountNumber)
        {

            C.AccountBalance += DepositCost;
            NewBalance = C.AccountBalance;
            SaveCleintsDataToFile(ClientsFileName, VClient);

            cout << "\nDone Successfully. New balance is: " << NewBalance << endl;
            
            return true;
        }

    }
    return false;

}
return false;
}

void ShowDepositClientScreen()
{

    cout << "\n--------------------------------\n";
    cout << "\tDeposit Client Screen\n";
    cout << "--------------------------------\n";


    vector <sClient> VClient = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    double amount = 0;


    while (!FindClientByAccountNumber(AccountNumber, VClient, Client))
    {
        cout << "\nClient with  [" << AccountNumber << "] dose not exist. ";

        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);


    amount = ReadDepositAmount();

    DepositBalanceToClientByAccountNumber(AccountNumber, amount, VClient);

}


    void ShowWithdrawClientScreen()
    {

        cout << "\n--------------------------------\n";
        cout << "\twithdraw Client Screen\n";
        cout << "--------------------------------\n";


        vector <sClient> VClient = LoadCleintsDataFromFile(ClientsFileName);
        sClient Client;
        string AccountNumber = ReadClientAccountNumber();
        

        while (!FindClientByAccountNumber(AccountNumber, VClient, Client))
        {
            cout << "\nClient with  [" << AccountNumber << "] dose not exist. ";

            AccountNumber = ReadClientAccountNumber();
        }

        PrintClientCard(Client);

        double amount = 0;
        amount = ReadDepositAmount();

        while (amount > Client.AccountBalance)
        {
            cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
            cout << "Please enter another amount? ";
            cin >> amount;
        }


      DepositBalanceToClientByAccountNumber(AccountNumber, amount * -1, VClient);

    }
    

void ShowTotalBalances()
{
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________________________________\n" << endl;

    double  TotalBalances = 0;

    if (vClients.size() == 0)
    {
        cout << "\t\t\t\tNo Clients Available In the System!";

    }

    else
    {
        for (sClient& Client : vClients)
        {
            PrintClientRecord(Client);
            TotalBalances += Client.AccountBalance;
            cout << endl;
        }
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________________________________\n" << endl;
    cout << "\n\t\t\t\t\tTotal Balances = " << TotalBalances << endl;
}

void  ShowTotalBalancesScreen()

{
    
    ShowTotalBalances();

}
enum enTransactionsMenuOptions
{
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBalances = 3,
   eMainMenu = 4
};

void TransactionsMenuOption(enTransactionsMenuOptions TransactionsMenuOptions)
{
    switch (TransactionsMenuOptions)
    {
    case enTransactionsMenuOptions::eDeposit:
        system("cls");
        ShowDepositClientScreen();
        GoBackToTransactionsMenue();
        break;

    case  enTransactionsMenuOptions::eWithdraw:
        system("cls");
        ShowWithdrawClientScreen();
        GoBackToTransactionsMenue();
        break;

    case  enTransactionsMenuOptions::eTotalBalances:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;

    case  enTransactionsMenuOptions::eMainMenu:
        ShowMainMenue();
       
   
    }


}

void ShowTransactionsMenue()
{

    system("cls");
    cout << "=================================================\n";
    cout << "\t\tTransactions Menue Screen\t\t\t\n";
    cout << "=================================================\n";
    cout << "\t  [1] Deposit.\n";
    cout << "\t  [2] Withdraw.\n";
    cout << "\t  [3] TotalBalances.\n";
    cout << "\t  [4] MainMenu.\n";
   
    cout << "================================================\n";

    TransactionsMenuOption((enTransactionsMenuOptions)ReadChoiceTransactions());
   

}


void PerformMainMenuOption(enMainMenuOptions  MainMenuOptions)
{


    switch (MainMenuOptions)
    {
    case enMainMenuOptions::eListClients:
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenuOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenuOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenuOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenuOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    case enMainMenuOptions::enTransactions:
        system("cls");
        ShowTransactionsMenue();
        GoBackToMainMenue();
        break;

    case enMainMenuOptions::eExit:
        system("cls");
        ShowExitScreen();
        break;

    }
}   
void ShowMainMenue()
{
    system("cls");
    cout << "=================================================\n";
    cout << "\t\tMain Menue Screen\t\t\t\n";
    cout << "=================================================\n";
    cout << "\t  [1] Show Client List.\n";
    cout << "\t  [2] Add New Client.\n";
    cout << "\t  [3] Delete Client.\n";
    cout << "\t  [4] Update Client Info.\n";
    cout << "\t  [5] Find Client.\n";
    cout << "\t  [6] Transactions.\n";
    cout << "\t  [7] Exit.\n";
    cout << "================================================\n";

    PerformMainMenuOption((enMainMenuOptions)ReadChoice());

}

int main()
{
    ShowMainMenue();

    return 0;
}