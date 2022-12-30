#include "Banque_APP.h"

using namespace std;

// Fenêtre de la banque
BankFrame::BankFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title)
{
	// Initialisation par défaut
	rate_ = 0;
	clientNumber_ = -1;
	server_ = -1;
	accountsListBox_ = nullptr;
	accessAccountButton_ = nullptr;
	deleteAccountButton_ = nullptr;
	createAccountButton_ = nullptr;
	logoutButton_ = nullptr;
	clientAdressText_ = nullptr;
	clientText_ = nullptr;
	selectedIndexAccount_ = -1;
	clientText2_ = nullptr;
	accountText_ = nullptr;
	balanceText_ = nullptr;
	rateText_ = nullptr;
	addButton_ = nullptr;
	withdrawButton_ = nullptr;
	TransferButton_ = nullptr;
	transactionButton_ = nullptr;
	changeAccountButton_ = nullptr;
	accountsLabel_ = nullptr;
	firstNameTransfer_ = "";
	lastNameTransfer_ = "";
	addressTransfer_ = "";
	passwordTransfer_ = "";
	clientNumberTransfer_ = 0;
	selectedIndexAccountTransfer_ = -1;
	
	// Le panneau sur lequel on va mettre les élements
	panel_ = new wxPanel(this);
	
	// Le message de bienvenue 
	welcome_ = new wxStaticText(panel_, wxID_ANY, "Texte de bienvenue\ndans lequel on peut mettre du texte", wxPoint(100, 100), wxSize(600, -1), wxALIGN_CENTER_HORIZONTAL);
	// On change la taille de la police
	wxFont font = welcome_->GetFont();
	font.SetPointSize(24);
	welcome_->SetFont(font);
	
	// Les boutons afin de choisir le serveur
	server1_ = new wxButton(panel_, wxID_ANY, "Server 1", wxPoint(100, 275), wxSize(150, 50));
	server2_ = new wxButton(panel_, wxID_ANY, "Server 2", wxPoint(325, 275), wxSize(150, 50));
	server3_ = new wxButton(panel_, wxID_ANY, "Server 3", wxPoint(550, 275), wxSize(150, 50));
	server1_->Bind(wxEVT_BUTTON, &BankFrame::OnServer, this);
	server2_->Bind(wxEVT_BUTTON, &BankFrame::OnServer, this);
	server3_->Bind(wxEVT_BUTTON, &BankFrame::OnServer, this);

	// Les boutons pour se connecter ou créer un compte
	connexion_button_ = new wxButton(panel_, wxID_ANY, "Se connecter", wxPoint(150, 275), wxSize(200, 50));
	creation_button_ = new wxButton(panel_, wxID_ANY, "Créer un compte", wxPoint(475, 275), wxSize(200, 50));
	connexion_button_->Bind(wxEVT_BUTTON, &BankFrame::Connexion, this);
	creation_button_->Bind(wxEVT_BUTTON, &BankFrame::Creation, this);
	// On cache les boutons au début
	connexion_button_->Show(false);
	creation_button_->Show(false);

	// Pour le debug, à supprimer à la fin
	CreateStatusBar();

	// Quand la fenêtre se ferme
	this->Bind(wxEVT_CLOSE_WINDOW, &BankFrame::OnClose, this);
}

void BankFrame::AccountDisplay() {
	accountsListBox_->Show(false);
	accessAccountButton_->Show(false);
	deleteAccountButton_->Show(false);
	createAccountButton_->Show(false);
	logoutButton_->Show(false);
	clientAdressText_->Show(false);
	clientText_->Show(false);
	accountsLabel_->Show(false);

	wxString clientText2 = wxString::Format("%s %s", firstName_, lastName_);
	clientText2_ = new wxStaticText(panel_, wxID_ANY, clientText2, wxPoint(325, 30), wxSize(150, -1), wxALIGN_CENTER);

	wxString accountText = wxString::Format("Nom du compte : %s", accountName_[selectedIndexAccount_]);
	accountText_ = new wxStaticText(panel_, wxID_ANY, accountText, wxPoint(200, 110), wxSize(400, -1), wxALIGN_CENTER);

	
	wxString balanceText = wxString::Format("Solde : %.2f", balance_[selectedIndexAccount_]);
	balanceText_ = new wxStaticText(panel_, wxID_ANY, balanceText, wxPoint(200, 200), wxSize(400, -1), wxALIGN_CENTER);

	wxString rateText = wxString::Format("Taux : %.2f", rate_);
	rateText_ = new wxStaticText(panel_, wxID_ANY, rateText, wxPoint(600, 30), wxSize(200, -1), wxALIGN_CENTER);
	

	wxFont font = clientText2_->GetFont();
	font.SetPointSize(12); // Set the font size here
	clientText2_->SetFont(font);

	font = accountText_->GetFont();
	font.SetPointSize(18); // Set the font size here
	accountText_->SetFont(font);

	font = balanceText_->GetFont();
	font.SetPointSize(18); // Set the font size here
	balanceText_->SetFont(font);

	font = rateText_->GetFont();
	font.SetPointSize(18); // Set the font size here
	rateText_->SetFont(font);

	if (!accountType_[selectedIndexAccount_]) {
		rateText_->Show(false);
	}

	addButton_ = new wxButton(panel_, wxID_ANY, "Ajouter", wxPoint(50, 300), wxSize(200, 50));
	withdrawButton_ = new wxButton(panel_, wxID_ANY, "Retirer", wxPoint(300, 300), wxSize(200, 50));
	TransferButton_ = new wxButton(panel_, wxID_ANY, "Transférer", wxPoint(550, 300), wxSize(200, 50));
	addButton_->Bind(wxEVT_BUTTON, &BankFrame::OnAddAccount, this);
	withdrawButton_->Bind(wxEVT_BUTTON, &BankFrame::OnWithdrawAccount, this);
	TransferButton_->Bind(wxEVT_BUTTON, &BankFrame::OnTransferAccount, this);

	transactionButton_ = new wxButton(panel_, wxID_ANY, "Liste des transactions", wxPoint(50, 450), wxSize(200, 50));
	transactionButton_->Bind(wxEVT_BUTTON, &BankFrame::OnTransactionAccount, this);

	changeAccountButton_ = new wxButton(panel_, wxID_ANY, "Changer de compte", wxPoint(550, 450), wxSize(200, 50));
	changeAccountButton_->Bind(wxEVT_BUTTON, &BankFrame::OnChangeAccount, this);

	
}

