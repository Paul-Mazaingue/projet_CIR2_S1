#include "bank.h"

using namespace std;

// Fenêtre de la banque
BankFrame::BankFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
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

	// Panneau sur lequel on va mettre les élements
	panel_ = new wxPanel(this);

	// Message de bienvenue 
	welcome_ = new wxStaticText(panel_, wxID_ANY, "Bienvenue dans votre banque en ligne,\nvotre partenaire financier de confiance.", wxPoint(100, 100), wxSize(600, -1), wxALIGN_CENTER_HORIZONTAL);
	// Changement de la taille de la police
	wxFont font = welcome_->GetFont();
	font.SetPointSize(24);
	welcome_->SetFont(font);

	// Boutons afin de choisir le serveur
	server1_ = new wxButton(panel_, wxID_ANY, "Paris", wxPoint(100, 275), wxSize(150, 50));
	server2_ = new wxButton(panel_, wxID_ANY, "Londre", wxPoint(325, 275), wxSize(150, 50));
	server3_ = new wxButton(panel_, wxID_ANY, "Berlin", wxPoint(550, 275), wxSize(150, 50));
	server1_->Bind(wxEVT_BUTTON, &BankFrame::OnServer, this);
	server2_->Bind(wxEVT_BUTTON, &BankFrame::OnServer, this);
	server3_->Bind(wxEVT_BUTTON, &BankFrame::OnServer, this);

	// Boutons pour se connecter ou créer un compte
	connexion_button_ = new wxButton(panel_, wxID_ANY, "Se connecter", wxPoint(150, 275), wxSize(200, 50));
	creation_button_ = new wxButton(panel_, wxID_ANY, "Créer un compte", wxPoint(475, 275), wxSize(200, 50));
	connexion_button_->Bind(wxEVT_BUTTON, &BankFrame::Connexion, this);
	creation_button_->Bind(wxEVT_BUTTON, &BankFrame::Creation, this);
	// On cache les boutons au début
	connexion_button_->Show(false);
	creation_button_->Show(false);

	// Quand la fenêtre se ferme
	this->Bind(wxEVT_CLOSE_WINDOW, &BankFrame::OnClose, this);
}

