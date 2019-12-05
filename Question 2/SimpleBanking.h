#ifndef SIMPLEBANKING_H 
#define SIMPLEBANKING_H

#include <vector>

class Account
{
public:
	Account();
	virtual ~Account();

	unsigned int getAccountId() const;
	const std::vector<unsigned int> & getCustomerIds() const;
	double getBalance() const;

	void addCustomerId(unsigned int customerId);
	void removeCustomerId(unsigned int customerId);

	// virtual enables custom logic for derived accounts such as credit
	virtual void enableOverdraw();
	virtual void disableOverdraw();
	virtual bool withdraw(double amount);
	virtual bool deposit(double amount);

private:
	static unsigned int lastId; // holds the ID that will be used by the next account instance.

	unsigned int m_accountId;
	std::vector<unsigned int> m_customerIds;
	double m_balance; // Q: should we have another balance that isn't a double? eg. not dollars.
	bool m_overdrawEnabled;
};

// Objects of class Customer use Accounts.
class Customer
{
public:
	Customer();
	virtual ~Customer();

	unsigned int getId() const;
	unsigned int getNumAccounts() const; // Usage: find out the number of accounts before using getAccount()

	Account * getAccount(unsigned int accountIndex);
	void attachAccount(Account * account);
	void detachAccount(unsigned int accountIndex);

private:
	static unsigned int lastId;
	unsigned int m_id;

	std::vector<Account *> m_accounts;	// Keep a list of account references.
};

#endif