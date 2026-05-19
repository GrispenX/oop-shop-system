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
    MOCK_METHOD(Customer, Get, (int id), (override));
    MOCK_METHOD(std::vector<Customer>, Get, (std::function<bool(Customer)> predicate), (override));
    MOCK_METHOD(std::vector<Customer>, GetAll, (), (override));
    MOCK_METHOD(void, Remove, (int id), (override));
};

class MockCashbackStrategy : public ICashbackStrategy
{
public:
    MOCK_METHOD(double, CalcCashback, (double receipt_total), (override));
};

TEST(CashbackServiceTest, CreateCustomerWithValidDataCreatesCustomerAndSetsbalanceToZero)
{
    std::shared_ptr<MockCustomerStorage> customer_storage = std::make_shared<MockCustomerStorage>();
    std::shared_ptr<MockCashbackStrategy> cashback_strategy = std::make_shared<MockCashbackStrategy>();

    EXPECT_CALL(*customer_storage, Add(Customer(0, "name", "surname", 0, cashback_strategy))).Times(1).WillOnce(Return(42));

    CashbackService service(customer_storage);

    EXPECT_EQ(service.CreateCustomer("name", "surname", cashback_strategy), 42);
}

TEST(CashbackServiceTest, CreateCustomerWithoutCashbackStrategyThrowsException)
{
    std::shared_ptr<MockCustomerStorage> customer_storage = std::make_shared<MockCustomerStorage>();

    CashbackService service(customer_storage);

    EXPECT_ANY_THROW(service.CreateCustomer("name", "surname", nullptr));
}

TEST(CashbackServiceTest, GetCustomerForExistingCustomerReturnsCustomer)
{
    std::shared_ptr<MockCustomerStorage> customer_storage = std::make_shared<MockCustomerStorage>();
    std::shared_ptr<MockCashbackStrategy> cashback_strategy = std::make_shared<MockCashbackStrategy>();

    Customer customer(42, "name", "surname", 123, cashback_strategy);

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(customer));

    CashbackService service(customer_storage);

    EXPECT_EQ(service.GetCustomer(42), customer);
}

TEST(CashbackServiceTest, GetCustomerForUnexistingCustomerReturnsNullopt)
{
    std::shared_ptr<MockCustomerStorage> customer_storage = std::make_shared<MockCustomerStorage>();

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Throw(std::runtime_error("Customer not found")));

    CashbackService service(customer_storage);

    EXPECT_EQ(service.GetCustomer(42), std::nullopt);
}

TEST(CashbackServiceTest, UseCashbackDecreasesCustomerBalance)
{
    std::shared_ptr<MockCustomerStorage> customer_storage = std::make_shared<MockCustomerStorage>();
    std::shared_ptr<MockCashbackStrategy> cashback_strategy = std::make_shared<MockCashbackStrategy>();

    Customer customer(42, "name", "surname", 123, cashback_strategy);
    Customer customer_after(42, "name", "surname", 100, cashback_strategy);

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(customer));
    EXPECT_CALL(*customer_storage, Update(customer_after)).Times(1);

    CashbackService service(customer_storage);

    service.UseCashback(42, 23);
}

TEST(CashbackServiceTest, UseCashbackWithInsuffitientBalanceThrowsException)
{
    std::shared_ptr<MockCustomerStorage> customer_storage = std::make_shared<MockCustomerStorage>();
    std::shared_ptr<MockCashbackStrategy> cashback_strategy = std::make_shared<MockCashbackStrategy>();

    Customer customer(42, "name", "surname", 123, cashback_strategy);

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(customer));

    CashbackService service(customer_storage);

    EXPECT_ANY_THROW(service.UseCashback(42, 124));
}

TEST(CashbackServiceTest, UseCashbackForUnexistingCustomerThrowsException)
{
    std::shared_ptr<MockCustomerStorage> customer_storage = std::make_shared<MockCustomerStorage>();

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Throw(std::runtime_error("Customer not found")));

    CashbackService service(customer_storage);

    EXPECT_ANY_THROW(service.UseCashback(42, 1));
}

TEST(CashbackServiceTest, AddCashbackForExistingCustomerAddsCashback)
{
    std::shared_ptr<MockCustomerStorage> customer_storage = std::make_shared<MockCustomerStorage>();
    std::shared_ptr<MockCashbackStrategy> cashback_strategy = std::make_shared<MockCashbackStrategy>();

    Customer customer(42, "name", "surname", 100, cashback_strategy);
    Customer customer_after(42, "name", "surname", 150, cashback_strategy);

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Return(customer));
    EXPECT_CALL(*cashback_strategy, CalcCashback(500)).Times(1).WillOnce(Return(50));
    EXPECT_CALL(*customer_storage, Update(customer_after)).Times(1);

    CashbackService service(customer_storage);

    service.AddCashback(42, 500);
}

TEST(CashbackServiceTest, AddCashbackForUnexistingCustomerThrowsException)
{
    std::shared_ptr<MockCustomerStorage> customer_storage = std::make_shared<MockCustomerStorage>();

    EXPECT_CALL(*customer_storage, Get(42)).Times(1).WillOnce(Throw(std::runtime_error("Customer not found")));

    CashbackService service(customer_storage);

    EXPECT_ANY_THROW(service.AddCashback(42, 500));
}

TEST(CashbackServiceTest, AddCashbackWithNegativeReceiptTotalThrowsException)
{
    std::shared_ptr<MockCustomerStorage> customer_storage = std::make_shared<MockCustomerStorage>();
    std::shared_ptr<MockCashbackStrategy> cashback_strategy = std::make_shared<MockCashbackStrategy>();

    Customer customer(42, "name", "surname", 100, cashback_strategy);

    ON_CALL(*customer_storage, Get(42)).WillByDefault(Return(customer));

    CashbackService service(customer_storage);

    EXPECT_ANY_THROW(service.AddCashback(42, -1));
}