void BankFrame::OnTransactionAccount(wxCommandEvent& event)
{
	// Crée le pop up
	wxDialog* dialog = new wxDialog(this, wxID_ANY, "Liste des transactions", wxDefaultPosition, wxSize(400, 400));

	// Crée la liste des transactions
	wxListBox* transactionListBox = new wxListBox(dialog, wxID_ANY, wxPoint(20, 20), wxSize(360, 340));

	// Remplit la liste des transactions
	for (int i = transactionList_.size()-1; i >= 0; i--)
	{
		if(transactionListAccount_[i] == selectedIndexAccount_){
			transactionListBox->Append(transactionList_[i]);	
		}
	}

	// Affiche le pop up
	dialog->ShowModal();

	// Détruit le pop up
	dialog->Destroy();
}

void BankFrame::OnAddAccount(wxCommandEvent& event) {
	// Show a dialog to enter the amount to add
	wxTextEntryDialog dialog(this, "Entrez le montant à ajouter :", "Ajout d'argent");
	if (dialog.ShowModal() == wxID_OK)
	{
		// Get the entered amount
		wxString amountString = dialog.GetValue();
		double amount;
		if (amountString.ToDouble(&amount) && amount > 0)
		{
			if(balance_[selectedIndexAccount_] + amount < 1000000000) {
				// The entered amount is a valid number
				balance_[selectedIndexAccount_] += amount; // Add the amount to the balance

				// Update the balance text
				wxString balanceText = wxString::Format("Solde : %.2f", balance_[selectedIndexAccount_]);
				balanceText_->SetLabel(balanceText);

				// Add the transaction to the transaction list
				wxString transaction = wxString::Format("Ajout de %.2f", amount);
				transactionList_.Add(transaction);
				transactionListAccount_.Add(selectedIndexAccount_);

				wxLogStatus("Ajout d'argent réussi");
			}
			else {
				// The balance would exceed the maximum balance of 1000000000
				wxMessageBox("Le solde ne peut pas dépasser 1000000000.", "Erreur", wxICON_ERROR);
				wxLogStatus("Erreur d'ajout d'argent : solde maximum dépassé");
			}
		}
		else
		{
			// The entered amount is not a valid number
			wxMessageBox("Le montant entré n'est pas valide.", "Erreur", wxICON_ERROR);
			wxLogStatus("Erreur d'ajout d'argent");
		}
	}
}

void BankFrame::OnWithdrawAccount(wxCommandEvent& event) {
	// Show a dialog to enter the amount to withdraw
	wxTextEntryDialog dialog(this, "Entrez le montant à retirer :", "Retrait d'argent");
	if (dialog.ShowModal() == wxID_OK)
	{
		// Get the entered amount
		wxString amountString = dialog.GetValue();
		double amount;
		if (amountString.ToDouble(&amount) && amount > 0)
		{
			// The entered amount is a valid number
			if (amount > balance_[selectedIndexAccount_])
			{
				// The balance would become negative
				wxMessageBox("Le solde ne peut pas devenir négatif.", "Erreur", wxICON_ERROR);
				wxLogStatus("Erreur de retrait d'argent : solde négatif");
			}
			else
			{
				// The balance is within the limits
				balance_[selectedIndexAccount_] -= amount; // Withdraw the amount from the balance

				// Update the balance text
				wxString balanceText = wxString::Format("Solde : %.2f", balance_[selectedIndexAccount_]);
				balanceText_->SetLabel(balanceText);

				// Add the transaction to the transaction list
				wxString transaction = wxString::Format("Retrait de %.2f", amount);
				transactionList_.Add(transaction);
				transactionListAccount_.Add(selectedIndexAccount_);

				wxLogStatus("Retrait d'argent réussi");
			}
		}
		else
		{
			// The entered amount is not a valid number
			wxMessageBox("Le montant entré n'est pas valide.", "Erreur", wxICON_ERROR);
			wxLogStatus("Erreur de retrait d'argent");
		}
	}
}