// Méthode qui affiche le compte sélectionné
void BankFrame::AccountDisplay() {
	// On cache les éléments de la page client
	accountsListBox_->Show(false);
	accessAccountButton_->Show(false);
	deleteAccountButton_->Show(false);
	createAccountButton_->Show(false);
	logoutButton_->Show(false);
	clientAdressText_->Show(false);
	clientText_->Show(false);
	accountsLabel_->Show(false);

	// On affiche les éléments de la page compte
	wxString clientText2 = wxString::Format("%s %s", firstName_, lastName_);
	clientText2_ = new wxStaticText(panel_, wxID_ANY, clientText2, wxPoint(325, 30), wxSize(150, -1), wxALIGN_CENTER);

	wxString accountText = wxString::Format("Nom du compte : %s", accountName_[selectedIndexAccount_]);
	accountText_ = new wxStaticText(panel_, wxID_ANY, accountText, wxPoint(200, 110), wxSize(400, -1), wxALIGN_CENTER);

	wxString balanceText = wxString::Format("Solde : %.2f", balance_[selectedIndexAccount_]);
	balanceText_ = new wxStaticText(panel_, wxID_ANY, balanceText, wxPoint(200, 200), wxSize(400, -1), wxALIGN_CENTER);

	wxString rateText = wxString::Format("Taux : %.2f", rate_);
	rateText_ = new wxStaticText(panel_, wxID_ANY, rateText, wxPoint(600, 30), wxSize(200, -1), wxALIGN_CENTER);

	// On redéfinie la taille des texte
	wxFont font = clientText2_->GetFont();
	font.SetPointSize(12);
	clientText2_->SetFont(font);

	font = accountText_->GetFont();
	font.SetPointSize(18);
	accountText_->SetFont(font);

	font = balanceText_->GetFont();
	font.SetPointSize(18);
	balanceText_->SetFont(font);

	font = rateText_->GetFont();
	font.SetPointSize(18);
	rateText_->SetFont(font);

	// Si le compte est un compte épargne on affiche le taux
	if (!accountType_[selectedIndexAccount_]) {
		rateText_->Show(false);
	}

	// On affiche les boutons et on les relie avec les méthodes
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

// Méthode qui affiche la liste des transactions
void BankFrame::OnTransactionAccount(wxCommandEvent& event)
{
	// Crée le pop up
	wxDialog* dialog = new wxDialog(this, wxID_ANY, "Liste des transactions", wxDefaultPosition, wxSize(400, 400));

	// Crée la liste des transactions
	wxListBox* transactionListBox = new wxListBox(dialog, wxID_ANY, wxPoint(20, 20), wxSize(360, 340));

	// Remplit la liste des transactions
	for (int i = transactionList_.size() - 1; i >= 0; i--)
	{
		if (transactionListAccount_[i] == selectedIndexAccount_) {
			transactionListBox->Append(transactionList_[i]);
		}
	}

	// Affiche le pop up
	dialog->ShowModal();

	// Détruit le pop up
	dialog->Destroy();
}

// Méthode qui ajoute de l'argent sur le compte
void BankFrame::OnAddAccount(wxCommandEvent& event) {
	// Affiche un pop up pour demander le montant à ajouter
	wxTextEntryDialog dialog(this, "Entrez le montant à ajouter :", "Ajout d'argent");
	if (dialog.ShowModal() == wxID_OK)
	{
		// On récupère le montant
		wxString amountString = dialog.GetValue();
		double amount;
		if (amountString.ToDouble(&amount) && amount > 0)
		{
			if (balance_[selectedIndexAccount_] + amount < 1000000000) {
				// Si le montant est valide on ajoute l'argent
				balance_[selectedIndexAccount_] += amount;

				// On met à jour le solde
				wxString balanceText = wxString::Format("Solde : %.2f", balance_[selectedIndexAccount_]);
				balanceText_->SetLabel(balanceText);

				// On ajoute la transaction à la liste
				wxString transaction = wxString::Format("Ajout de %.2f", amount);

				// Si on est au max de transactions on supprime la plus ancienne
				if (transactionList_.GetCount() == 9999) {
					transactionList_.RemoveAt(0);
					transactionListAccount_.RemoveAt(0);
				}

				transactionList_.Add(transaction);
				transactionListAccount_.Add(selectedIndexAccount_);
			}
			else {
				// La balance ne peut pas dépasser 1 milliard
				wxMessageBox("Le solde ne peut pas dépasser 1 milliard.", "Erreur", wxICON_ERROR);
			}
		}
		else
		{
			// Si le montant n'est pas valide on affiche une erreur
			wxMessageBox("Le montant entré n'est pas valide.", "Erreur", wxICON_ERROR);
		}
	}
}

// Méthode qui retire de l'argent sur le compte
void BankFrame::OnWithdrawAccount(wxCommandEvent& event) {
	// Affiche un pop up pour demander le montant à retirer
	wxTextEntryDialog dialog(this, "Entrez le montant à retirer :", "Retrait d'argent");
	if (dialog.ShowModal() == wxID_OK)
	{
		// On récupère le montant
		wxString amountString = dialog.GetValue();
		double amount;
		if (amountString.ToDouble(&amount) && amount > 0)
		{
			if (amount > balance_[selectedIndexAccount_])
			{
				// Si le montant est supérieur au solde on affiche une erreur
				wxMessageBox("Le solde ne peut pas devenir négatif.", "Erreur", wxICON_ERROR);
			}
			else
			{
				// Si le montant est valide on retire l'argent
				balance_[selectedIndexAccount_] -= amount; // Withdraw the amount from the balance

				// On met à jour le solde
				wxString balanceText = wxString::Format("Solde : %.2f", balance_[selectedIndexAccount_]);
				balanceText_->SetLabel(balanceText);

				// On ajoute la transaction à la liste
				wxString transaction = wxString::Format("Retrait de %.2f", amount);

				// Si on est au max de transactions on supprime la plus ancienne
				if (transactionList_.GetCount() == 9999) {
					transactionList_.RemoveAt(0);
					transactionListAccount_.RemoveAt(0);
				}

				transactionList_.Add(transaction);
				transactionListAccount_.Add(selectedIndexAccount_);
			}
		}
		else
		{
			// Si le montant n'est pas valide on affiche une erreur
			wxMessageBox("Le montant entré n'est pas valide.", "Erreur", wxICON_ERROR);
		}
	}
}

// Méthode qui récupère les infos du compte qui reçois l'argent
bool BankFrame::ClientInfoTransfer(wxString clientNumber) {

	boost::property_tree::ptree pt;

	// On lit le fichier json
	try {
		read_json("bank.json", pt);
	}
	catch (const std::exception& e) {
		wxMessageBox(e.what(), "Erreur", wxOK | wxICON_ERROR);
	}

	// On vérifie si le numéro de client existe
	if (pt.count(clientNumber.ToStdString()) == 0) // vérifie que le numéro de client existe dans le fichier json
	{
		return false;
	}


	// On récupère les informations du client
	firstNameTransfer_ = pt.get<std::string>(clientNumber.ToStdString() + ".firstname_");
	lastNameTransfer_ = pt.get<std::string>(clientNumber.ToStdString() + ".lastName_");
	addressTransfer_ = pt.get<std::string>(clientNumber.ToStdString() + ".address_");
	passwordTransfer_ = pt.get<std::string>(clientNumber.ToStdString() + ".password_");
	clientNumber.ToDouble(&clientNumberTransfer_);

	// On récupère les informations des comptes
	for (auto& account : pt.get_child(clientNumber.ToStdString() + ".accountName_"))
	{
		accountNameTransfer_.Add(account.second.data());
		accountTypeTransfer_.Add(std::stoi(pt.get<std::string>(clientNumber.ToStdString() + ".accountType_." + account.first)));
		balanceTransfer_.Add(std::stod(pt.get<std::string>(clientNumber.ToStdString() + ".balance_." + account.first)));
	}


	// On récupère les informations des transactions
	for (auto& transaction : pt.get_child(clientNumber.ToStdString() + ".transactionList_"))
	{
		transactionListTransfer_.Add(transaction.second.data());
		transactionListAccountTransfer_.Add(std::stoi(pt.get<std::string>(clientNumber.ToStdString() + ".transactionListAccount_." + transaction.first)));
	}

	return true;
}

// Méthode qui sauvegarde les informations du compte qui a reçu le transfert
void BankFrame::ClientInfoSaveTransfer() {
	boost::property_tree::ptree pt;


	ifstream file_out("bank.json");
	// Si le fichier existe on le lit
	if (file_out.good()) {
		try {
			read_json("bank.json", pt);
			// On supprime les informations relative au client
			pt.erase(to_string((int)clientNumberTransfer_));
		}
		catch (const std::exception& e) {
			wxMessageBox(e.what(), "Erreur", wxOK | wxICON_ERROR);
		}
	}


	// Ajout des informations du client
	pt.put(to_string((int)clientNumberTransfer_) + ".firstname_", firstNameTransfer_);
	pt.put(to_string((int)clientNumberTransfer_) + ".lastName_", lastNameTransfer_);
	pt.put(to_string((int)clientNumberTransfer_) + ".password_", passwordTransfer_);
	pt.put(to_string((int)clientNumberTransfer_) + ".address_", addressTransfer_);

	// Ajout des informations des comptes
	for (size_t i = 0; i < accountNameTransfer_.size(); i++)
	{
		pt.put(to_string((int)clientNumberTransfer_) + ".accountName_." + to_string(i), accountNameTransfer_[i]);
		pt.put(to_string((int)clientNumberTransfer_) + ".accountType_." + to_string(i), accountTypeTransfer_[i]);
		pt.put(to_string((int)clientNumberTransfer_) + ".balance_." + to_string(i), balanceTransfer_[i]);
	}

	// Ajout des informations des transactions
	for (size_t i = 0; i < transactionListTransfer_.size(); i++)
	{
		pt.put(to_string((int)clientNumberTransfer_) + ".transactionList_." + to_string(i), transactionListTransfer_[i]);
		pt.put(to_string((int)clientNumberTransfer_) + ".transactionListAccount_." + to_string(i), transactionListAccountTransfer_[i]);
	}

	// On sauvegarde les informations
	ofstream file_in("bank.json");
	write_json(file_in, pt);
	file_in.close();
}

// Méthode qui transfère de l'argent sur un autre compte
void BankFrame::OnTransferAccount(wxCommandEvent& event) {
	// On affiche le pop up pour entrer le numéro de client
	wxTextEntryDialog dialog(this, "Entrez le numéro de client :", "Transfer d'argent");
	if (dialog.ShowModal() == wxID_OK)
	{
		// On récupère le numéro de client
		wxString clientNumberString = dialog.GetValue();

		double clientNumber;
		if (clientNumberString.ToDouble(&clientNumber) && clientNumber > 0 && ClientInfoTransfer(clientNumberString))
		{
			// On récupère les informations du compte sélectionné

			// On affiche le pop up pour choisir le compte
			wxSingleChoiceDialog choiceDialog(this, "Choisissez le compte de destination :", "Transfer d'argent", accountNameTransfer_);
			if (choiceDialog.ShowModal() == wxID_OK)
			{
				// On récupère l'indice du compte
				int selectedIndexAccountTransfer_ = choiceDialog.GetSelection();

				// On affiche le pop up pour entrer le montant
				wxTextEntryDialog amountDialog(this, "Entrez le montant à transférer :", "Transfer d'argent");
				if (amountDialog.ShowModal() == wxID_OK)
				{
					// On récupère le montant à transférer
					wxString amountString = amountDialog.GetValue();
					double amount;
					if (amountString.ToDouble(&amount))
					{
						if (amount > 0 && balance_[selectedIndexAccount_] >= amount)
						{
							// On vérifie si le compte de départ a assez d'argent et que le montant est positif

							// On retire l'argent du compte de départ
							balance_[selectedIndexAccount_] -= amount;



							// si le nombre de transaction est au max on supprime la plus ancienne
							if (transactionListTransfer_.GetCount() == 9999) {
								transactionListTransfer_.RemoveAt(0);
								transactionListAccountTransfer_.RemoveAt(0);
							}
							if (transactionList_.GetCount() == 9999) {
								transactionList_.RemoveAt(0);
								transactionListAccount_.RemoveAt(0);
							}

							// Si les numéro de client sont différents on ajoute la transaction dans les deux listes
							if (clientNumberTransfer_ != clientNumber_) {
								// On ajoute la transaction dans la liste de transactions du compte de départ
								wxString message = wxString::Format("Vous avez envoye %.2f sur le compte %s (%.0f)", amount, accountNameTransfer_[selectedIndexAccountTransfer_], clientNumberTransfer_);
								transactionList_.Add(message);
								transactionListAccount_.Add(selectedIndexAccount_);

								// On ajoute la transaction dans la liste de transactions du compte de destination
								message = wxString::Format("Vous avez recu %.2f de la part du compte %s (%.0f)", amount, accountName_[selectedIndexAccount_], clientNumber_);
								transactionListTransfer_.Add(message);
								transactionListAccountTransfer_.Add(selectedIndexAccountTransfer_);

								// On ajoute l'argent au compte de destination
								balanceTransfer_[selectedIndexAccountTransfer_] += amount;

								// On sauvegarde les informations du compte de destination
								ClientInfoSaveTransfer();
							}
							// Si le numéro de client est le même on ajoute la transaction au compte de départ
							else {
								// On ajoute la transaction dans la liste de transactions du compte de départ
								wxString message = wxString::Format("Vous avez envoye %.2f sur votre compte %s", amount, accountNameTransfer_[selectedIndexAccountTransfer_]);
								transactionList_.Add(message);
								transactionListAccount_.Add(selectedIndexAccount_);

								// On ajoute la transaction dans la liste de transactions du compte de destination
								message = wxString::Format("Vous avez recu %.2f de votre compte %s", amount, accountName_[selectedIndexAccount_]);
								transactionList_.Add(message);
								transactionListAccount_.Add(selectedIndexAccountTransfer_);

								// On ajoute l'argent au compte de destination
								balance_[selectedIndexAccountTransfer_] += amount;
							}



							// On affiche les informations du compte de départ
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



							// on affiche un message pour dire que le transfert a été effectué
							wxMessageBox(wxString::Format("Transfer de %.2f réussi !", amount), "Transfer d'argent", wxICON_INFORMATION);
						}
						else
						{
							// On affiche un message d'erreur si le compte de départ n'a pas assez d'argent ou si le montant est négatif
							wxMessageBox("Impossible de réaliser le Transfer. Vérifiez que le montant est valide et que vous avez assez de solde.", "Erreur", wxICON_ERROR);
						}
					}
					else
					{
						// On affiche un message d'erreur si le montant n'est pas valide
						wxMessageBox("Le montant entré n'est pas valide.", "Erreur", wxICON_ERROR);
					}
				}
			}
		}
		else
		{
			// On affiche un message d'erreur si le numéro de client du compte de destination n'est pas valide
			wxMessageBox("Le numéro de client entré n'est pas valide.", "Erreur", wxICON_ERROR);
		}
		// On reinitalise les variables du compte de destination
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

// Méthode qui affiche les informations du client
void BankFrame::OnChangeAccount(wxCommandEvent& event) {
	// On cache les informations du compte
	clientText2_->Show(false);
	accountText_->Show(false);
	balanceText_->Show(false);
	rateText_->Show(false);
	addButton_->Show(false);
	withdrawButton_->Show(false);
	TransferButton_->Show(false);
	transactionButton_->Show(false);
	changeAccountButton_->Show(false);

	// On affiche les informations du client
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

	// On ajoute les comptes dans la liste
	for (int i = 0; i < accountName_.size(); i++) {
		accountList.Add(wxString::Format("Compte %s - Solde : %.2f", accountName_[i], balance_[i]));
	}
	// On crée la liste
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

	// On affiche les informations du compte
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

		// On met à jour la liste des comptes
		wxArrayString accountList;
		for (int i = 0; i < accountName_.size(); i++) {
			accountList.Add(wxString::Format("Compte %s - Solde : %.2f", accountName_[i], balance_[i]));
		}
		accountsListBox_ = new wxListBox(panel_, wxID_ANY, wxPoint(250, 150), wxSize(300, 200), accountList, wxLB_SINGLE);


	}
}

// Création d'un compte
void BankFrame::OnCreateAccount(wxCommandEvent& event) {
	// On affiche un pop up de création de compte
	wxDialog* dialog = new wxDialog(this, wxID_ANY, "Création de compte", wxDefaultPosition, wxSize(300, 200));
	dialog->Center();

	// Création des éléments du pop up
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
		// L'utilisateur a cliqué sur le bouton "Créer le compte"
		// On récupère les informations du pop up
		wxString accountName = accountNameField->GetValue();
		int accountType = accountTypeChoice->GetSelection();

		// On vérifie les informations
		if (accountName.empty())
		{
			// Si le nom du compte est vide on affiche un message d'erreur
			wxMessageBox("Veuillez entrer un nom de compte.", "Erreur", wxICON_ERROR);
		}
		else
		{
			// Si les données sont valides on crée le compte
			if (accountName_.GetCount() < 100) {
				// Ajout du compte
				accountName_.Add(accountName);
				balance_.Add(0.0);
				accountType_.Add(accountType);

				selectedIndexAccount_ = accountName_.GetCount() - 1;

				// Si le nombre de transaction est au max on supprime la plus ancienne
				if (transactionList_.GetCount() == 9999) {
					transactionList_.RemoveAt(0);
					transactionListAccount_.RemoveAt(0);
				}
				// Ajout de la transaction
				transactionList_.Add("Creation du compte");
				transactionListAccount_.Add(selectedIndexAccount_);

				// Affichage du compte
				AccountDisplay();
			}
			else {
				// Si le nombre de compte est au max on affiche un message d'erreur
				wxMessageBox("Vous êtes au nombre maximum de compte.", "Erreur", wxICON_ERROR);
			}
		}
	}

	// On ferme le pop up
	dialog->Destroy();
}

// Méthode pour se déconnecter
void BankFrame::OnLogout(wxCommandEvent& event) {
	// On sauvegarde les informations
	ClientInfoSave();

	// On réinitialise les informations
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
	}
	else if (serverId == server2_->GetId())
	{
		// L'utilisateur a sélectionné le serveur 2
		server_ = 2;
	}
	else if (serverId == server3_->GetId())
	{
		// L'utilisateur a sélectionné le serveur 3
		server_ = 3;
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
			bool test = ClientInfo(clientNumber, password);
			if (test) {

				// On affiche le compte client
				ClientDisplay();

			}
			else {
				// On affiche un message d'erreur
				wxMessageBox("Les identifiants sont invalides", "Erreur", wxICON_ERROR);
			}

		}
	}

	// On détruit le pop up
	dialog->Destroy();
}

