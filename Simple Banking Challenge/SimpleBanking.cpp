#include "SimpleBanking.h"
#include <cmath> // abs()
#include <algorithm>

unsigned int Account::lastId = 1;
unsigned int Customer::lastId = 1;

Account::Account()
{
	m_accountId = lastId++;
	m_balance = 0.00;
	m_customerIds = std::vector<unsigned int>();
	m_overdrawEnabled = false;
}

Account::~Account()
{
}

unsigned int Account::getAccountId() const
{
	return m_accountId;
}

const std::vector<unsigned int>& Account::getCustomerIds() const
{
	return m_customerIds;
}

void Account::addCustomerId(unsigned int customerId)
{
	m_customerIds.push_back(customerId);
}

void Account::removeCustomerId(unsigned int customerId)
{
	for (int i = 0; i < m_customerIds.size(); i++) {
		if (m_customerIds.at(i) == customerId) {
			// Invalidate the customer ID by setting it equal to 0 (effectively removes it)
			m_customerIds[i] = 0;
		}
	}
}

double Account::getBalance() const
{
	return m_balance;
}

void Account::enableOverdraw()
{
	m_overdrawEnabled = true;
}

void Account::disableOverdraw()
{
	m_overdrawEnabled = false;
}

bool Account::withdraw(double amount)
{
	if (!m_overdrawEnabled) {
		if (amount > m_balance) {
			return false;
		}
	}
	m_balance -= amount;
	return true;
}

bool Account::deposit(double amount)
{
	m_balance += amount;
	return true;
}


Customer::Customer()
{
	m_accounts = std::vector<Account *>();
	m_id = lastId++;
}

Customer::~Customer()
{
}

unsigned int Customer::getId() const
{
	return m_id;
}

unsigned int Customer::getNumAccounts() const
{
	return m_accounts.size();
}

Account * Customer::getAccount(unsigned int accountIndex)
{
	// Check if m_accounts is empty, then if accountIndex is in range
	if (m_accounts.size() > 0 && accountIndex < (m_accounts.size())) // abs() to cancel negatives?
	{
		return m_accounts.at(accountIndex);
	}
	else
	{
		// Nullptr if account not associated with this customer
		return 0;
	}
}

void Customer::attachAccount(Account * account)
{
	m_accounts.push_back(account);
	account->addCustomerId(m_id);
}

void Customer::detachAccount(unsigned int accountIndex)
{
	// Remove ourself from the account
	m_accounts[accountIndex]->removeCustomerId(m_id);
	m_accounts.erase(std::remove(m_accounts.begin(), m_accounts.end(), m_accounts[accountIndex]), m_accounts.end());	// remove the account from our list
}