bool BankFrame::ClientInfoTransfer(wxString clientNumber) {
	boost::property_tree::ptree pt;
	try {
		read_json("bank.json", pt);
	}
	catch (const std::exception& e) {
		wxMessageBox(e.what(), "Erreur", wxOK | wxICON_ERROR);
	}

	if (pt.count(clientNumber.ToStdString()) == 0) // vérifie que le numéro de client existe dans le fichier json
	{
		return false;
	}

	
	// récupère les infos du client et les stocke dans les attributs de la classe
	firstNameTransfer_ = pt.get<std::string>(clientNumber.ToStdString() + ".firstname_");
	lastNameTransfer_ = pt.get<std::string>(clientNumber.ToStdString() + ".lastName_");
	addressTransfer_ = pt.get<std::string>(clientNumber.ToStdString() + ".address_");
	passwordTransfer_ = pt.get<std::string>(clientNumber.ToStdString() + ".password_");
	clientNumber.ToDouble(&clientNumberTransfer_);

	// récupère les infos des comptes du client et les stocke dans les wxArray correspondants
	for (auto& account : pt.get_child(clientNumber.ToStdString() + ".accountName_"))
	{
		accountNameTransfer_.Add(account.second.data());
		accountTypeTransfer_.Add(std::stoi(pt.get<std::string>(clientNumber.ToStdString() + ".accountType_." + account.first)));
		balanceTransfer_.Add(std::stod(pt.get<std::string>(clientNumber.ToStdString() + ".balance_." + account.first)));
	}
	

	// récupère les infos des transactions du client et les stocke dans les wxArray correspondants
	for (auto& transaction : pt.get_child(clientNumber.ToStdString() + ".transactionList_"))
	{
		transactionListTransfer_.Add(transaction.second.data());
		transactionListAccountTransfer_.Add(std::stoi(pt.get<std::string>(clientNumber.ToStdString() + ".transactionListAccount_." + transaction.first)));
	}
	
	return true;
}

void BankFrame::ClientInfoSaveTransfer() {
	boost::property_tree::ptree pt;

	ifstream file_out("bank.json");
	if (file_out.good()) {
		try {
			read_json("bank.json", pt);
			pt.erase(to_string((int)clientNumberTransfer_));
		}
		catch (const std::exception& e) {
			wxMessageBox(e.what(), "Erreur", wxOK | wxICON_ERROR);
		}
	}

	// Ajout des informations du client dans la variable pt

	pt.put(to_string((int)clientNumberTransfer_) + ".firstname_", firstNameTransfer_);
	pt.put(to_string((int)clientNumberTransfer_) + ".lastName_", lastNameTransfer_);
	pt.put(to_string((int)clientNumberTransfer_) + ".password_", passwordTransfer_);
	pt.put(to_string((int)clientNumberTransfer_) + ".address_", addressTransfer_);

	for (size_t i = 0; i < accountNameTransfer_.size(); i++)
	{
		pt.put(to_string((int)clientNumberTransfer_) + ".accountName_." + to_string(i), accountNameTransfer_[i]);
		pt.put(to_string((int)clientNumberTransfer_) + ".accountType_." + to_string(i), accountTypeTransfer_[i]);
		pt.put(to_string((int)clientNumberTransfer_) + ".balance_." + to_string(i), balanceTransfer_[i]);
	}

	for (size_t i = 0; i < transactionListTransfer_.size(); i++)
	{
		pt.put(to_string((int)clientNumberTransfer_) + ".transactionList_." + to_string(i), transactionListTransfer_[i]);
		pt.put(to_string((int)clientNumberTransfer_) + ".transactionListAccount_." + to_string(i), transactionListAccountTransfer_[i]);
	}

	// Enregistrement du contenu de pt dans un fichier au format JSON
	ofstream file_in("bank.json");
	write_json(file_in, pt);
	file_in.close();
}

