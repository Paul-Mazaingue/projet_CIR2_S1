#include <iostream>
#include <wx/wx.h>
#include <random>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>

// Fenêtre de la banque
class BankFrame : public wxFrame
{
public:
	// Constructeur
	BankFrame(const wxString& title);

	// Lorsqu'un serveur est sélectionné
	void OnServer(wxCommandEvent& event);

	// Lorsque l'on veut se connecter
	void Connexion(wxCommandEvent& event);

	// Lorsque l'on veut créer un compte
	void Creation(wxCommandEvent& event);

	// On affiche la fenêtre du compte client
	void ClientDisplay();

	// On affiche la fenêtre du compte client
	void AccountDisplay();

	void BankFrame::OnAccessAccount(wxCommandEvent& event);
	void BankFrame::OnDeleteAccount(wxCommandEvent& event);
	void BankFrame::OnCreateAccount(wxCommandEvent& event);
	void BankFrame::OnLogout(wxCommandEvent& event);
	wxListBox* accountsListBox_;
	wxButton* accessAccountButton_;
	wxButton* deleteAccountButton_;
	wxButton* createAccountButton_;
	wxButton* logoutButton_;
	wxStaticText* clientAdressText_;
	wxStaticText* clientText_;
	int selectedIndexAccount_;
	wxStaticText* clientText2_;
	wxStaticText* accountText_;
	wxStaticText* balanceText_;
	wxStaticText* rateText_;
	wxButton* addButton_;
	wxButton* withdrawButton_;
	wxButton* TransferButton_;
	wxButton* transactionButton_;
	wxButton* changeAccountButton_;
	void OnChangeAccount(wxCommandEvent& event);
	void BankFrame::OnAddAccount(wxCommandEvent& event);
	void BankFrame::OnWithdrawAccount(wxCommandEvent& event);
	void BankFrame::OnTransferAccount(wxCommandEvent& event);
	int BankFrame::GenerateClientNumber();
	bool BankFrame::ClientInfo(wxString clientNumber, wxString password_);
	void BankFrame::ClientInfoSave();
	wxStaticText* accountsLabel_;
	void BankFrame::OnTransactionAccount(wxCommandEvent& event);
	bool BankFrame::ClientInfoTransfer(wxString clientNumber);
	wxString firstNameTransfer_;
	wxString lastNameTransfer_;
	wxString addressTransfer_;
	wxString passwordTransfer_;
	double clientNumberTransfer_;
	wxArrayString accountNameTransfer_;
	wxArrayInt accountTypeTransfer_;
	wxArrayDouble balanceTransfer_;
	wxArrayString transactionListTransfer_;
	wxArrayInt transactionListAccountTransfer_;
	int selectedIndexAccountTransfer_;
	void BankFrame::ClientInfoSaveTransfer();
	void BankFrame::OnClose(wxCloseEvent& evt);
	bool BankFrame::IsPresent(int clientNumber);
private:
	// Variable du client
	wxString firstName_;
	wxString lastName_;
	wxString address_;
	wxString password_;
	double clientNumber_;
	double rate_;
	int server_;
	wxArrayString accountName_;
	wxArrayInt accountType_;
	wxArrayDouble balance_;
	wxArrayString transactionList_;
	wxArrayInt transactionListAccount_;
	

	// Le panneau sur lequel on va mettre les élements
	wxPanel* panel_;

	// Le message de bienvenue 
	wxStaticText* welcome_;

	// Les boutons afin de choisir le serveur
	wxButton* server1_;
	wxButton* server2_;
	wxButton* server3_;

	// Les boutons pour se connecter ou créer un compte
	wxButton* connexion_button_;
	wxButton* creation_button_;
};

// L'application de la banque
class BankApp : public wxApp
{
public:
	// Lorsque l'on lance l'application
	virtual bool OnInit();
};