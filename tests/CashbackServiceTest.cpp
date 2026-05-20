#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "./Helpers.h"

#include "Infrastructure/CashbackService.h"

using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

class MockCustomerStorage : public ICustomerStorage
{
public:
    MOCK_METHOD(int, Add, (Customer customer), (override));
    MOCK_METHOD(void, Update, (Customer customer), (override));
    MOCK_METHOD(std::optional<Customer>, Get, (int id), (override));
    MOCK_METHOD(std::vector<Customer>, Get, (std::function<bool(Customer)> predicate), (override));
    MOCK_METHOD(std::vector<Customer>, GetAll, (), (override));
    MOCK_METHOD(void, Remove, (int id), (override));
};

class MockCashbackStrategy : public ICashbackStrategy
{
public:
    MOCK_METHOD(double, CalcCashback, (double receipt_total), (override));
};



class CashbackServiceTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        customer_storage = std::make_shared<MockCustomerStorage>();
        service = new CashbackService(customer_storage);

        cashback_strategy = std::make_shared<MockCashbackStrategy>();
    }

    void TearDown() override
    {
        delete service;
    }

    std::shared_ptr<MockCustomerStorage> customer_storage;
    CashbackService* service;

    std::shared_ptr<MockCashbackStrategy> cashback_strategy;
};



TEST_F(CashbackServiceTest, CreateCustomerWithValidDataCreatesCustomerAndSetsbalanceToZero)
{
    EXPECT_CALL(*customer_storage, Add(Customer(0, "name", "surname", 0, cashback_strategy))).Times(1).WillOnce(Return(42));

    EXPECT_EQ(service->CreateCustomer("name", "surname", cashback_strategy), 42);
}

TEST_F(CashbackServiceTest, CreateCustomerWithEmptyNameThrowsException)
{
    EXPECT_ANY_THROW(service->CreateCustomer("", "surname", std::make_shared<MockCashbackStrategy>()));
}

TEST_F(CashbackServiceTest, CreateCustomerWithEmptySurnameThrowsException)
{
    EXPECT_ANY_THROW(service->CreateCustomer("name", "", std::make_shared<MockCashbackStrategy>()));
}

TEST_F(CashbackServiceTest, CreateCustomerWithoutCashbackStrategyThrowsException)
{
    EXPECT_ANY_THROW(service->CreateCustomer("name", "surname", nullptr));
}

TEST_F(CashbackServiceTest, SetNameForExistingCustomerChangesName)
{
    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(Customer(42, "Till", "Lindemann", 123, cashback_strategy)));
    EXPECT_CALL(*customer_storage, Update(Customer(42, "New name", "Lindemann", 123, cashback_strategy))).Times(1);

    EXPECT_NO_THROW(service->SetCustomerName(42, "New name"));
}

TEST_F(CashbackServiceTest, SetEmptyNameForExistingCustomerThrowsException)
{
    ON_CALL(*customer_storage, Get(42)).WillByDefault(Return(Customer(42, "Till", "Lindemann", 123, cashback_strategy)));

    EXPECT_ANY_THROW(service->SetCustomerName(42, ""));
}

TEST_F(CashbackServiceTest, SetNameForUnexistingCustomerThrowsException)
{
    ON_CALL(*customer_storage, Get(42)).WillByDefault(Return(std::nullopt));

    EXPECT_ANY_THROW(service->SetCustomerName(42, "New name"));
}

TEST_F(CashbackServiceTest, SetSurnameForExistingCustomerChangesSurname)
{
    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(Customer(42, "Till", "Lindemann", 123, cashback_strategy)));
    EXPECT_CALL(*customer_storage, Update(Customer(42, "Till", "New surname", 123, cashback_strategy))).Times(1);

    EXPECT_NO_THROW(service->SetCustomerSurname(42, "New surname"));
}

TEST_F(CashbackServiceTest, SetEmptySurnameForExistingCustomerThrowsException)
{
    ON_CALL(*customer_storage, Get(42)).WillByDefault(Return(Customer(42, "Till", "Lindemann", 123, cashback_strategy)));

    EXPECT_ANY_THROW(service->SetCustomerSurname(42, ""));
}

TEST_F(CashbackServiceTest, SetSurnameForUnexistingCustomerThrowsException)
{
    ON_CALL(*customer_storage, Get(42)).WillByDefault(Return(std::nullopt));

    EXPECT_ANY_THROW(service->SetCustomerSurname(42, "New surname"));
}

TEST_F(CashbackServiceTest, GetCustomerForExistingCustomerReturnsCustomer)
{
    Customer customer(42, "name", "surname", 123, cashback_strategy);

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(customer));

    EXPECT_EQ(service->GetCustomer(42), customer);
}

TEST_F(CashbackServiceTest, GetCustomerForUnexistingCustomerReturnsNullopt)
{
    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_EQ(service->GetCustomer(42), std::nullopt);
}

TEST_F(CashbackServiceTest, UseCashbackDecreasesCustomerBalance)
{
    Customer customer(42, "name", "surname", 123, cashback_strategy);
    Customer customer_after(42, "name", "surname", 100, cashback_strategy);

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(customer));
    EXPECT_CALL(*customer_storage, Update(customer_after)).Times(1);

    service->UseCashback(42, 23);
}

TEST_F(CashbackServiceTest, UseCashbackWithInsuffitientBalanceThrowsException)
{
    Customer customer(42, "name", "surname", 123, cashback_strategy);

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(customer));

    EXPECT_ANY_THROW(service->UseCashback(42, 124));
}

TEST_F(CashbackServiceTest, UseCashbackForUnexistingCustomerThrowsException)
{
    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_ANY_THROW(service->UseCashback(42, 1));
}

TEST_F(CashbackServiceTest, AddCashbackForExistingCustomerAddsCashback)
{
    std::shared_ptr<MockCashbackStrategy> cashback_strategy = std::make_shared<MockCashbackStrategy>();

    Customer customer(42, "name", "surname", 100, cashback_strategy);
    Customer customer_after(42, "name", "surname", 150, cashback_strategy);

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(customer));
    EXPECT_CALL(*cashback_strategy, CalcCashback(500)).Times(1).WillOnce(Return(50));
    EXPECT_CALL(*customer_storage, Update(customer_after)).Times(1);

    service->AddCashback(42, 500);
}

TEST_F(CashbackServiceTest, AddCashbackForUnexistingCustomerThrowsException)
{
    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(std::nullopt));

    EXPECT_ANY_THROW(service->AddCashback(42, 500));
}

TEST_F(CashbackServiceTest, AddCashbackWithNegativeReceiptTotalThrowsException)
{    std::shared_ptr<MockCashbackStrategy> cashback_strategy = std::make_shared<MockCashbackStrategy>();

    Customer customer(42, "name", "surname", 100, cashback_strategy);

    ON_CALL(*customer_storage, Get(42)).WillByDefault(Return(customer));

    EXPECT_ANY_THROW(service->AddCashback(42, -1));
}