void BankFrame::OnTransferAccount(wxCommandEvent& event) {
	// Show a dialog to enter the client number
	wxTextEntryDialog dialog(this, "Entrez le numéro de client :", "Transfer d'argent");
	if (dialog.ShowModal() == wxID_OK)
	{
		// Get the entered client number
		wxString clientNumberString = dialog.GetValue();
		
		double clientNumber;
		if (clientNumberString.ToDouble(&clientNumber) && clientNumber>0 && ClientInfoTransfer(clientNumberString))
		{
			// The entered client number is a valid number
			// Check if the client exists and get the list of their accounts

			// Show a dialog to select the account to transfer the money to
			wxSingleChoiceDialog choiceDialog(this, "Choisissez le compte de destination :", "Transfer d'argent", accountNameTransfer_);
			if (choiceDialog.ShowModal() == wxID_OK)
			{
				// Get the selected index
				int selectedIndexAccountTransfer_ = choiceDialog.GetSelection();

				// Show a dialog to enter the amount to transfer
				wxTextEntryDialog amountDialog(this, "Entrez le montant à transférer :", "Transfer d'argent");
				if (amountDialog.ShowModal() == wxID_OK)
				{
					// Get the entered amount
					wxString amountString = amountDialog.GetValue();
					double amount;
					if (amountString.ToDouble(&amount))
					{
						// Check if the transfer is possible (i.e. if the amount is greater than 0 and the current account has enough balance)
						if (amount > 0 && balance_[selectedIndexAccount_] >= amount)
						{
							// Perform the transfer
							balance_[selectedIndexAccount_] -= amount;

							

							
							if (transactionListTransfer_.GetCount() == 9999) {
								transactionListTransfer_.RemoveAt(0);
								transactionListAccountTransfer_.RemoveAt(0);
							}
							if (transactionList_.GetCount() == 9999) {
								transactionList_.RemoveAt(0);
								transactionListAccount_.RemoveAt(0);
							}

							if (clientNumberTransfer_ != clientNumber_) {
								wxString message = wxString::Format("Vous avez envoye %.2f sur le compte %s (%.0f)", amount, accountNameTransfer_[selectedIndexAccountTransfer_], clientNumberTransfer_);
								transactionList_.Add(message);
								transactionListAccount_.Add(selectedIndexAccount_);

								message = wxString::Format("Vous avez recu %.2f de la part du compte %s (%.0f)", amount, accountName_[selectedIndexAccount_], clientNumber_);
								transactionListTransfer_.Add(message);
								transactionListAccountTransfer_.Add(selectedIndexAccountTransfer_);

								balanceTransfer_[selectedIndexAccountTransfer_] += amount;
								
								ClientInfoSaveTransfer();
							}
							else {
								wxString message = wxString::Format("Vous avez envoye %.2f sur votre compte %s", amount, accountNameTransfer_[selectedIndexAccountTransfer_]);
								transactionList_.Add(message);
								transactionListAccount_.Add(selectedIndexAccount_);

								message = wxString::Format("Vous avez recu %.2f de votre compte %s", amount, accountName_[selectedIndexAccount_]);
								transactionList_.Add(message);
								transactionListAccount_.Add(selectedIndexAccountTransfer_);

								balance_[selectedIndexAccountTransfer_] += amount;
							}

							

							// Update the account display
							clientText2_->Show(false);
							accountText_->Show(false);
							balanceText_->Show(false);
							rateText_->Show(false);
							addButton_->Show(false);
							withdrawButton_->Show(false);
							TransferButton_->Show(false);
							transactionButton_->Show(false);
							changeAccountButton_->Show(false);
							AccountDisplay();

							

							// Show a message to confirm the transfer
							wxMessageBox(wxString::Format("Transfer de %.2f réussi !", amount), "Transfer d'argent", wxICON_INFORMATION);
						}
						else
						{
							// The transfer is not possible, show an error message
							wxMessageBox("Impossible de réaliser le Transfer. Vérifiez que le montant est valide et que vous avez assez de solde.", "Erreur", wxICON_ERROR);
						}
					}
					else
					{
						// The entered amount is not a valid number, show an error message
						wxMessageBox("Le montant entré n'est pas valide.", "Erreur", wxICON_ERROR);
					}
				}
			}
		}
		else
		{
			// The entered client number is not a valid number, show an error message
			wxMessageBox("Le numéro de client entré n'est pas valide.", "Erreur", wxICON_ERROR);
		}
		firstNameTransfer_ = "";
		lastNameTransfer_ = "";
		clientNumberTransfer_ = 0;
		addressTransfer_ = "";
		passwordTransfer_ = "";
		accountNameTransfer_.clear();
		accountTypeTransfer_.clear();
		balanceTransfer_.clear();
		transactionListTransfer_.clear();
		transactionListAccountTransfer_.clear();
	}
}

void BankFrame::OnChangeAccount(wxCommandEvent& event) {
	clientText2_->Show(false);
	accountText_->Show(false);
	balanceText_->Show(false);
	rateText_->Show(false);
	addButton_->Show(false);
	withdrawButton_->Show(false);
	TransferButton_->Show(false);
	transactionButton_->Show(false);
	changeAccountButton_->Show(false);

	ClientDisplay();
}

