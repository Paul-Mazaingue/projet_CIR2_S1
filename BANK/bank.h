// Code fait par Paul Mazaingue
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

private:
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

	// On affiche la fenêtre du compte client
	void BankFrame::OnAccessAccount(wxCommandEvent& event);

	// Méthode qui supprime un compte
	void BankFrame::OnDeleteAccount(wxCommandEvent& event);

	// Méthode qui créé un compte
	void BankFrame::OnCreateAccount(wxCommandEvent& event);

	// Méthode pour se déconnecter
	void BankFrame::OnLogout(wxCommandEvent& event);

	// Liste des comptes
	wxListBox* accountsListBox_;

	// Bouton page client
	wxButton* accessAccountButton_;
	wxButton* deleteAccountButton_;
	wxButton* createAccountButton_;
	wxButton* logoutButton_;

	// texte page client
	wxStaticText* clientAdressText_;
	wxStaticText* clientText_;
	wxStaticText* accountsLabel_;

	// Indice du compte sélectionné
	int selectedIndexAccount_;

	// Texte page compte
	wxStaticText* clientText2_;
	wxStaticText* accountText_;
	wxStaticText* balanceText_;
	wxStaticText* rateText_;

	// Bouton page compte
	wxButton* addButton_;
	wxButton* withdrawButton_;
	wxButton* TransferButton_;
	wxButton* transactionButton_;
	wxButton* changeAccountButton_;

	// Méthode pour revenir à la page client
	void OnChangeAccount(wxCommandEvent& event);

	// Méthode pour ajouter de l'argent
	void BankFrame::OnAddAccount(wxCommandEvent& event);

	// Méthode pour retirer de l'argent
	void BankFrame::OnWithdrawAccount(wxCommandEvent& event);

	// Méthode pour faire un virement
	void BankFrame::OnTransferAccount(wxCommandEvent& event);

	// Méthode pour générer un numéro de client
	int BankFrame::GenerateClientNumber();

	// Méthode pour récupèrer les info d'un client
	bool BankFrame::ClientInfo(wxString clientNumber, wxString password_);

	// Méthode pour sauvegarder les informations d'un client
	void BankFrame::ClientInfoSave();

	// Méthode pour afficher la liste des transactions d'un compte
	void BankFrame::OnTransactionAccount(wxCommandEvent& event);

	// Méthode pour récupèrer les info d'un compte pour le transfert
	bool BankFrame::ClientInfoTransfer(wxString clientNumber);

	// Donnée du compte pour le transfert
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

	// Méthode pour sauvegarder les information du compte pour le transfert
	void BankFrame::ClientInfoSaveTransfer();

	// Méthode qui gère la fermeture de la fenêtre
	void BankFrame::OnClose(wxCloseEvent& evt);

	// Méthode qui vérifie si un numéro de client existe
	bool BankFrame::IsPresent(int clientNumber);

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