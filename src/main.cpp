#include "Core/IProductStorage.h"
#include "Core/IReceiptStorage.h"
#include "Bussiness/RegularDiscount.h"
#include "Bussiness/BundleDiscount.h"
#include "Bussiness/ProductService.h"
#include "Bussiness/ReceiptService.h"
#include "Bussiness/DiscountService.h"
#include <iostream>
#include <unordered_map>

class InMemProductStorage : public IProductStorage
{
public:
    int AddProduct(Product product) override
    {
        int id = m_NextID++;
        product.SetID(id);
        m_Products.emplace(id, product);
        return id;
    }

    void UpdateProduct(Product product) override
    {
        auto it = m_Products.find(product.GetID());
        if(it == m_Products.end()) throw std::runtime_error("Product does not exist");
        it->second = product;
    }

    Product GetProduct(int id) override
    {
        return m_Products.at(id);
    }

    std::vector<Product> GetProducts(std::function<bool(Product)> predicate) override
    {
        std::vector<Product> products;
        for(auto& [id, product] : m_Products)
        {
            if(predicate(product)) products.push_back(product);
        }
        return products;
    }

    std::vector<Product> GetAllProducts() override
    {
        std::vector<Product> products;
        for(auto& product : m_Products) products.push_back(product.second);
        return products;
    }

    void RemoveProduct(int id) override
    {
        m_Products.erase(id);
    }

private:
    std::unordered_map<int, Product> m_Products;
    int m_NextID = 1;
};

class InMemReceiptStorage : public IReceiptStorage
{
public:
    int AddReceipt(Receipt receipt) override
    {
        int id = m_NextID++;
        receipt.SetID(id);
        m_Receipts.emplace(id, receipt);
        return id;
    }

    void UpdateReceipt(Receipt receipt) override
    {
        auto it = m_Receipts.find(receipt.GetID());
        if(it == m_Receipts.end()) throw std::runtime_error("Receipt does not exist");
        it->second = receipt;
    }

    Receipt GetReceipt(int id) override
    {
        return m_Receipts.at(id);
    }

    std::vector<Receipt> GetReceipts(std::function<bool(Receipt)> predicate) override
    {
        std::vector<Receipt> receipts;
        for(auto& [id, receipt] : m_Receipts)
        {
            if(predicate(receipt)) receipts.push_back(receipt);
        }
        return receipts;
    }

    std::vector<Receipt> GetAllReceipts() override
    {
        std::vector<Receipt> receipts;
        for(auto& receipt : m_Receipts) receipts.push_back(receipt.second);
        return receipts;
    }

    void RemoveReceipt(int id) override
    {
        m_Receipts.erase(id);
    }

private:
    std::unordered_map<int, Receipt> m_Receipts;
    int m_NextID = 1;
};

int main()
{
    std::shared_ptr<IProductStorage> prod_stor = std::make_shared<InMemProductStorage>();
    std::shared_ptr<IReceiptStorage> rece_stor = std::make_shared<InMemReceiptStorage>();

    ProductService prod_serv(prod_stor);
    DiscountService disc_serv(prod_stor);
    ReceiptService rece_serv(prod_stor, rece_stor);

    int id1 = prod_serv.CreateProduct("Winr", 200);
    int id2 = prod_serv.CreateProduct("Bread", 30);
    int id3 = prod_serv.CreateProduct("Coockies", 40);
    prod_serv.SetName(id1, "Wine");

    disc_serv.SetDiscount(id1, std::make_shared<RegularDiscount>(0.2));
    disc_serv.SetDiscount(id3, std::make_shared<BundleDiscount>(3, 1.0));

    prod_serv.SetPrice(id3, 50);

    rece_serv.CreateReceipt({{id1, 5}, {id2, 10}, {id3, 8}});

    std::cout << "--- Products ---\n";
    for(auto& p : prod_serv.GetAll())
    {
        std::cout << "\t" << p.GetID() << ". " << p.GetName() << " " << p.GetPrice() << "$\n";
    }
    std::cout << "\n";

    std::cout << "--- Discounted ---\n";
    for(auto& dp : disc_serv.GetDiscountedProducts())
    {
        std::cout << "\t" << dp.GetID() << ". " << dp.GetName() << "\n";
    }
    std::cout << "\n";

    std::cout << "--- Receipts ---\n";
    for(auto& r : rece_serv.GetAllReceipts())
    {
        std::cout << "\t" << r.GetID() << ". " << r.CalcTotal() << "$ Timestamp: " << r.GetTimestamp() << "\n";
        for(auto& item : r.GetItems())
        {
            std::cout << "\t\t" << item.GetQuantity() << "x " << item.GetProduct().GetName() << ": " << item.CalcPrice() << "\n";
        }
    }
    std::cout << "\n";
}