// Affichage du compte du client
void BankFrame::ClientDisplay() {
	// On cache le message de bienvenue et les boutons de connection et de création
	welcome_->Show(false);
	connexion_button_->Show(false);
	creation_button_->Show(false);

	// Affiche le nom du client et son numéro de client
	wxString clientText = wxString::Format("Bonjour %s %s\nNuméro de client : %.0f", firstName_, lastName_, clientNumber_);
	clientText_ = new wxStaticText(panel_, wxID_ANY, clientText, wxPoint(20, 10), wxSize(200, -1));
	wxFont font = clientText_->GetFont();
	font.SetPointSize(20);
	clientText_->SetFont(font);

	// Affiche l'adresse du client
	wxString clientAdressText = wxString::Format("Adresse : %s", address_);
	clientAdressText_ = new wxStaticText(panel_, wxID_ANY, clientAdressText, wxPoint(20, 100), wxSize(150, -1));
	

	// Crée une liste avec les comptes du client
	wxArrayString accountList;
	accountsLabel_ = new wxStaticText(panel_, wxID_ANY, "Liste des comptes", wxPoint(325, 115), wxSize(150, -1), wxALIGN_CENTER);
	font = accountsLabel_->GetFont();
	font.SetPointSize(14); // Changement de la taille de la police
	accountsLabel_->SetFont(font);
	for (int i = 0; i < accountName_.size(); i++) {
		accountList.Add(wxString::Format("Compte %s - Solde : %.2f", accountName_[i], balance_[i]));
	}
	accountsListBox_ = new wxListBox(panel_, wxID_ANY, wxPoint(250, 150), wxSize(300, 200), accountList, wxLB_SINGLE);

	// Crée des boutons pour accéder ou supprimer le compte sélectionné, et un bouton qui permet de créer un compte
	accessAccountButton_ = new wxButton(panel_, wxID_ANY, "Accéder au compte", wxPoint(50, 400), wxSize(200, 50));
	deleteAccountButton_ = new wxButton(panel_, wxID_ANY, "Supprimer le compte", wxPoint(300, 400), wxSize(200, 50));
	createAccountButton_ = new wxButton(panel_, wxID_ANY, "Créer un compte", wxPoint(550, 400), wxSize(200, 50));
	accessAccountButton_->Bind(wxEVT_BUTTON, &BankFrame::OnAccessAccount, this);
	deleteAccountButton_->Bind(wxEVT_BUTTON, &BankFrame::OnDeleteAccount, this);
	createAccountButton_->Bind(wxEVT_BUTTON, &BankFrame::OnCreateAccount, this);

	// Crée un bouton qui supprime toutes les informations et retourne à la page de choix de serveurs
	logoutButton_ = new wxButton(panel_, wxID_ANY, "Déconnexion", wxPoint(550, 500), wxSize(200, 50));
	logoutButton_->Bind(wxEVT_BUTTON, &BankFrame::OnLogout, this);
}

// Affiche le compte sélectionné
void BankFrame::OnAccessAccount(wxCommandEvent& event) {
	// on récupère l'indice du compte sélectionné
	selectedIndexAccount_ = accountsListBox_->GetSelection();
	if (selectedIndexAccount_ == wxNOT_FOUND) {
		// Si aucun compte n'est sélectionné on affiche un message d'erreur
		wxMessageBox("Aucun compte sélectionné !", "Erreur", wxICON_ERROR);
		return;
	}

	AccountDisplay();
}

// Supression d'un compte
void BankFrame::OnDeleteAccount(wxCommandEvent& event) {
	// On récupère l'id du compte sélectionné
	selectedIndexAccount_ = accountsListBox_->GetSelection();
	if (selectedIndexAccount_ == wxNOT_FOUND) {
		// Si aucun compte n'est sélectionné on affiche un message d'erreur
		wxMessageBox("Aucun compte sélectionné !", "Erreur", wxICON_ERROR);
		return;
	}

	// On affiche un message afin de confirmer la supression
	int confirm = wxMessageBox("Voulez-vous vraiment supprimer ce compte ?", "Confirmation", wxICON_QUESTION | wxYES_NO);
	if (confirm == wxYES) {


		// On supprime le compte
		for (int i = transactionList_.size() - 1; i >= 0; i--)
		{
			if (transactionListAccount_[i] == selectedIndexAccount_) {
				transactionListAccount_.erase(transactionListAccount_.begin() + i);
				transactionList_.erase(transactionList_.begin() + i);
			}
		}

		accountName_.erase(accountName_.begin() + selectedIndexAccount_);
		accountType_.erase(accountType_.begin() + selectedIndexAccount_);
		balance_.erase(balance_.begin() + selectedIndexAccount_);

		accountsListBox_->Destroy();

		wxArrayString accountList;
		for (int i = 0; i < accountName_.size(); i++) {
			accountList.Add(wxString::Format("Compte %s - Solde : %.2f", accountName_[i], balance_[i]));
		}
		accountsListBox_ = new wxListBox(panel_, wxID_ANY, wxPoint(250, 150), wxSize(300, 200), accountList, wxLB_SINGLE);


	}
}