// Méthode qui génère un numéro de client
int BankFrame::GenerateClientNumber()
{
	// On génère un numéro de client aléatoire entre 100000 et 999999
	bool ok = false;
	while (true) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(100000, 999999);
		// On vérifie que le numéro de client n'existe pas déjà
		ok = IsPresent(dis(gen));
		if (ok) {
			return dis(gen);
		}
	}
}

// Fonction qui vérifie si une numéro de client existe déjà
bool BankFrame::IsPresent(int clientNumber) {
	boost::property_tree::ptree pt;
	ifstream file_out("bank.json");
	if (!file_out.good()) {
		return true;
	}
	file_out.close();

	// On lit le fichier JSON
	try {
		read_json("bank.json", pt);
	}
	catch (const std::exception& e) {
		wxMessageBox(e.what(), "Erreur", wxOK | wxICON_ERROR);
	}

	// On parcourt les clients
	if (pt.count(to_string(clientNumber)) == 0) // On vérifie que le numéro de client n'existe pas dans le fichier json
	{
		return true;
	}
	return false;
}

// Méthode qui gère la création d'un compte
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

			// On récupère les infos du client
			firstName_ = firstName;
			lastName_ = lastName;
			address_ = address;
			password_ = password;
			clientNumber_ = GenerateClientNumber();
			accountName_.Add("Epargne");
			accountType_.Add(0);
			balance_.Add(0);

			// Si le nombre de transaction est au max on supprime la première transaction


			if (transactionList_.GetCount() == 9999) {
				transactionList_.RemoveAt(0);
				transactionListAccount_.RemoveAt(0);
			}

			transactionList_.Add("Creation du compte");
			transactionListAccount_.Add(0);

			// On affiche un message d'information pour informer de la réussite
			wxString message = wxString::Format("Votre compte a été créé avec succès, retenez votre numéro de client : %.0f", clientNumber_);
			wxMessageBox(message, "Information", wxICON_INFORMATION);

			// On affiche le compte client
			ClientDisplay();
		}
	}

	// On détruit le pop up
	dialog->Destroy();
}

