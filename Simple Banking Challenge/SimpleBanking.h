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
		// holds the ID that will be used by the next account instance.
		static unsigned int lastId;
		unsigned int m_accountId;
		std::vector<unsigned int> m_customerIds;
		// Q: should we have another balance that isn't a double? eg. not dollars.
		double m_balance;
		bool m_overdrawEnabled;
};


class Customer
{
	public:
		Customer();
		virtual ~Customer();

		unsigned int getId() const;

		// Usage: find out the number of accounts before using getAccount()
		unsigned int getNumAccounts() const;

		Account * getAccount(unsigned int accountIndex);
		void attachAccount(Account * account);
		void detachAccount(unsigned int accountIndex);

	private:
		static unsigned int lastId;
		unsigned int m_id;

		// Keep a list of account references.
		std::vector<Account *> m_accounts;
};

#endif