// Création d'un compte
void BankFrame::OnCreateAccount(wxCommandEvent& event) {
	// Create the pop-up window
	wxDialog* dialog = new wxDialog(this, wxID_ANY, "Création de compte", wxDefaultPosition, wxSize(300, 200));
	dialog->Center();

	// Create the form controls
	wxStaticText* accountNameLabel = new wxStaticText(dialog, wxID_ANY, "Nom du compte :", wxPoint(20, 20));
	wxTextCtrl* accountNameField = new wxTextCtrl(dialog, wxID_ANY, "", wxPoint(20, 40), wxSize(250, -1));
	wxStaticText* accountTypeLabel = new wxStaticText(dialog, wxID_ANY, "Type de compte :", wxPoint(20, 70));
	wxChoice* accountTypeChoice = new wxChoice(dialog, wxID_ANY, wxPoint(20, 90), wxSize(250, -1));
	accountTypeChoice->Append("Compte courant");
	accountTypeChoice->Append("Compte épargne");
	accountTypeChoice->SetSelection(0);
	wxButton* createButton = new wxButton(dialog, wxID_OK, "Créer le compte", wxPoint(150, 130));
	wxButton* cancelButton = new wxButton(dialog, wxID_CANCEL, "Annuler", wxPoint(20, 130));

	if (dialog->ShowModal() == wxID_OK)
	{
		// The user clicked the "Créer le compte" button
		// Retrieve the form data
		wxString accountName = accountNameField->GetValue();
		int accountType = accountTypeChoice->GetSelection();

		// Check the form data
		if (accountName.empty())
		{
			// The account name field is empty, display an error message
			wxMessageBox("Veuillez entrer un nom de compte.", "Erreur", wxICON_ERROR);
		}
		else
		{
			// The form data is valid
			// Perform additional processing here (e.g., create the account in the database)
			if(accountName_.GetCount() < 100) {
				// Add the new account to the list of accounts for the client
				accountName_.Add(accountName);
				balance_.Add(0.0);
				accountType_.Add(accountType);
				// Display the newly created account
				selectedIndexAccount_ = accountName_.GetCount() - 1;
				transactionList_.Add("Creation du compte");
				transactionListAccount_.Add(selectedIndexAccount_);
				AccountDisplay();
			}
			else {
				wxMessageBox("Vous êtes au nombre maximum de compte.", "Erreur", wxICON_ERROR);
			}
		}
	}

	// Destroy the pop-up window
	dialog->Destroy();
}

// Déconnection
void BankFrame::OnLogout(wxCommandEvent& event) {
	// On sauvegarde les informations
	ClientInfoSave();

	// On reset les infos
	firstName_ = "";
	lastName_ = "";
	clientNumber_ = 0;
	address_ = "";
	password_ = "";
	server_ = 0;
	accountName_.clear();
	accountType_.clear();
	balance_.clear();
	transactionList_.clear();
	transactionListAccount_.clear();

	// On cache les éléments
	accountsListBox_->Show(false);
	accessAccountButton_->Show(false);
	deleteAccountButton_->Show(false);
	createAccountButton_->Show(false);
	logoutButton_->Show(false);
	clientAdressText_->Show(false);
	clientText_->Show(false);
	accountsLabel_->Show(false);

	// On affiche les éléments de choix de serveur
	welcome_->Show(true);
	server1_->Show(true);
	server2_->Show(true);
	server3_->Show(true);
}

// Lorsqu'un serveur est sélectionné
void BankFrame::OnServer(wxCommandEvent& event)
{
	int serverId = event.GetId();
	// Déterminer le serveur sélectionné en fonction de l'identifiant du bouton
	if (serverId == server1_->GetId())
	{
		// L'utilisateur a sélectionné le serveur 1
		server_ = 1;
		// ...

		// Pour le debug, à supprimer à la fin
		wxLogStatus("server 1");
	}
	else if (serverId == server2_->GetId())
	{
		// L'utilisateur a sélectionné le serveur 2
		server_ = 2;
		// ...

		// Pour le debug, à supprimer à la fin
		wxLogStatus("server 2");
	}
	else if (serverId == server3_->GetId())
	{
		// L'utilisateur a sélectionné le serveur 3
		server_ = 3;
		// ...

		// Pour le debug, à supprimer à la fin
		wxLogStatus("server 3");
	}

	// On masque les éléments de la fenêtre
	server1_->Show(false);
	server2_->Show(false);
	server3_->Show(false);

	// On affiche les autres éléments
	connexion_button_->Show(true);
	creation_button_->Show(true);
}