// Méthode qui récupère les infos du client
bool BankFrame::ClientInfo(wxString clientNumber, wxString password) {
	boost::property_tree::ptree pt;

	// Si le fichier n'existe pas, on affiche une erreur
	std::ifstream file("bank.json");
	if (!file.good()) {
		return false;
	}


	// On lit le fichier JSON
	try {
		read_json("bank.json", pt);
	}
	catch (const std::exception& e) {
		wxMessageBox(e.what(), "Erreur", wxOK | wxICON_ERROR);
	}

	// On parcourt les clients
	if (pt.count(clientNumber.ToStdString()) == 0) // on vérifie que le numéro de client existe dans le fichier json
	{
		return false;
	}

	// On compare le mot de passe
	password_ = pt.get<std::string>(clientNumber.ToStdString() + ".password_");
	if (password != password_) // vérifie que le mot de passe est correct
	{
		password_ = "";
		return false;
	}

	// On récupère les infos du client et les stocke dans les attributs de la classe
	firstName_ = pt.get<std::string>(clientNumber.ToStdString() + ".firstname_");
	lastName_ = pt.get<std::string>(clientNumber.ToStdString() + ".lastName_");
	address_ = pt.get<std::string>(clientNumber.ToStdString() + ".address_");
	password_ = pt.get<std::string>(clientNumber.ToStdString() + ".password_");
	clientNumber.ToDouble(&clientNumber_);

	// On récupère les infos des comptes du client et les stocke dans les wxArray correspondants
	for (auto& account : pt.get_child(clientNumber.ToStdString() + ".accountName_"))
	{
		accountName_.Add(account.second.data());
		accountType_.Add(std::stoi(pt.get<std::string>(clientNumber.ToStdString() + ".accountType_." + account.first)));
		balance_.Add(std::stod(pt.get<std::string>(clientNumber.ToStdString() + ".balance_." + account.first)));
	}

	// On récupère les infos des transactions du client et les stocke dans les wxArray correspondants
	for (auto& transaction : pt.get_child(clientNumber.ToStdString() + ".transactionList_"))
	{
		transactionList_.Add(transaction.second.data());
		transactionListAccount_.Add(std::stoi(pt.get<std::string>(clientNumber.ToStdString() + ".transactionListAccount_." + transaction.first)));
	}

	return true;
}

// Méthode qui sauvegarde les infos du client
void BankFrame::ClientInfoSave() {

	boost::property_tree::ptree pt;

	// On lit le fichier JSON
	ifstream file_out("bank.json");
	if (file_out.good()) {
		try {
			read_json("bank.json", pt);
			// On supprime les infos du client
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

// Méthode qui gère la fermeture de la fenêtre
void BankFrame::OnClose(wxCloseEvent& evt) {
	// On sauvegarde les infos du client s'il est connecté
	if (!welcome_->IsShown()) {
		ClientInfoSave();
	}
	// On ferme la fenêtre
	evt.Skip();
}

IMPLEMENT_APP(BankApp)

// Lorsque l'on lance l'application
bool BankApp::OnInit()
{
	// on instancie une fenêtre
	BankFrame* frame = new BankFrame("Absolute Financial Corp");
	// On définie la taille de la fenêtre (sans les bordures)
	frame->SetClientSize(800, 600);
	// On centre la fenêtre sur l'écran
	frame->Center();
	// On affiche la fenêtre
	frame->Show(true);
	return true;
}