// Lorsque l'on veut se connecter
void BankFrame::Connexion(wxCommandEvent& event)
{
	// On crée le pop up
	wxDialog* dialog = new wxDialog(this, wxID_ANY, "Connection", wxDefaultPosition, wxSize(300, 200));
	dialog->Center();
	// On crée les contrôles du formulaire
	wxStaticText* clientLabel = new wxStaticText(dialog, wxID_ANY, "Numéro client :", wxPoint(20, 20));
	wxTextCtrl* clientField = new wxTextCtrl(dialog, wxID_ANY, "", wxPoint(20, 40), wxSize(250, -1));
	wxStaticText* passwordLabel = new wxStaticText(dialog, wxID_ANY, "Mot de passe :", wxPoint(20, 70));
	wxTextCtrl* passwordField = new wxTextCtrl(dialog, wxID_ANY, "", wxPoint(20, 90), wxSize(250, -1), wxTE_PASSWORD);
	wxButton* connexionButton = new wxButton(dialog, wxID_OK, "Se connecter", wxPoint(150, 130));
	wxButton* cancelButton = new wxButton(dialog, wxID_CANCEL, "Annuler", wxPoint(20, 130));

	if (dialog->ShowModal() == wxID_OK)
	{
		// L'utilisateur a cliqué sur le bouton "Se connecter"
		// On récupère les données du formulaire
		wxString clientNumber = clientField->GetValue();
		wxString password = passwordField->GetValue();

		// On vérifie les données du formulaire 
		if (clientNumber.empty() || password.empty())
		{
			// Un des champs est vide, on affiche un message d'erreur
			wxMessageBox("Veuillez remplir tous les champs du formulaire.", "Erreur", wxICON_ERROR);
		}
		else
		{
			// Les données du formulaire sont valides
			// ...

			// Pour le debug, à modifier à la fin
			bool test = ClientInfo(clientNumber, password);
			if (test) {

				// On affiche le compte client
				ClientDisplay();

			}
			else {
				wxMessageBox("Les identifiants sont invalides", "Erreur", wxICON_ERROR);
			}
			
		}
	}

	// On détruit le pop up
	dialog->Destroy();
}

int BankFrame::GenerateClientNumber()
{
	bool ok = false;
	while(true){
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(100000, 999999);
		ok = IsPresent(dis(gen));
		if(ok){
			return dis(gen);
		}
	}	
}

bool BankFrame::IsPresent(int clientNumber) {
	boost::property_tree::ptree pt;
	try {
		read_json("bank.json", pt);
	}
	catch (const std::exception& e) {
		wxMessageBox(e.what(), "Erreur", wxOK | wxICON_ERROR);
	}

	if (pt.count(to_string(clientNumber)) == 0) // vérifie que le numéro de client n'existe pas dans le fichier json
	{
		return true;
	}
	return false;
}

// Lorsque l'on veut créer un compte
void BankFrame::Creation(wxCommandEvent& event)
{
	// On crée le pop up
	wxDialog* dialog = new wxDialog(this, wxID_ANY, "Création de compte", wxDefaultPosition, wxSize(300, 400));

	// On crée les contrôles du formulaire
	wxStaticText* firstNameLabel = new wxStaticText(dialog, wxID_ANY, "Prénom :", wxPoint(20, 20));
	wxTextCtrl* firstNameField = new wxTextCtrl(dialog, wxID_ANY, "", wxPoint(20, 40), wxSize(250, -1));
	wxStaticText* lastNameLabel = new wxStaticText(dialog, wxID_ANY, "Nom :", wxPoint(20, 70));
	wxTextCtrl* lastNameField = new wxTextCtrl(dialog, wxID_ANY, "", wxPoint(20, 90), wxSize(250, -1));
	wxStaticText* addressLabel = new wxStaticText(dialog, wxID_ANY, "Adresse :", wxPoint(20, 120));
	wxTextCtrl* addressField = new wxTextCtrl(dialog, wxID_ANY, "", wxPoint(20, 140), wxSize(250, -1));
	wxStaticText* passwordLabel = new wxStaticText(dialog, wxID_ANY, "Mot de passe :", wxPoint(20, 170));
	wxTextCtrl* passwordField = new wxTextCtrl(dialog, wxID_ANY, "", wxPoint(20, 190), wxSize(250, -1), wxTE_PASSWORD);
	wxStaticText* passwordConfirmLabel = new wxStaticText(dialog, wxID_ANY, "Confirmer le mot de passe :", wxPoint(20, 220));
	wxTextCtrl* passwordConfirmField = new wxTextCtrl(dialog, wxID_ANY, "", wxPoint(20, 240), wxSize(250, -1), wxTE_PASSWORD);
	wxButton* createButton = new wxButton(dialog, wxID_OK, "Créer le compte", wxPoint(150, 280));
	wxButton* cancelButton = new wxButton(dialog, wxID_CANCEL, "Annuler", wxPoint(20, 280));

	if (dialog->ShowModal() == wxID_OK)
	{
		// L'utilisateur a cliqué sur le bouton "Créer le compte"
		// On récupère les données du formulaire
		wxString firstName = firstNameField->GetValue();
		wxString lastName = lastNameField->GetValue();
		wxString address = addressField->GetValue();
		wxString password = passwordField->GetValue();
		wxString passwordConfirm = passwordConfirmField->GetValue();

		// On vérifie les données du formulaire
		if (firstName.empty() || lastName.empty() || address.empty() || password.empty() || passwordConfirm.empty())
		{
			// Un des champs est vide, on affiche un message d'erreur
			wxMessageBox("Veuillez remplir tous les champs du formulaire.", "Erreur", wxICON_ERROR);
		}
		else if (password != passwordConfirm)
		{
			// Les mots de passe ne sont pas identiques, on affiche un message d'erreur
			wxMessageBox("Les mots de passe saisis ne sont pas identiques.", "Erreur", wxICON_ERROR);
		}
		else
		{
			
			firstName_ = firstName;
			lastName_ = lastName;
			address_ = address;
			password_ = password;
			clientNumber_ = GenerateClientNumber();
			accountName_.Add("Epargne");
			accountType_.Add(0);
			balance_.Add(0);
			transactionList_.Add("Creation du compte");
			transactionListAccount_.Add(0);

			// On affiche un message d'information pour informer de la réussite
			wxString message = wxString::Format("Votre compte a été créé avec succès, retenez votre numéro de client : %.0f", clientNumber_);
			wxMessageBox(message, "Information", wxICON_INFORMATION);

			// On affiche le compte client
			ClientDisplay();

			wxLogStatus("compte créé avec succès");
		}
	}

	// On détruit le pop up
	dialog->Destroy();
}

bool BankFrame::ClientInfo(wxString clientNumber, wxString password) {
	boost::property_tree::ptree pt;
	try {
		read_json("bank.json", pt);
	}
	catch (const std::exception& e) {
		wxMessageBox(e.what(), "Erreur", wxOK | wxICON_ERROR);
	}

	if (pt.count(clientNumber.ToStdString()) == 0) // vérifie que le numéro de client existe dans le fichier json
	{
		return false;
	}

	password_ = pt.get<std::string>(clientNumber.ToStdString() + ".password_");
	if (password != password_) // vérifie que le mot de passe est correct
	{
		password_ = "";
		return false;
	}
	// récupère les infos du client et les stocke dans les attributs de la classe
	firstName_ = pt.get<std::string>(clientNumber.ToStdString() + ".firstname_");
	lastName_ = pt.get<std::string>(clientNumber.ToStdString() + ".lastName_");
	address_ = pt.get<std::string>(clientNumber.ToStdString() + ".address_");
	password_ = pt.get<std::string>(clientNumber.ToStdString() + ".password_");
	clientNumber.ToDouble(&clientNumber_);

	// récupère les infos des comptes du client et les stocke dans les wxArray correspondants
	for (auto& account : pt.get_child(clientNumber.ToStdString() + ".accountName_"))
	{
		accountName_.Add(account.second.data());
		accountType_.Add(std::stoi(pt.get<std::string>(clientNumber.ToStdString() + ".accountType_." + account.first)));
		balance_.Add(std::stod(pt.get<std::string>(clientNumber.ToStdString() + ".balance_." + account.first)));
	}

	// récupère les infos des transactions du client et les stocke dans les wxArray correspondants
	for (auto& transaction : pt.get_child(clientNumber.ToStdString() + ".transactionList_"))
	{
		transactionList_.Add(transaction.second.data());
		transactionListAccount_.Add(std::stoi(pt.get<std::string>(clientNumber.ToStdString() + ".transactionListAccount_." + transaction.first)));
	}

	return true;
}

void BankFrame::ClientInfoSave() {

	boost::property_tree::ptree pt;

	ifstream file_out("bank.json");
	if (file_out.good()) {
		try {
			read_json("bank.json", pt);
			pt.erase(to_string((int)clientNumber_));
		}
		catch (const std::exception& e) {
			wxMessageBox(e.what(), "Erreur", wxOK | wxICON_ERROR);
		}
	}

	// Ajout des informations du client dans la variable pt
	
	pt.put(to_string((int)clientNumber_) + ".firstname_", firstName_);
	pt.put(to_string((int)clientNumber_) + ".lastName_", lastName_);
	pt.put(to_string((int)clientNumber_) + ".password_", password_);
	pt.put(to_string((int)clientNumber_) + ".address_", address_);

	for (size_t i = 0; i < accountName_.size(); i++)
	{
		pt.put(to_string((int)clientNumber_) + ".accountName_." + to_string(i), accountName_[i]);
		pt.put(to_string((int)clientNumber_) + ".accountType_." + to_string(i), accountType_[i]);
		pt.put(to_string((int)clientNumber_) + ".balance_." + to_string(i), balance_[i]);
	}
	
	for (size_t i = 0; i < transactionList_.size(); i++)
	{
		pt.put(to_string((int)clientNumber_) + ".transactionList_." + to_string(i), transactionList_[i]);
		pt.put(to_string((int)clientNumber_) + ".transactionListAccount_." + to_string(i), transactionListAccount_[i]);
	}

	// Enregistrement du contenu de pt dans un fichier au format JSON
	ofstream file_in("bank.json");
	write_json(file_in, pt);
	file_in.close();
	
}

void BankFrame::OnClose(wxCloseEvent& evt) {
	if(!welcome_->IsShown()){
		// On sauvegarde les données
		ClientInfoSave();
	}
	// Pour pouvoir fermer la fenêtre
	evt.Skip();
}

IMPLEMENT_APP(BankApp)

// Lorsque l'on lance l'application
bool BankApp::OnInit()
{
	// on instancie une fenêtre
	BankFrame* frame = new BankFrame("Bank_Name");
	// On définie la taille de la fenêtre (sans les bordures)
	frame->SetClientSize(800, 600);
	// On centre la fenêtre sur l'écran
	frame->Center();
	// On affiche la fenêtre
	frame->Show(true);
	return true;
}